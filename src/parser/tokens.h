/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include "../lexer/lexer.h"

// X-Macro to define AST nodes
#define AST_NODES(X)                                                                               \
	X(CHR, AST_NODE_WITH_VALUE, struct String* value)                                              \
	X(STRING, AST_NODE_WITH_VALUE, struct String* value)                                           \
	X(INTEGER, AST_NODE_WITH_VALUE, struct String* value)                                          \
	X(FLOAT, AST_NODE_WITH_VALUE, struct String* value)                                            \
	X(VARIABLE, AST_NODE_WITH_VALUE, struct String* value)                                         \
	X(ASSIGNMENT, AST_NODE_ASSIGNMENT, struct AST_VARIABLE* identifier; struct AST * value)        \
	X(MODULO_ASSIGNMENT, AST_NODE_ASSIGNMENT, struct AST_VARIABLE* identifier; struct AST * value) \
	X(MULTIPLICATION_ASSIGNMENT, AST_NODE_ASSIGNMENT, struct AST_VARIABLE* identifier;             \
	  struct AST * value)                                                                          \
	X(EXPONENT_ASSIGNMENT, AST_NODE_ASSIGNMENT, struct AST_VARIABLE* identifier;                   \
	  struct AST * value)                                                                          \
	X(DIVISION_ASSIGNMENT, AST_NODE_ASSIGNMENT, struct AST_VARIABLE* identifier;                   \
	  struct AST * value)                                                                          \
	X(FLOOR_DIVISION_ASSIGNMENT, AST_NODE_ASSIGNMENT, struct AST_VARIABLE* identifier;             \
	  struct AST * value)                                                                          \
	X(ADDITION_ASSIGNMENT, AST_NODE_ASSIGNMENT, struct AST_VARIABLE* identifier;                   \
	  struct AST * value)                                                                          \
	X(SUBTRACTION_ASSIGNMENT, AST_NODE_ASSIGNMENT, struct AST_VARIABLE* identifier;                \
	  struct AST * value)                                                                          \
	X(BITWISE_AND_ASSIGNMENT, AST_NODE_ASSIGNMENT, struct AST_VARIABLE* identifier;                \
	  struct AST * value)                                                                          \
	X(BITWISE_OR_ASSIGNMENT, AST_NODE_ASSIGNMENT, struct AST_VARIABLE* identifier;                 \
	  struct AST * value)                                                                          \
	X(BITWISE_XOR_ASSIGNMENT, AST_NODE_ASSIGNMENT, struct AST_VARIABLE* identifier;                \
	  struct AST * value)                                                                          \
	X(BITWISE_NOT_ASSIGNMENT, AST_NODE_ASSIGNMENT, struct AST_VARIABLE* identifier;                \
	  struct AST * value)                                                                          \
	X(BITWISE_LEFT_SHIFT_ASSIGNMENT, AST_NODE_ASSIGNMENT, struct AST_VARIABLE* identifier;         \
	  struct AST * value)                                                                          \
	X(BITWISE_RIGHT_SHIFT_ASSIGNMENT, AST_NODE_ASSIGNMENT, struct AST_VARIABLE* identifier;        \
	  struct AST * value)                                                                          \
	X(OPEN_BRACE, AST_NODE_BASIC, /* No extra fields */)                                           \
	X(CLOSE_BRACE, AST_NODE_BASIC, /* No extra fields */)                                          \
	X(COMMA, AST_NODE_BASIC, /* No extra fields */)                                                \
	X(COLON, AST_NODE_BASIC, /* No extra fields */)                                                \
	X(FUNCTION_DEFINITION, AST_NODE_FUNCTION_DEFINITION, struct AST_VARIABLE* identifier;          \
	  struct AST_OPEN_BRACE * open_brace; struct Array * arguments; struct Array * argument_types; \
	  struct AST_CLOSE_BRACE * close_brace)

// X-Macro to define AST node identifiers
typedef enum {
#define ENUM_ENTRY(name, free, ...) ASTTOKENS_##name,
	AST_NODES(ENUM_ENTRY)
#undef ENUM_ENTRY
} ASTTokenIdentifiers;

