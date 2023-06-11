/**
 * Part of the Exeme Language Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include "../includes.c"

#include "../lexer/lexer.c"
#include "../utils/array.c"
#include "../utils/errors.c"

/**
 * Represents an AST.
 */
struct AST {
	enum {
		AST_VARIABLE,
	} IDENTIFIER;
	union {
		struct AST_VARIABLE {
			const struct LexerToken *_TOKEN;
			const struct String *NAME;
		} AST_VARIABLE;
	} data;
};

#define AST_STRUCT_SIZE sizeof(struct AST)

struct AST *_ast_new(struct AST ast) {
	struct AST *self = malloc(AST_STRUCT_SIZE);

	if (!self) {
		panic("failed to malloc AST struct");
	}

	*self = ast; // Copy to heap from stack
	return self;
}

/* Vararg macro to reduce boilerplate */
#define ast_new(type, ...)                                                     \
	_ast_new((struct AST){type, {.type = (struct type){__VA_ARGS__}}})

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
		lexer_free(self->lexer);

		free(self);
		self = NULL;
	} else {
		panic("Parser struct has already been freed");
	}
}

bool parser_parse(void) { return false; }
