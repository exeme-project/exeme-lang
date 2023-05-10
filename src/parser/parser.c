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
enum ParserTokenIdentifiers {
	PARSERTOKENS_VARIABLE,

	// Assignment operators
	PARSERTOKENS_ASSIGNMENT,

	PARSERTOKENS_MODULO_ASSIGNMENT,
	PARSERTOKENS_MULTIPLICATION_ASSIGNMENT,
	PARSERTOKENS_EXPONENT_ASSIGNMENT,
	PARSERTOKENS_DIVISION_ASSIGNMENT,
	PARSERTOKENS_FLOOR_DIVISION_ASSIGNMENT,
	PARSERTOKENS_ADDITION_ASSIGNMENT,
	PARSERTOKENS_SUBTRACTION_ASSIGNMENT,

	PARSERTOKENS_BITWISE_AND_ASSIGNMENT,
	PARSERTOKENS_BITWISE_OR_ASSIGNMENT,
	PARSERTOKENS_BITWISE_XOR_ASSIGNMENT,
	PARSERTOKENS_BITWISE_NOT_ASSIGNMENT,
	PARSERTOKENS_BITWISE_LEFT_SHIFT_ASSIGNMENT,
	PARSERTOKENS_BITWISE_RIGHT_SHIFT_ASSIGNMENT,
};

/* Forward declarations to silence warnings */
struct Parser;
struct ParserToken;

bool parser_parse(struct Parser *self);
void parserToken_free(struct ParserToken *self);

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
 *
 * @return The created ParserTokenVariable struct.
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

/**
 * Frees an ParserTokenVariable struct.
 *
 * @param self The current ParserTokenVariable struct.
 */
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
 * Represents a parser token for an assignment.
 */
struct ParserTokenAssignment {
	struct ParserToken *operand1, *operand2;
};

#define PARSERTOKENASSIGNMENT_STRUCT_SIZE sizeof(struct ParserTokenAssignment)

/**
 * Creates a new ParserTokenAssignment.
 *
 * @return The created ParserTokenAssignment struct.
 */
struct ParserTokenAssignment *
parserTokenAssignment_new(struct ParserToken *operand1,
						  struct ParserToken *operand2) {
	struct ParserTokenAssignment *self =
		malloc(PARSERTOKENASSIGNMENT_STRUCT_SIZE);

	self->operand1 = operand1;
	self->operand2 = operand2;

	return self;
}

/**
 * Frees an ParserTokenVariable struct.
 *
 * @param self The current ParserTokenVariable struct.
 */
void parserTokenAssignment_free(struct ParserTokenAssignment *self) {
	if (self) {
		parserToken_free(self->operand1);
		parserToken_free(self->operand2);

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
		struct ParserTokenAssignment *ASSIGNMENT;
		struct ParserTokenVariable *VARIABLE;
	};
};

#define PARSERTOKEN_STRUCT_SIZE sizeof(struct ParserToken)

/**
 * Creates a new ParserToken struct.
 *
 * @param IDENTIFIER Token identifier.
 * @param VALUE      The token value.
 *
 * @return The created ParserToken struct.
 */
struct ParserToken *
parserToken_new(const enum ParserTokenIdentifiers IDENTIFIER, void *VALUE) {
	struct ParserToken *self = malloc(PARSERTOKEN_STRUCT_SIZE);

	if (!self) {
		panic("failed to malloc ParserToken struct");
	}

	self->identifier = IDENTIFIER;

	switch (self->identifier) {
	case PARSERTOKENS_ASSIGNMENT:
		self->ASSIGNMENT = VALUE;
		break;
	case PARSERTOKENS_VARIABLE:
		self->VARIABLE = VALUE;
		break;
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
		case PARSERTOKENS_ASSIGNMENT:
		case PARSERTOKENS_MODULO_ASSIGNMENT:
		case PARSERTOKENS_MULTIPLICATION_ASSIGNMENT:
		case PARSERTOKENS_EXPONENT_ASSIGNMENT:
		case PARSERTOKENS_DIVISION_ASSIGNMENT:
		case PARSERTOKENS_FLOOR_DIVISION_ASSIGNMENT:
		case PARSERTOKENS_ADDITION_ASSIGNMENT:
		case PARSERTOKENS_SUBTRACTION_ASSIGNMENT:
		case PARSERTOKENS_BITWISE_AND_ASSIGNMENT:
		case PARSERTOKENS_BITWISE_OR_ASSIGNMENT:
		case PARSERTOKENS_BITWISE_XOR_ASSIGNMENT:
		case PARSERTOKENS_BITWISE_NOT_ASSIGNMENT:
		case PARSERTOKENS_BITWISE_LEFT_SHIFT_ASSIGNMENT:
		case PARSERTOKENS_BITWISE_RIGHT_SHIFT_ASSIGNMENT:
			parserTokenAssignment_free(self->ASSIGNMENT);
			break;
		case PARSERTOKENS_VARIABLE:
			parserTokenVariable_free(self->VARIABLE);
			break;
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
 *
 * @return The created Parser struct.
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

bool parser_parseAssignment(struct Parser *self,
							const struct LexerToken *lexerToken) {
	if (!self->_token) {
		lexer_error(self->lexer,
					ERRORIDENTIFIER_NAMES._values[ERRORIDENTIFIERS_PARSER_2],
					"missing first operand for assignment", lexerToken);
	}

	return true;
}

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
		return parser_parseAssignment(self, lexerToken);
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
