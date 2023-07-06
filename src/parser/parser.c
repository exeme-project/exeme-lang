/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include "../includes.c"

#include "../lexer/lexer.c"
#include "../utils/array.c"
#include "../utils/string.c"
#include "./tokens.c"

/**
 * Represents a parser.
 */
struct Parser {
	struct Array *parserTokens;
	struct AST *AST;
	struct Lexer *lexer;
};

#define PARSER_STRUCT_SIZE sizeof(struct Parser)

/**
 * Creates a new Parser struct.
 *
 * @param FILE_PATH The path of the file to parse.
 *
 * @return The created Parser struct.
 */
struct Parser *parser_new(const char *FILE_PATH) {
	struct Parser *self = malloc(PARSER_STRUCT_SIZE);

	if (!self) {
		panic("failed to malloc Parser struct");
	}

	self->parserTokens = array_new();
	self->AST = NULL;
	self->lexer = lexer_new(FILE_PATH);

	return self;
}

/* Forward declarations to silence warnings */
bool parser_parse(struct Parser *self, bool freeParserTokens);

/**
 * Frees a Parser struct.
 *
 * @param self The current Parser struct.
 */
void parser_free(struct Parser *self) {
	if (self) {
		if (self->parserTokens) {
			for (size_t index = 0; index < self->parserTokens->length;
				 index++) {
				free((void *)array_get(self->parserTokens, index));
			}

			array_free(self->parserTokens);
		}

		if (self->AST) {
			ast_free(self->AST);
		}

		lexer_free(self->lexer);

		free(self);
		self = NULL;
	} else {
		panic("Parser struct has already been freed");
	}
}

/**
 * Parses the current number.
 *
 * @param self The current Parser struct.
 */
void parser_parseNumber(struct Parser *self,
						const struct LexerToken *lexerToken) {

	void* IDENTIFIER =
		(lexerToken->identifier == LEXERTOKENS_INTEGER) ? AST_INTEGER
														: AST_FLOAT;

	array_insert(self->parserTokens, self->parserTokens->length,
				 ast_new(AST_INTEGER, lexerToken, 0)); // TODO: Fix wrong value
}

/**
 * Parses the current identifier.
 *
 * @param self The current Parser struct.
 */
void parser_parseIdentifier(struct Parser *self,
							const struct LexerToken *lexerToken,
							size_t lexerTokenIndex) {
	bool pointer = false;

	if (self->lexer->tokens->length > 1) {
		if (((const struct LexerToken *)array_get(self->lexer->tokens,
												  lexerTokenIndex - 1))
				->identifier == LEXERTOKENS_MULTIPLICATION) {
			pointer = true;
		}
	}

	array_insert(self->parserTokens, self->parserTokens->length,
				 ast_new(AST_VARIABLE, pointer, lexerToken, lexerToken->value));
}

/**
 * Parses the current assignment.
 *
 * @param self The current Parser struct.
 */
void parser_parseAssignment(struct Parser *self,
							const struct LexerToken *lexerToken) {
	struct AST *identifier, *value = NULL;

	if (self->parserTokens->length != 1) {
		lexer_error(self->lexer, error_get(P0001),
					stringConcatenate(
						2, "expected 1 parser token before assignment, got ",
						ulToString(self->parserTokens->length)),
					lexerToken);
	}

	identifier = (struct AST *)array_get(self->parserTokens, 0);

	if (identifier->IDENTIFIER != AST_VARIABLE) {
		lexer_error(self->lexer, error_get(P0002),
					stringConcatenate(5, "expected parser token of type '",
									  astTokens_getName(AST_VARIABLE),
									  "' before assignment, got '",
									  astTokens_getName(identifier->IDENTIFIER),
									  "'"),
					lexerToken);
	}

	parser_parse(self, false);

	if (self->parserTokens->length != 1) {
		lexer_error(self->lexer, error_get(P0003),
					stringConcatenate(
						2, "expected 1 parser token after assignment, got ",
						ulToString(self->parserTokens->length)),
					lexerToken);
	}

	value = (struct AST *)array_get(self->parserTokens, 0);

	array_insert(
		self->parserTokens, self->parserTokens->length,
		ast_new(AST_ASSIGNMENT, lexerToken,
				(const struct AST_VARIABLE *)identifier->data.AST_ASSIGNMENT,
				value));

	free(identifier); // Free the struct without freeing the inner data
}

/**
 * Calls the correct function for lexing the current lexer token.
 *
 * @param self The current Parser struct.
 */
void parser_parseNext(struct Parser *self) {
	size_t lexerTokenIndex = 0;

	const struct LexerToken *lexerToken =
		lexer_getToken(self->lexer, &lexerTokenIndex);

	switch (lexerToken->identifier) {
	case LEXERTOKENS_INTEGER:
	case LEXERTOKENS_FLOAT:
		parser_parseNumber(self, lexerToken);
		break;
	case LEXERTOKENS_IDENTIFIER:
		parser_parseIdentifier(self, lexerToken, lexerTokenIndex);
		break;
	case LEXERTOKENS_ASSIGNMENT:
		parser_parseAssignment(self, lexerToken);
		break;
	default:
		break;
	}
}

/**
 * Gets the next lexer token and parses it.
 *
 * @param self             The current Parser struct.
 * @param freeParserTokens Whether to free the parser tokens when clearing the
 * array.
 *
 * @return bool Whether parsing succeeded.
 */
bool parser_parse(struct Parser *self, bool freeParserTokens) {
	array_clear(self->parserTokens,
				freeParserTokens ? (void (*)(const void *))ast_free : NULL);

	if (self->AST) {
		ast_free(self->AST);
	}

	do {
		if (!lexer_lex(self->lexer, true)) {
			return false;
		}

		parser_parseNext(self);
	} while (self->AST == NULL);

	return true;
}