static char* const g_ASTTOKEN_NAMES_INTERNAL[] = {
#define ENUM_ENTRY(name, free, ...) #name,
	AST_NODES(ENUM_ENTRY)
#undef ENUM_ENTRY
};

extern const struct Array g_ASTTOKEN_NAMES;

/**
 * Gets the name of an AST node identifier.
 *
 * @param IDENTIFIER The AST node identifier.
 *
 * @return The name of the AST node identifier.
 */
const char* ast_tokens_get_name(
	const ASTTokenIdentifiers IDENTIFIER); // NOLINT(readability-avoid-const-params-in-decls)

// X-Macro to define AST node structs
#define STRUCT_DEF(name, free, ...)                                                                \
	typedef struct AST_##name {                                                                    \
		const struct LexerToken* _token;                                                           \
		__VA_ARGS__                                                                                \
		__VA_OPT__(;) /* Only add a semicolon if __VA_ARGS__ is not empty */                       \
	} AST_##name;

// Generate AST node structs using AST_NODES
AST_NODES(STRUCT_DEF)

#undef STRUCT_DEF

// Represents an AST node
typedef struct AST {
	ASTTokenIdentifiers identifier;
	void (*free)(struct AST**); // Free function pointer
	union {
#define UNION_ENTRY(name, free, ...) struct AST_##name* name;
		AST_NODES(UNION_ENTRY)
#undef UNION_ENTRY
	} data;
} AST;

#define AST_STRUCT_SIZE sizeof(struct AST)

/**
 * Creates a new AST struct.
 *
 * @param IDENTIFIER The AST node identifier.
 * @param ... The AST node fields.
 *
 * @return The created AST struct.
 */
// NOLINTBEGIN(readability-avoid-const-params-in-decls)
struct AST* ast_new(const int IDENTIFIER, ...);
// NOLINTEND(readability-avoid-const-params-in-decls)

/**
 * Frees an AST struct.
 *
 * @param p_self The current AST struct.
 */
void ast_free(struct AST** p_self);

// Define new functions dynamically
#define AST_NODE_NEW_FUNCTION(name, ast, ...) ast##_NEW(name, __VA_ARGS__)

