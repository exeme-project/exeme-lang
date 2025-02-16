/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include "../lexer/lexer.h"
#include <stdarg.h>

// X-Macro to define AST tokens
#define AST_TOKENS(X)                                                                              \
	X(CHR, AST_TOKEN_WITH_VALUE, const struct LexerToken* TOKEN; struct String * value)            \
	X(STRING, AST_TOKEN_WITH_VALUE, const struct LexerToken* TOKEN; struct String * value)         \
	X(INTEGER, AST_TOKEN_WITH_VALUE, const struct LexerToken* TOKEN; struct String * value)        \
	X(FLOAT, AST_TOKEN_WITH_VALUE, const struct LexerToken* TOKEN; struct String * value)          \
	X(VARIABLE, AST_TOKEN_WITH_VALUE, const struct LexerToken* TOKEN; struct String * value)       \
	X(ASSIGNMENT, AST_TOKEN_ASSIGNMENT, const struct LexerToken* TOKEN;                            \
	  struct AST_VARIABLE * identifier; struct AST * value)                                        \
	X(MODULO_ASSIGNMENT, AST_TOKEN_ASSIGNMENT, const struct LexerToken* TOKEN;                     \
	  struct AST_VARIABLE * identifier; struct AST * value)                                        \
	X(MULTIPLICATION_ASSIGNMENT, AST_TOKEN_ASSIGNMENT, const struct LexerToken* TOKEN;             \
	  struct AST_VARIABLE * identifier; struct AST * value)                                        \
	X(EXPONENT_ASSIGNMENT, AST_TOKEN_ASSIGNMENT, const struct LexerToken* TOKEN;                   \
	  struct AST_VARIABLE * identifier; struct AST * value)                                        \
	X(DIVISION_ASSIGNMENT, AST_TOKEN_ASSIGNMENT, const struct LexerToken* TOKEN;                   \
	  struct AST_VARIABLE * identifier; struct AST * value)                                        \
	X(FLOOR_DIVISION_ASSIGNMENT, AST_TOKEN_ASSIGNMENT, const struct LexerToken* TOKEN;             \
	  struct AST_VARIABLE * identifier; struct AST * value)                                        \
	X(ADDITION_ASSIGNMENT, AST_TOKEN_ASSIGNMENT, const struct LexerToken* TOKEN;                   \
	  struct AST_VARIABLE * identifier; struct AST * value)                                        \
	X(SUBTRACTION_ASSIGNMENT, AST_TOKEN_ASSIGNMENT, const struct LexerToken* TOKEN;                \
	  struct AST_VARIABLE * identifier; struct AST * value)                                        \
	X(BITWISE_AND_ASSIGNMENT, AST_TOKEN_ASSIGNMENT, const struct LexerToken* TOKEN;                \
	  struct AST_VARIABLE * identifier; struct AST * value)                                        \
	X(BITWISE_OR_ASSIGNMENT, AST_TOKEN_ASSIGNMENT, const struct LexerToken* TOKEN;                 \
	  struct AST_VARIABLE * identifier; struct AST * value)                                        \
	X(BITWISE_XOR_ASSIGNMENT, AST_TOKEN_ASSIGNMENT, const struct LexerToken* TOKEN;                \
	  struct AST_VARIABLE * identifier; struct AST * value)                                        \
	X(BITWISE_NOT_ASSIGNMENT, AST_TOKEN_ASSIGNMENT, const struct LexerToken* TOKEN;                \
	  struct AST_VARIABLE * identifier; struct AST * value)                                        \
	X(BITWISE_LEFT_SHIFT_ASSIGNMENT, AST_TOKEN_ASSIGNMENT, const struct LexerToken* TOKEN;         \
	  struct AST_VARIABLE * identifier; struct AST * value)                                        \
	X(BITWISE_RIGHT_SHIFT_ASSIGNMENT, AST_TOKEN_ASSIGNMENT, const struct LexerToken* TOKEN;        \
	  struct AST_VARIABLE * identifier; struct AST * value)                                        \
	X(OPEN_BRACE, AST_TOKEN_BASIC, const struct LexerToken* TOKEN;)                                \
	X(CLOSE_BRACE, AST_TOKEN_BASIC, const struct LexerToken* TOKEN;)                               \
	X(COMMA, AST_TOKEN_BASIC, const struct LexerToken* TOKEN;)                                     \
	X(COLON, AST_TOKEN_BASIC, const struct LexerToken* TOKEN;)                                     \
	X(FUNCTION_DEFINITION, AST_TOKEN_FUNCTION_DEFINITION, const struct LexerToken* TOKEN;          \
	  struct AST_VARIABLE * identifier; struct AST_OPEN_BRACE * open_brace;                        \
	  struct Array * arguments; struct Array * argument_types;                                     \
	  struct AST_CLOSE_BRACE * close_brace)

