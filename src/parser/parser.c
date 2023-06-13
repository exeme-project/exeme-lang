/**
 * Part of the Exeme Language Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include "../includes.c"

#include "../lexer/lexer.c"
#include "../utils/array.c"
#include "./tokens.c"

/**
 * Represents a parser.
 */
struct Parser {
	struct Array *lexerTokens;
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

	self->lexerTokens = array_new();
	self->AST = NULL;
	self->lexer = lexer_new(FILE_PATH);

	return self;
}

/**
 * Frees a Parser struct.
 *
 * @param self The current Parser struct.
 */
void parser_free(struct Parser *self) {
	if (self) {
		array_free(self->lexerTokens);

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

void parser_parseNext(struct Parser *self) {}

/**
 * Gets the next lexer token and parses it.
 *
 * @param self     The current Parser struct.
 *
 * @return bool Whether parsing succeeded.
 */
bool parser_parse(struct Parser *self) {
	array_clear(self->lexerTokens);

	if (self->AST) {
		ast_free(self->AST);
	}

	do {
		if (!lexer_lex(self->lexer, true)) {
			return false;
		}

		parser_parseNext(self);
	} while (self->AST != NULL);

	return true;
}