#define AST_NODE_ASSIGNMENT_NEW(name, fields)                                                      \
	void ast##name##_new(struct AST*(parent), struct AST_VARIABLE*(identifier),                    \
						 struct AST*(value)) {                                                     \
		struct AST_##name* lp_self = malloc(sizeof(struct AST_##name));                            \
		if (!lp_self) {                                                                            \
			PANIC("failed to malloc AST_" #name " struct");                                        \
		}                                                                                          \
		lp_self->identifier = identifier;                                                          \
		lp_self->value		= value;                                                               \
		(parent)->data.name = lp_self;                                                             \
	}

#define AST_NODE_BASIC_NEW(name, fields)                                                           \
	void ast##name##_new(struct AST*(parent)) {                                                    \
		struct AST_##name* lp_self = malloc(sizeof(struct AST_##name));                            \
		if (!lp_self) {                                                                            \
			PANIC("failed to malloc AST_" #name " struct");                                        \
		}                                                                                          \
		(parent)->data.name = lp_self;                                                             \
	}

#define AST_NODE_WITH_VALUE_NEW(name, fields)                                                      \
	void ast##name##_new(struct AST*(parent), struct String*(value)) {                             \
		struct AST_##name* lp_self = malloc(sizeof(struct AST_##name));                            \
		if (!lp_self) {                                                                            \
			PANIC("failed to malloc AST_" #name " struct");                                        \
		}                                                                                          \
		lp_self->value		= value;                                                               \
		(parent)->data.name = lp_self;                                                             \
	}

#define AST_NODE_FUNCTION_DEFINITION_NEW(name, fields)                                             \
	void ast##name##_new(struct AST*(ast), struct AST_VARIABLE*(identifier),                       \
						 struct AST_OPEN_BRACE*(open_brace), struct Array*(arguments),             \
						 struct Array*(argument_types), struct AST_CLOSE_BRACE*(close_brace)) {    \
		struct AST_##name* lp_self = malloc(sizeof(struct AST_##name));                            \
		if (!lp_self) {                                                                            \
			PANIC("failed to malloc AST_" #name " struct");                                        \
		}                                                                                          \
		lp_self->identifier		= identifier;                                                      \
		lp_self->open_brace		= open_brace;                                                      \
		lp_self->arguments		= arguments;                                                       \
		lp_self->argument_types = argument_types;                                                  \
		lp_self->close_brace	= close_brace;                                                     \
		(ast)->data.name		= lp_self;                                                         \
	}

// Create new functions for all nodes
AST_NODES(AST_NODE_NEW_FUNCTION) // NOLINT(bugprone-easily-swappable-parameters)

// Define AST new function list
static void (*const g_AST_NODE_NEW_FUNCTIONS[])(void*) = {
#define ENUM_ENTRY(name, new, ...) (void (*)(void*)) ast##name##_new,
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wcast-function-type"
#pragma clang diagnostic ignored "-Wcast-function-type-strict"
	AST_NODES(ENUM_ENTRY)
#pragma clang diagnostic pop
#undef ENUM_ENTRY
};

// Define free functions dynamically
#define AST_NODE_FREE_FUNCTION(name, free, ...) free##_FREE(name, __VA_ARGS__)

#define AST_NODE_ASSIGNMENT_FREE(name, fields)                                                     \
	void ast##name##_free(struct AST_##name** self) {                                              \
		if (self && *self) {                                                                       \
			lexer_token_free((struct LexerToken**)&(*self)->_token);                               \
			astVARIABLE_free((struct AST_VARIABLE**)&(*self)->identifier);                         \
			ast_free(&(*self)->value);                                                             \
			free(*self);                                                                           \
			*self = NULL;                                                                          \
		} else {                                                                                   \
			PANIC("AST_" #name " struct has already been freed");                                  \
		}                                                                                          \
	}

#define AST_NODE_BASIC_FREE(name, fields)                                                          \
	void ast##name##_free(struct AST_##name** self) {                                              \
		if (self && *self) {                                                                       \
			lexer_token_free((struct LexerToken**)&(*self)->_token);                               \
			free(*self);                                                                           \
			*self = NULL;                                                                          \
		} else {                                                                                   \
			PANIC("AST_" #name " struct has already been freed");                                  \
		}                                                                                          \
	}

#define AST_NODE_WITH_VALUE_FREE(name, fields)                                                     \
	void ast##name##_free(struct AST_##name** self) {                                              \
		if (self && *self) {                                                                       \
			lexer_token_free((struct LexerToken**)&(*self)->_token);                               \
			string_free(&(*self)->value);                                                          \
			free(*self);                                                                           \
			*self = NULL;                                                                          \
		} else {                                                                                   \
			PANIC("AST_" #name " struct has already been freed");                                  \
		}                                                                                          \
	}

#define AST_NODE_FUNCTION_DEFINITION_FREE(name, fields)                                            \
	void ast##name##_free(struct AST_##name** p_self) {                                            \
		if (p_self && *p_self) {                                                                   \
			lexer_token_free((struct LexerToken**)&(*p_self)->_token);                             \
			astOPEN_BRACE_free((struct AST_OPEN_BRACE**)&(*p_self)->open_brace);                   \
			astCLOSE_BRACE_free((struct AST_CLOSE_BRACE**)&(*p_self)->close_brace);                \
			array_free((struct Array**)&(*p_self)->arguments);                                     \
			array_free((struct Array**)&(*p_self)->argument_types);                                \
			free(*p_self);                                                                         \
			*p_self = NULL;                                                                        \
		} else {                                                                                   \
			PANIC("AST_FUNCTION_DEFINITION struct has already been freed");                        \
		}                                                                                          \
	}

// Create free functions for all nodes
AST_NODES(AST_NODE_FREE_FUNCTION)

// Define AST free function list
static void (*const g_AST_NODE_FREE_FUNCTIONS[])(void*) = {
#define ENUM_ENTRY(name, free, ...) (void (*)(void*)) ast##name##_free,
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wcast-function-type-strict"
	AST_NODES(ENUM_ENTRY)
#pragma clang diagnostic pop
#undef ENUM_ENTRY
};
