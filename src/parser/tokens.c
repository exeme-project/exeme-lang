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
	lp_self->free = g_AST_TOKEN_FREE_GENERIC_FUNCTIONS[IDENTIFIER];
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
		(*p_self)->free(p_self);

		free(*p_self);
		*p_self = NULL;
	} else {
		PANIC("AST struct has already been freed");
	}
}
