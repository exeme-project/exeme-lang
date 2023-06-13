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
	enum ASTIdentifiers {
		AST_VARIABLE,
	} IDENTIFIER;
	union {
		struct AST_VARIABLE {
			const struct LexerToken *_token;
			const struct String *NAME;
		} *AST_VARIABLE;
	} data;
};

#define AST_STRUCT_SIZE sizeof(struct AST)
#define AST_VARIABLE_STRUCT_SIZE sizeof(struct AST_VARIABLE)

void ast_variable_free(struct AST_VARIABLE *self) {
	if (self) {
		lexerToken_free(self->_token);
		string_free(self->NAME);

		free(self);
		self = NULL;
	} else {
		panic("AST_VARIABLE struct has already been freed");
	}
}

struct AST *ast_new_(enum ASTIdentifiers IDENTIFIER, void *data) {
	struct AST *self = malloc(AST_STRUCT_SIZE);

	if (!self) {
		panic("failed to malloc AST struct");
	}

	self->IDENTIFIER = IDENTIFIER;

	switch (self->IDENTIFIER) {
	case AST_VARIABLE:
		self->data.AST_VARIABLE = malloc(AST_VARIABLE_STRUCT_SIZE);

		if (!self->data.AST_VARIABLE) {
			panic("failed to malloc AST_VARIABLE struct");
		}

		memcpy(self->data.AST_VARIABLE, data, AST_VARIABLE_STRUCT_SIZE);
		break;
	}

	return self;
}

/* Vararg macro to reduce boilerplate */
#define ast_new(type, ...) ast_new_(type, &(struct type){__VA_ARGS__})

void ast_free(struct AST *self) {
	if (self) {
		switch (self->IDENTIFIER) {
		case AST_VARIABLE:
			ast_variable_free(self->data.AST_VARIABLE);
			break;
		}

		free(self);
		self = NULL;
	} else {
		panic("AST struct has already been freed");
	}
}