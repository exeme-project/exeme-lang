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
bool parser_parse(struct Parser *self, bool freeParserTokens, bool nextLine);

/**
 * Frees a Parser struct.
 *
 * @param self The current Parser struct.
 */
void parser_free(struct Parser **self) {
	struct AST *parserToken = NULL;

	if (self && *self) {
		if ((*self)->parserTokens) {
			for (size_t index = 0; index < (*self)->parserTokens->length;
				 index++) {
				parserToken =
					(struct AST *)array_get((*self)->parserTokens, index);

				ast_free(&parserToken);
			}

			array_free(&(*self)->parserTokens);
		}

		if ((*self)->AST) {
			ast_free(&(*self)->AST);
		}

		lexer_free(&(*self)->lexer);

		free((*self));
		(*self) = NULL;
	} else {
		panic("Parser struct has already been freed");
	}
}

/**
 * Parses the current chr or string.
 *
 * @param self The current Parser struct.
 */
void parser_parseChrOrString(struct Parser *self,
							 const struct LexerToken *lexerToken) {
	if (lexerToken->identifier == LEXERTOKENS_CHR) {
		array_insert(self->parserTokens, self->parserTokens->length,
					 ast_new(ASTTOKENS_CHR, AST_CHR, lexerToken,
							 string_new(lexerToken->value->_value, true)));
	} else {
		array_insert(self->parserTokens, self->parserTokens->length,
					 ast_new(ASTTOKENS_STRING, AST_STRING, lexerToken,
							 string_new(lexerToken->value->_value, true)));
	}
}

/**
 * Parses the current number.
 *
 * @param self The current Parser struct.
 */
