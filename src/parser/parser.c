/**
 * Part of the Exeme Language Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include "../includes.c"

#include "../lexer/lexer.c"
#include "../utils/errors.c"

/**
 * Used to identify different parser tokens.
 */
enum ParserTokenIdentifiers { PARSERTOKENS_VARIABLE };

/**
 * Represent a parser token for a variable.
 */
struct ParserTokenVariable {
	size_t startChrIndex, endChrIndex, lineIndex;
	const struct String *value;
};

#define PARSERTOKENVARIABLE_STRUCT_SIZE sizeof(struct ParserTokenVariable)

/**
 * Creates a new ParserTokenVariable struct.
 *
 * @param startChrIndex Start char index of the token.
 * @param endChrIndex   End char index of the token.
 * @param lineIndex     Line index of the token.
 * @param value         Value of the token.
 */
struct ParserTokenVariable *
parserTokenVariable_new(size_t startChrIndex, size_t endChrIndex,
						size_t lineIndex, const struct String *value) {

	struct ParserTokenVariable *self = malloc(PARSERTOKENVARIABLE_STRUCT_SIZE);

	self->startChrIndex = startChrIndex;
	self->endChrIndex = endChrIndex;
	self->lineIndex = lineIndex;

	self->value = value;

	return self;
}

void parserTokenVariable_free(struct ParserTokenVariable *self) {
	if (self) {
		string_free(self->value);

		free(self);
		self = NULL;
	} else {
		panic("ParserTokenVariable struct has already been freed");
	}
}

/**
 * Represents a parser token.
 */
struct ParserToken {
	enum ParserTokenIdentifiers identifier;

	union {
		struct ParserTokenVariable *VARIABLE;
	};
};

#define PARSERTOKEN_STRUCT_SIZE sizeof(struct ParserToken)

/**
 * Creates a new ParserToken struct.
 *
 * @param IDENTIFIER Token identifier.
 * @param VALUE      The token value.
 */
struct ParserToken *
parserToken_new(const enum ParserTokenIdentifiers IDENTIFIER, void *VALUE) {
	struct ParserToken *self = malloc(PARSERTOKEN_STRUCT_SIZE);

	if (!self) {
		panic("failed to malloc ParserToken struct");
	}

	self->identifier = IDENTIFIER;

	switch (self->identifier) {
	case PARSERTOKENS_VARIABLE:
		self->VARIABLE = VALUE;
	}

	return self;
}

/**
 * Frees an ParserToken struct.
 *
 * @param self The current ParserToken struct.
 */
void parserToken_free(struct ParserToken *self) {
	if (self) {
		switch (self->identifier) {
		case PARSERTOKENS_VARIABLE:
			parserTokenVariable_free(self->VARIABLE);
		}

		free(self);
		self = NULL;
	} else {
		panic("ParserToken struct has already been freed");
	}
}

/**
 * Represents a parser.
 */
struct Parser {
	struct Lexer *lexer;
	struct ParserToken *_token, *token;
};

#define PARSER_STRUCT_SIZE sizeof(struct Parser)

/**
 * Creates a new Parser struct.
 *
 * @param FILE_PATH The path of the file to parse.
 */
struct Parser *parser_new(const char *FILE_PATH) {
	struct Parser *self = malloc(PARSER_STRUCT_SIZE);

	if (!self) {
		panic("failed to malloc Parser struct");
	}

	self->lexer = lexer_new(FILE_PATH);

	self->_token = NULL;
	self->token = NULL;

	return self;
}

/**
 * Frees an Parser struct.
 *
 * @param self The current Parser struct.
 */
void parser_free(const struct Parser *self) {
	if (self) {
		if (self->lexer) {
			lexer_free(self->lexer);
		}

		if (self->_token) {
			parserToken_free(self->_token);
		}

		if (self->token) {
			parserToken_free(self->token);
		}

		free(self);
		self = NULL;
	} else {
		panic("Parser struct has already been freed");
	}
}

bool parser_parse(struct Parser *self);

/**
 * Parses the current lexer token.
 *
 * @param self       The current parser struct.
 * @param lexerToken The current lexer token.
 *
 * @return Whether a parser token was created.
 */
bool parser_parseIdentifier(struct Parser *self,
							const struct LexerToken *lexerToken) {
	if (self->_token) {
		lexer_error(self->lexer,
					ERRORIDENTIFIER_NAMES._values[ERRORIDENTIFIERS_PARSER_1],
					"no effect of variable", lexerToken);
	}

	self->_token =
		parserToken_new(PARSERTOKENS_VARIABLE,
						parserTokenVariable_new(
							lexerToken->startChrIndex, lexerToken->endChrIndex,
							lexerToken->lineIndex, lexerToken->value));

	return false;
}

/**
 * Calls the correct function for parsing the current lexer token.
 *
 * @param self The current parser struct.
 *
 * @return Whether a parser token was created.
 */
bool parser_parseNext(struct Parser *self,
					  const struct LexerToken *lexerToken) {
	printf("identifier: %s\nstartChrIndex: %zu\nendChrIndex: "
		   "%zu\nlineIndex: "
		   "%zu\nvalue: %s\n\n",
		   (const char *)LEXERTOKEN_NAMES._values[lexerToken->identifier],
		   lexerToken->startChrIndex, lexerToken->endChrIndex,
		   lexerToken->lineIndex, lexerToken->value->_value);

	switch (lexerToken->identifier) {
	case LEXERTOKENS_IDENTIFIER:
		return parser_parseIdentifier(self, lexerToken);
	}

	return false;
}

/**
 * Gets the next lexer token and parses it.
 *
 * @param self The current parser struct.
 *
 * @return bool Whether parsing succeeded.
 */
bool parser_parse(struct Parser *self) {
	self->token = NULL;

	do {
		if (!lexer_lex(self->lexer, true)) {
			return false;
		}
	} while (!parser_parseNext(self, lexer_getToken(self->lexer)));

	return true;
}