// X-Macro to define AST token identifiers
typedef enum {
#define AST_TOKEN_ENUM_ENTRY(name, ...) ASTTOKENS_##name,
	AST_TOKENS(AST_TOKEN_ENUM_ENTRY)
#undef AST_TOKEN_ENUM_ENTRY
} ASTTokenIdentifiers;

// X-Macro to define AST token names
static char* const g_ASTTOKEN_NAMES_INTERNAL[] = {
#define AST_TOKEN_TO_STRING(name, ...) #name,
	AST_TOKENS(AST_TOKEN_TO_STRING)
#undef AST_TOKEN_TO_STRING
};

extern const struct Array g_ASTTOKEN_NAMES;

/**
 * Gets the name of an AST token identifier.
 *
 * @param IDENTIFIER The AST token identifier.
 *
 * @return The name of the AST token identifier.
 */
const char* ast_tokens_get_name(
	const ASTTokenIdentifiers IDENTIFIER); // NOLINT(readability-avoid-const-params-in-decls)

// X-Macro to define AST tokens structs
#define AST_TOKEN_STRUCT_DEF(name, _, ...)                                                         \
	typedef struct AST_##name {                                                                    \
		__VA_ARGS__;                                                                               \
	} AST_##name;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wextra-semi"
AST_TOKENS(AST_TOKEN_STRUCT_DEF) // Using __VA_OPT__(;) causes compilation errors? This is a
								 // workaround. No idea why though. Linting is fine with it.
#pragma clang diagnostic pop
#undef AST_TOKEN_STRUCT_DEF