void parser_parseNumber(struct Parser *self,
						const struct LexerToken *lexerToken) {
	if (lexerToken->identifier == LEXERTOKENS_INTEGER) {
		array_insert(self->parserTokens, self->parserTokens->length,
					 ast_new(ASTTOKENS_INTEGER, AST_INTEGER, lexerToken,
							 string_new(lexerToken->value->_value, true)));
	} else {
		array_insert(self->parserTokens, self->parserTokens->length,
					 ast_new(ASTTOKENS_FLOAT, AST_FLOAT, lexerToken,
							 string_new(lexerToken->value->_value, true)));
	}
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
				 ast_new(ASTTOKENS_VARIABLE, AST_VARIABLE, pointer, lexerToken,
						 string_new(lexerToken->value->_value, true)));
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

	if (identifier->IDENTIFIER != ASTTOKENS_VARIABLE) {
		lexer_error(self->lexer, error_get(P0002),
					stringConcatenate(5, "expected parser token of type '",
									  astTokens_getName(ASTTOKENS_VARIABLE),
									  "' before assignment, got '",
									  astTokens_getName(identifier->IDENTIFIER),
									  "'"),
					lexerToken);
	}

	parser_parse(self, false, false);

	if (self->parserTokens->length != 1) {
		lexer_error(self->lexer, error_get(P0003),
					stringConcatenate(
						2, "expected 1 parser token after assignment, got ",
						ulToString(self->parserTokens->length)),
					lexerToken);
	}

	value = (struct AST *)array_get(self->parserTokens, 0);

	switch (lexerToken->identifier) {
	case LEXERTOKENS_ASSIGNMENT:
		self->AST = ast_new(
			ASTTOKENS_ASSIGNMENT, AST_ASSIGNMENT, lexerToken,
			(const struct AST_VARIABLE *)identifier->data.AST_ASSIGNMENT,
			value);
		break;
	case LEXERTOKENS_MODULO_ASSIGNMENT:
		self->AST = ast_new(
			ASTTOKENS_MODULO_ASSIGNMENT, AST_MODULO_ASSIGNMENT, lexerToken,
			(const struct AST_VARIABLE *)identifier->data.AST_ASSIGNMENT,
			value);
		break;
	case LEXERTOKENS_MULTIPLICATION_ASSIGNMENT:
		self->AST = ast_new(
			ASTTOKENS_MULTIPLICATION_ASSIGNMENT, AST_MULTIPLICATION_ASSIGNMENT,
			lexerToken,
			(const struct AST_VARIABLE *)identifier->data.AST_ASSIGNMENT,
			value);
		break;
	case LEXERTOKENS_EXPONENT_ASSIGNMENT:
		self->AST = ast_new(
			ASTTOKENS_EXPONENT_ASSIGNMENT, AST_EXPONENT_ASSIGNMENT, lexerToken,
			(const struct AST_VARIABLE *)identifier->data.AST_ASSIGNMENT,
			value);
		break;
	case LEXERTOKENS_DIVISION_ASSIGNMENT:
		self->AST = ast_new(
			ASTTOKENS_DIVISION_ASSIGNMENT, AST_DIVISION_ASSIGNMENT, lexerToken,
			(const struct AST_VARIABLE *)identifier->data.AST_ASSIGNMENT,
			value);
		break;
	case LEXERTOKENS_FLOOR_DIVISION_ASSIGNMENT:
		self->AST = ast_new(
			ASTTOKENS_FLOOR_DIVISION_ASSIGNMENT, AST_FLOOR_DIVISION_ASSIGNMENT,
			lexerToken,
			(const struct AST_VARIABLE *)identifier->data.AST_ASSIGNMENT,
			value);
		break;
	case LEXERTOKENS_ADDITION_ASSIGNMENT:
		self->AST = ast_new(
			ASTTOKENS_ADDITION_ASSIGNMENT, AST_ADDITION_ASSIGNMENT, lexerToken,
			(const struct AST_VARIABLE *)identifier->data.AST_ASSIGNMENT,
			value);
		break;
	case LEXERTOKENS_SUBTRACTION_ASSIGNMENT:
		self->AST = ast_new(
			ASTTOKENS_SUBTRACTION_ASSIGNMENT, AST_SUBTRACTION_ASSIGNMENT,
			lexerToken,
			(const struct AST_VARIABLE *)identifier->data.AST_ASSIGNMENT,
			value);
		break;
	case LEXERTOKENS_BITWISE_AND_ASSIGNMENT:
		self->AST = ast_new(
			ASTTOKENS_BITWISE_AND_ASSIGNMENT, AST_BITWISE_AND_ASSIGNMENT,
			lexerToken,
			(const struct AST_VARIABLE *)identifier->data.AST_ASSIGNMENT,
			value);
		break;
	case LEXERTOKENS_BITWISE_OR_ASSIGNMENT:
		self->AST = ast_new(
			ASTTOKENS_BITWISE_OR_ASSIGNMENT, AST_BITWISE_OR_ASSIGNMENT,
			lexerToken,
			(const struct AST_VARIABLE *)identifier->data.AST_ASSIGNMENT,
			value);
		break;
	case LEXERTOKENS_BITWISE_XOR_ASSIGNMENT:
		self->AST = ast_new(
			ASTTOKENS_BITWISE_XOR_ASSIGNMENT, AST_BITWISE_XOR_ASSIGNMENT,
			lexerToken,
			(const struct AST_VARIABLE *)identifier->data.AST_ASSIGNMENT,
			value);
		break;
	case LEXERTOKENS_BITWISE_NOT_ASSIGNMENT:
		self->AST = ast_new(
			ASTTOKENS_BITWISE_NOT_ASSIGNMENT, AST_BITWISE_NOT_ASSIGNMENT,
			lexerToken,
			(const struct AST_VARIABLE *)identifier->data.AST_ASSIGNMENT,
			value);
		break;
	case LEXERTOKENS_BITWISE_LEFT_SHIFT_ASSIGNMENT:
		self->AST = ast_new(
			ASTTOKENS_BITWISE_LEFT_SHIFT_ASSIGNMENT,
			AST_BITWISE_LEFT_SHIFT_ASSIGNMENT, lexerToken,
			(const struct AST_VARIABLE *)identifier->data.AST_ASSIGNMENT,
			value);
		break;
	case LEXERTOKENS_BITWISE_RIGHT_SHIFT_ASSIGNMENT:
		self->AST = ast_new(
			ASTTOKENS_BITWISE_RIGHT_SHIFT_ASSIGNMENT,
			AST_BITWISE_RIGHT_SHIFT_ASSIGNMENT, lexerToken,
			(const struct AST_VARIABLE *)identifier->data.AST_ASSIGNMENT,
			value);
		break;
	}

	free(identifier); // Free the struct without freeing the inner data
	array_clear(self->parserTokens,
				NULL); // Clear the array without freeing the inner data
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
	case LEXERTOKENS_CHR:
	case LEXERTOKENS_STRING:
		parser_parseChrOrString(self, lexerToken);
		break;
	case LEXERTOKENS_INTEGER:
	case LEXERTOKENS_FLOAT:
		parser_parseNumber(self, lexerToken);
		break;
	case LEXERTOKENS_IDENTIFIER:
		parser_parseIdentifier(self, lexerToken, lexerTokenIndex);
		break;
	case LEXERTOKENS_ASSIGNMENT:
	case LEXERTOKENS_MODULO_ASSIGNMENT:
	case LEXERTOKENS_MULTIPLICATION_ASSIGNMENT:
	case LEXERTOKENS_EXPONENT_ASSIGNMENT:
	case LEXERTOKENS_DIVISION_ASSIGNMENT:
	case LEXERTOKENS_FLOOR_DIVISION_ASSIGNMENT:
	case LEXERTOKENS_ADDITION_ASSIGNMENT:
	case LEXERTOKENS_SUBTRACTION_ASSIGNMENT:
	case LEXERTOKENS_BITWISE_AND_ASSIGNMENT:
	case LEXERTOKENS_BITWISE_OR_ASSIGNMENT:
	case LEXERTOKENS_BITWISE_XOR_ASSIGNMENT:
	case LEXERTOKENS_BITWISE_NOT_ASSIGNMENT:
	case LEXERTOKENS_BITWISE_LEFT_SHIFT_ASSIGNMENT:
	case LEXERTOKENS_BITWISE_RIGHT_SHIFT_ASSIGNMENT:
		parser_parseAssignment(self, lexerToken);
		break;
	default:
		printf("unsupported lexer token for parser: %s\n",
			   lexerTokens_getName(lexerToken->identifier));
		break;
	}
}

/**
 * Gets the next lexer token and parses it.
 *
 * @param self             The current Parser struct.
 * @param freeParserTokens Whether to free the parser tokens when clearing
 * the array.
 * @param nextLine         Whether to get the next line from the lexer.
 *
 * @return bool Whether parsing succeeded.
 */
bool parser_parse(struct Parser *self, bool freeParserTokens, bool nextLine) {
	array_clear(self->parserTokens,
				freeParserTokens ? (void (*)(const void *))ast_free : NULL);

	if (self->AST) {
		ast_free(&self->AST);
	}

	do {
		if (!lexer_lex(self->lexer, nextLine)) {
			return false;
		}

		parser_parseNext(self);
	} while (self->AST == NULL);

	return true;
}
