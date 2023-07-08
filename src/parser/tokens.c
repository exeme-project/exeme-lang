/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include "../includes.c"

#include "../errors.c"
#include "../lexer/tokens.c"
#include "../utils/array.c"

/**
 * Represents an AST.
 */
struct AST {
	/**
	 * Used to identify different AST tokens.
	 */
	enum ASTTokenIdentifiers {
		ASTTOKENS_INTEGER,
		ASTTOKENS_FLOAT,

		ASTTOKENS_VARIABLE,
		ASTTOKENS_ASSIGNMENT,
	} IDENTIFIER;
	union {
		/* Represents an integer in the AST. */
		struct AST_INTEGER {
			const struct LexerToken *_token;
			const int VALUE;
		} *AST_INTEGER;

		/* Represents a float in the AST. */
		struct AST_FLOAT {
			const struct LexerToken *_token;
			const float VALUE;
		} *AST_FLOAT;

		/* Represents a variable in the AST.*/
		struct AST_VARIABLE {
			const bool POINTER;
			const struct LexerToken *_token;
			const struct String *NAME;
		} *AST_VARIABLE;

		/* Represents an assignment in the AST */
		struct AST_ASSIGNMENT {
			const struct LexerToken *_token;
			const struct AST_VARIABLE *IDENTIFIER;
			const struct AST *VALUE;
		} *AST_ASSIGNMENT;
	} data;
};

#define AST_STRUCT_SIZE sizeof(struct AST)
#define AST_INTEGER_STRUCT_SIZE sizeof(struct AST_INTEGER)
#define AST_FLOAT_STRUCT_SIZE sizeof(struct AST_FLOAT)
#define AST_VARIABLE_STRUCT_SIZE sizeof(struct AST_VARIABLE)
#define AST_ASSIGNMENT_STRUCT_SIZE sizeof(struct AST_ASSIGNMENT)

/**
 * Contains the names of each of the AST token identifiers.
 */
const struct Array ASTTOKEN_NAMES = {
	4,
	(const void *[]){
		"AST_INTEGER",
		"AST_FLOAT",
		"AST_VARIABLE",
		"AST_ASSIGNMENT",
	},
};

/**
 * Gets the name of an AST token.
 *
 * @param IDENTIFIER The AST token's identifier.
 *
 * @return The name of the AST token.
 */
const char *astTokens_getName(const enum ASTTokenIdentifiers IDENTIFIER) {
	if ((size_t)IDENTIFIER + 1 > ASTTOKEN_NAMES.length) {
		panic("ASTTOKEN_NAMES get index out of bounds");
	}

	return ASTTOKEN_NAMES._values[IDENTIFIER];
}

/* Forward declarations */
void ast_free(struct AST *self);

/**
 * Frees an AST_INTEGER struct.
 *
 * @param self The current AST_INTEGER struct.
 */
void astInteger_free(struct AST_INTEGER *self) {
	if (self) {
		lexerToken_free((struct LexerToken *)self->_token);

		free(self);
		self = NULL;
	} else {
		panic("AST_INTEGER struct has already been freed");
	}
}

/**
 * Frees an AST_FLOAT struct.
 *
 * @param self The current AST_FLOAT struct.
 */
void astFloat_free(struct AST_FLOAT *self) {
	if (self) {
		lexerToken_free((struct LexerToken *)self->_token);

		free(self);
		self = NULL;
	} else {
		panic("AST_FLOAT struct has already been freed");
	}
}

/**
 * Frees an AST_VARIABLE struct.
 *
 * @param self The current AST_VARIABLE struct.
 */
void astVariable_free(struct AST_VARIABLE *self) {
	if (self) {
		lexerToken_free((struct LexerToken *)self->_token);
		string_free((struct String *)self->NAME);

		free(self);
		self = NULL;
	} else {
		panic("AST_VARIABLE struct has already been freed");
	}
}

/**
 * Frees an AST_ASSIGNMENT struct.
 *
 * @param self The current AST_ASSIGNMENT struct.
 */
void astAssignment_free(struct AST_ASSIGNMENT *self) {
	if (self) {
		lexerToken_free((struct LexerToken *)self->_token);
		ast_free((struct AST *)self->VALUE);
		astVariable_free((struct AST_VARIABLE *)self->IDENTIFIER);

		free(self);
		self = NULL;
	} else {
		panic("AST_ASSIGNMENT struct has already been freed");
	}
}

/**
 * WARNING: DO NOT USE - USE THE MACRO INSTEAD.
 *
 * Creates a new AST struct.
 *
 * @param IDENTIFIER The identifier of the AST.
 * @param data       The data of the AST.
 *
 * @return The created AST struct.
 */
struct AST *ast_new__(enum ASTTokenIdentifiers IDENTIFIER, void *data) {
	struct AST *self = malloc(AST_STRUCT_SIZE);
	void *astData = NULL;
	size_t astDataStructSize = 0;

	if (!self) {
		panic("failed to malloc AST struct");
	}

	self->IDENTIFIER = IDENTIFIER;

	switch (self->IDENTIFIER) {
	case ASTTOKENS_INTEGER:
		astData = self->data.AST_INTEGER = malloc(AST_INTEGER_STRUCT_SIZE);
		astDataStructSize = AST_INTEGER_STRUCT_SIZE;
		break;
	case ASTTOKENS_FLOAT:
		astData = self->data.AST_FLOAT = malloc(AST_FLOAT_STRUCT_SIZE);
		astDataStructSize = AST_FLOAT_STRUCT_SIZE;
		break;
	case ASTTOKENS_VARIABLE:
		astData = self->data.AST_VARIABLE = malloc(AST_VARIABLE_STRUCT_SIZE);
		astDataStructSize = AST_VARIABLE_STRUCT_SIZE;
		break;
	case ASTTOKENS_ASSIGNMENT:
		astData = self->data.AST_ASSIGNMENT =
			malloc(AST_ASSIGNMENT_STRUCT_SIZE);
		astDataStructSize = AST_ASSIGNMENT_STRUCT_SIZE;
		break;
	}

	if (!astData) {
		panic(stringConcatenate(3, "failed to malloc ",
								astTokens_getName(self->IDENTIFIER),
								" struct"));
	}

	memcpy(astData, data, astDataStructSize);

	return self;
}

/* Vararg macro to reduce boilerplate */
#define ast_new(identifier, type, ...)                                         \
	ast_new__(identifier, &(struct type){__VA_ARGS__})

/**
 * Frees an AST struct.
 *
 * @param self The current AST struct.
 */
void ast_free(struct AST *self) {
	if (self) {
		switch (self->IDENTIFIER) {
		case ASTTOKENS_INTEGER:
			astInteger_free(self->data.AST_INTEGER);
			break;
		case ASTTOKENS_FLOAT:
			astFloat_free(self->data.AST_FLOAT);
			break;
		case ASTTOKENS_VARIABLE:
			astVariable_free(self->data.AST_VARIABLE);
			break;
		case ASTTOKENS_ASSIGNMENT:
			astAssignment_free(self->data.AST_ASSIGNMENT);
			break;
		}

		free(self);
		self = NULL;
	} else {
		panic("AST struct has already been freed");
	}
}