// Represents an AST node
typedef struct AST {
	ASTTokenIdentifiers identifier;
	void (*free)(struct AST**); // Free function pointer
	union {
// X-Macro to define AST tokens in the AST struct
#define AST_TOKEN_STRUCT_ENTRY(name, ...) struct AST_##name* name;
		AST_TOKENS(AST_TOKEN_STRUCT_ENTRY)
#undef AST_TOKEN_STRUCT_ENTRY
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

// Dynamically defines the new function for the token
#define AST_TOKEN_NEW_FUNCTION_DEFINE(name, ast, ...) ast##_NEW_DEFINE(name, __VA_ARGS__)

#define AST_TOKEN_ASSIGNMENT_NEW_DEFINE(name, ...)                                                 \
	void ast##name##_new(struct AST*(p_parent), va_list p_args);

#define AST_TOKEN_BASIC_NEW_DEFINE(name, ...)                                                      \
	void ast##name##_new(struct AST*(p_parent), va_list p_args);

#define AST_TOKEN_FUNCTION_DEFINITION_NEW_DEFINE(name, ...)                                        \
	void ast##name##_new(struct AST*(p_ast), va_list p_args);

#define AST_TOKEN_WITH_VALUE_NEW_DEFINE(name, ...)                                                 \
	void ast##name##_new(struct AST*(p_parent), va_list p_args);

AST_TOKENS(AST_TOKEN_NEW_FUNCTION_DEFINE) // Define all token new functions

// Dynamically generates the new function for the token. This will be called in the implementation
// file
#define AST_TOKEN_NEW_FUNCTION_IMPLEMENT(name, ast, ...) ast##_NEW_IMPLEMENT(name, __VA_ARGS__)

#define AST_TOKEN_ASSIGNMENT_NEW_IMPLEMENT(name, ...)                                              \
	void ast##name##_new(struct AST*(p_parent), va_list p_args) {                                  \
		struct AST_##name* lp_self = malloc(sizeof(struct AST_##name));                            \
		if (!lp_self) {                                                                            \
			PANIC("failed to malloc AST_" #name " struct");                                        \
		}                                                                                          \
		lp_self->TOKEN		  = va_arg(p_args, const struct LexerToken*);                          \
		lp_self->identifier	  = va_arg(p_args, struct AST_VARIABLE*);                              \
		lp_self->value		  = va_arg(p_args, struct AST*);                                       \
		(p_parent)->data.name = lp_self;                                                           \
	}

#define AST_TOKEN_BASIC_NEW_IMPLEMENT(name, ...)                                                   \
	void ast##name##_new(struct AST*(p_parent), va_list p_args) {                                  \
		struct AST_##name* lp_self = malloc(sizeof(struct AST_##name));                            \
		if (!lp_self) {                                                                            \
			PANIC("failed to malloc AST_" #name " struct");                                        \
		}                                                                                          \
		lp_self->TOKEN		  = va_arg(p_args, const struct LexerToken*);                          \
		(p_parent)->data.name = lp_self;                                                           \
	}

#define AST_TOKEN_FUNCTION_DEFINITION_NEW_IMPLEMENT(name, ...)                                     \
	void ast##name##_new(struct AST*(p_ast), va_list p_args) {                                     \
		struct AST_##name* lp_self = malloc(sizeof(struct AST_##name));                            \
		if (!lp_self) {                                                                            \
			PANIC("failed to malloc AST_" #name " struct");                                        \
		}                                                                                          \
		lp_self->TOKEN			= va_arg(p_args, const struct LexerToken*);                        \
		lp_self->identifier		= va_arg(p_args, struct AST_VARIABLE*);                            \
		lp_self->open_brace		= va_arg(p_args, struct AST_OPEN_BRACE*);                          \
		lp_self->arguments		= va_arg(p_args, struct Array*);                                   \
		lp_self->argument_types = va_arg(p_args, struct Array*);                                   \
		lp_self->close_brace	= va_arg(p_args, struct AST_CLOSE_BRACE*);                         \
		(p_ast)->data.name		= lp_self;                                                         \
	}

#define AST_TOKEN_WITH_VALUE_NEW_IMPLEMENT(name, ...)                                              \
	void ast##name##_new(struct AST*(p_parent), va_list p_args) {                                  \
		struct AST_##name* lp_self = malloc(sizeof(struct AST_##name));                            \
		if (!lp_self) {                                                                            \
			PANIC("failed to malloc AST_" #name " struct");                                        \
		}                                                                                          \
		lp_self->TOKEN		  = va_arg(p_args, const struct LexerToken*);                          \
		lp_self->value		  = va_arg(p_args, struct String*);                                    \
		(p_parent)->data.name = lp_self;                                                           \
	}

// X-Macro to define AST new function list
static void (*const g_AST_TOKEN_NEW_FUNCTIONS[])(void*) = {
#define AST_TOKEN_NEW_FUNC_TO_STRING(name, ...) (void (*)(void*)) ast##name##_new,
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wcast-function-type"
#pragma clang diagnostic ignored "-Wcast-function-type-strict"
	AST_TOKENS(AST_TOKEN_NEW_FUNC_TO_STRING)
#pragma clang diagnostic pop
#undef AST_TOKEN_NEW_FUNC_TO_STRING
};

// Dynamically defines the specific free function for the token
#define AST_TOKEN_FREE_SPECIFIC_FUNCTION_DEFINE(name, free, ...)                                   \
	free##_FREE_SPECIFIC_DEFINE(name, __VA_ARGS__)

#define AST_TOKEN_ASSIGNMENT_FREE_SPECIFIC_DEFINE(name, ...)                                       \
	void ast##name##_specific_free(struct AST_##name** p_self);

#define AST_TOKEN_BASIC_FREE_SPECIFIC_DEFINE(name, ...)                                            \
	void ast##name##_specific_free(struct AST_##name** p_self);

#define AST_TOKEN_FUNCTION_DEFINITION_FREE_SPECIFIC_DEFINE(name, ...)                              \
	void ast##name##_specific_free(struct AST_##name** p_self);

#define AST_TOKEN_WITH_VALUE_FREE_SPECIFIC_DEFINE(name, ...)                                       \
	void ast##name##_specific_free(struct AST_##name** p_self);

AST_TOKENS(AST_TOKEN_FREE_SPECIFIC_FUNCTION_DEFINE) // Define all specific token free functions

// Dynamically generates the specific free function for the token.  This will be called in the
// implementation file
#define AST_TOKEN_FREE_SPECIFIC_FUNCTION_IMPLEMENT(name, free, ...)                                \
	free##_FREE_SPECIFIC_IMPLEMENT(name, __VA_ARGS__)

#define AST_TOKEN_ASSIGNMENT_FREE_SPECIFIC_IMPLEMENT(name, ...)                                    \
	void ast##name##_specific_free(struct AST_##name** p_self) {                                   \
		if (p_self && *p_self) {                                                                   \
			lexer_token_free((struct LexerToken**)&(*p_self)->TOKEN);                              \
			astVARIABLE_specific_free((struct AST_VARIABLE**)&(*p_self)->identifier);              \
			ast_free(&(*p_self)->value);                                                           \
			free(*p_self);                                                                         \
			*p_self = NULL;                                                                        \
		} else {                                                                                   \
			PANIC("AST_" #name " struct has already been freed");                                  \
		}                                                                                          \
	}

#define AST_TOKEN_BASIC_FREE_SPECIFIC_IMPLEMENT(name, ...)                                         \
	void ast##name##_specific_free(struct AST_##name** p_self) {                                   \
		if (p_self && *p_self) {                                                                   \
			lexer_token_free((struct LexerToken**)&(*p_self)->TOKEN);                              \
			free(*p_self);                                                                         \
			*p_self = NULL;                                                                        \
		} else {                                                                                   \
			PANIC("AST_" #name " struct has already been freed");                                  \
		}                                                                                          \
	}

#define AST_TOKEN_FUNCTION_DEFINITION_FREE_SPECIFIC_IMPLEMENT(name, ...)                           \
	void ast##name##_specific_free(struct AST_##name** p_self) {                                   \
		if (p_self && *p_self) {                                                                   \
			lexer_token_free((struct LexerToken**)&(*p_self)->TOKEN);                              \
			astOPEN_BRACE_specific_free((struct AST_OPEN_BRACE**)&(*p_self)->open_brace);          \
			astCLOSE_BRACE_specific_free((struct AST_CLOSE_BRACE**)&(*p_self)->close_brace);       \
			array_free((struct Array**)&(*p_self)->arguments);                                     \
			array_free((struct Array**)&(*p_self)->argument_types);                                \
			free(*p_self);                                                                         \
			*p_self = NULL;                                                                        \
		} else {                                                                                   \
			PANIC("AST_FUNCTION_DEFINITION struct has already been freed");                        \
		}                                                                                          \
	}

#define AST_TOKEN_WITH_VALUE_FREE_SPECIFIC_IMPLEMENT(name, ...)                                    \
	void ast##name##_specific_free(struct AST_##name** p_self) {                                   \
		if (p_self && *p_self) {                                                                   \
			lexer_token_free((struct LexerToken**)&(*p_self)->TOKEN);                              \
			string_free(&(*p_self)->value);                                                        \
			free(*p_self);                                                                         \
			*p_self = NULL;                                                                        \
		} else {                                                                                   \
			PANIC("AST_" #name " struct has already been freed");                                  \
		}                                                                                          \
	}

// Dynamically defines the generic free function for the token
#define AST_TOKEN_FREE_GENERIC_FUNCTION_DEFINE(name, free, ...)                                    \
	free##_FREE_GENERIC_DEFINE(name, __VA_ARGS__)

#define AST_TOKEN_ASSIGNMENT_FREE_GENERIC_DEFINE(name, ...)                                        \
	void ast##name##_generic_free(struct AST** p_self);

#define AST_TOKEN_BASIC_FREE_GENERIC_DEFINE(name, ...)                                             \
	void ast##name##_generic_free(struct AST** p_self);

#define AST_TOKEN_FUNCTION_DEFINITION_FREE_GENERIC_DEFINE(name, ...)                               \
	void ast##name##_generic_free(struct AST** p_self);

#define AST_TOKEN_WITH_VALUE_FREE_GENERIC_DEFINE(name, ...)                                        \
	void ast##name##_generic_free(struct AST** p_self);

AST_TOKENS(AST_TOKEN_FREE_GENERIC_FUNCTION_DEFINE) // Define all generic token free functions

// X-Macro to define AST generic free function list
static void (*const g_AST_TOKEN_FREE_GENERIC_FUNCTIONS[])(void*) = {
#define AST_TOKEN_FREE_GENERIC_FUNC_TO_STRING(name, ...) (void (*)(void*)) ast##name##_generic_free,
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wcast-function-type-strict"
	AST_TOKENS(AST_TOKEN_FREE_GENERIC_FUNC_TO_STRING)
#pragma clang diagnostic pop
#undef AST_TOKEN_FREE_GENERIC_FUNC_TO_STRING
};

// Dynamically generates the generic free function for the token. This will be called in the
// implementation file
#define AST_TOKEN_FREE_GENERIC_FUNCTION_IMPLEMENT(name, free, ...)                                 \
	free##_FREE_GENERIC_IMPLEMENT(name, __VA_ARGS__)

#define AST_TOKEN_ASSIGNMENT_FREE_GENERIC_IMPLEMENT(name, ...)                                     \
	void ast##name##_generic_free(struct AST** p_self) {                                           \
		if (p_self && *p_self) {                                                                   \
			ast##name##_specific_free((struct AST_##name**)&(*p_self)->data.name);                 \
		} else {                                                                                   \
			PANIC("AST_" #name " struct has already been freed");                                  \
		}                                                                                          \
	}

#define AST_TOKEN_BASIC_FREE_GENERIC_IMPLEMENT(name, ...)                                          \
	void ast##name##_generic_free(struct AST** p_self) {                                           \
		if (p_self && *p_self) {                                                                   \
			ast##name##_specific_free((struct AST_##name**)&(*p_self)->data.name);                 \
		} else {                                                                                   \
			PANIC("AST_" #name " struct has already been freed");                                  \
		}                                                                                          \
	}

#define AST_TOKEN_FUNCTION_DEFINITION_FREE_GENERIC_IMPLEMENT(name, ...)                            \
	void ast##name##_generic_free(struct AST** p_self) {                                           \
		if (p_self && *p_self) {                                                                   \
			ast##name##_specific_free((struct AST_##name**)&(*p_self)->data.name);                 \
		} else {                                                                                   \
			PANIC("AST_FUNCTION_DEFINITION struct has already been freed");                        \
		}                                                                                          \
	}

#define AST_TOKEN_WITH_VALUE_FREE_GENERIC_IMPLEMENT(name, ...)                                     \
	void ast##name##_generic_free(struct AST** p_self) {                                           \
		if (p_self && *p_self) {                                                                   \
			ast##name##_specific_free((struct AST_##name**)&(*p_self)->data.name);                 \
		} else {                                                                                   \
			PANIC("AST_" #name " struct has already been freed");                                  \
		}                                                                                          \
	}
