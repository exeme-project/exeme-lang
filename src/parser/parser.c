/**
 * Part of the Exeme Language Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include "../includes.c"

#include "../lexer/lexer.c"

/**
 * Represents a parser.
 */
struct Parser {
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

	self->lexer = lexer_new(FILE_PATH);

	return self;
}

/**
 * Calls the correct function for lexing the current Lexer token.
 *
 * @param self The current parser struct.
 *
 * @return bool - Whether parsing succeeded.
 */
bool parser_parseNext(struct Parser *self,
					  const struct LexerToken *lexerToken) {
	printf("identifier: %s\nstartChrIndex: %zu\nendChrIndex: "
		   "%zu\nlineIndex: "
		   "%zu\nvalue: %s\n\n",
		   (const char *)LEXER_TOKEN_NAMES._values[lexerToken->identifier],
		   lexerToken->startChrIndex, lexerToken->endChrIndex,
		   lexerToken->lineIndex, lexerToken->value->_value);

	switch (lexerToken->identifier) {
	case LEXERTOKENS_ASSIGNMENT:
		break;
	}

	return true;
}

/**
 * Gets the next Lexer token and parses it.
 *
 * @param self The current parser struct.
 *
 * @return bool Whether parsing succeeded.
 */
bool parser_parse(struct Parser *self) {
	if (!lexer_lex(self->lexer, true)) {
		return false;
	}

	return parser_parseNext(self, lexer_getToken(self->lexer));
}
