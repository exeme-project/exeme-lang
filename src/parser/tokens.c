/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#include "./tokens.h"
#include "../lexer/tokens.h"
#include "../utils/array.h"
#include "../utils/panic.h"
#include <stdarg.h>

// Create a new function for each node
AST_TOKENS(AST_TOKEN_NEW_FUNCTION_IMPLEMENT) // NOLINT(bugprone-easily-swappable-parameters)

// Create a specific free function for each node
AST_TOKENS(AST_TOKEN_FREE_SPECIFIC_FUNCTION_IMPLEMENT)

// Create a generic free function for each node
AST_TOKENS(AST_TOKEN_FREE_GENERIC_FUNCTION_IMPLEMENT)

const struct Array g_ASTTOKEN_NAMES =
	ARRAY_UPGRADE_STACK((const void**)g_ASTTOKEN_NAMES_INTERNAL,
						sizeof(g_ASTTOKEN_NAMES_INTERNAL) / ARRAY_STRUCT_ELEMENT_SIZE);

const char* ast_tokens_get_name(const ASTTokenIdentifiers IDENTIFIER) {
	if (IDENTIFIER + 1 > g_ASTTOKEN_NAMES.length) {
		PANIC("g_ASTTOKEN_NAMES get index out of bounds");
	}

	return g_ASTTOKEN_NAMES._values[IDENTIFIER];
}

struct AST* ast_new(const int IDENTIFIER, ...) {
	struct AST* lp_self = malloc(AST_STRUCT_SIZE);

	if (!lp_self) {
		PANIC("failed to malloc AST struct");
	}

	lp_self->identifier = IDENTIFIER;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wincompatible-function-pointer-types"
#pragma clang diagnostic ignored "-Wincompatible-function-pointer-types-strict"
	void (*const lp_NEW_NODE)(struct AST*, va_list) = g_AST_TOKEN_NEW_FUNCTIONS[IDENTIFIER];
#pragma clang diagnostic pop

	// Pass all var args to the node's constructor, but not as var args
	va_list lpArgs;
	va_start(lpArgs, IDENTIFIER);
	lp_NEW_NODE(lp_self, lpArgs);
	va_end(lpArgs);

	return lp_self;
}

void ast_free(struct AST** p_self) {
	if (p_self && *p_self) {
		g_AST_TOKEN_FREE_GENERIC_FUNCTIONS[(*p_self)->identifier](p_self);

		free(*p_self);
		*p_self = NULL;
	} else {
		PANIC("AST struct has already been freed");
	}
}

void* ast_get_data(const struct AST* p_self) {
	switch (p_self->identifier) {
		// X-Macro to define AST token getters
#define AST_TOKEN_GETTER(name, ...)                                                                \
	case ASTTOKENS_##name: {                                                                       \
		return p_self->data.name;                                                                  \
	}
		AST_TOKENS(AST_TOKEN_GETTER)
#undef AST_TOKEN_GETTER
	default:
		PANIC("unsupported AST token identifier in ast_get_data");
	}
}

const struct LexerToken* ast_get_inner_lexer_token(const struct AST* p_self) {
	switch (p_self->identifier) {
		// X-Macro to define AST token inner lexer token getters
#define AST_TOKEN_INNER_LEXER_TOKEN_GETTER(name, ...)                                              \
	case ASTTOKENS_##name: {                                                                       \
		return ((struct AST_##name*)p_self)->TOKEN;                                                \
	}
		AST_TOKENS(AST_TOKEN_INNER_LEXER_TOKEN_GETTER)
#undef AST_TOKEN_INNER_LEXER_TOKEN_GETTER
	default:
		PANIC("unsupported AST token identifier in ast_get_inner_lexer_token");
	}
}
