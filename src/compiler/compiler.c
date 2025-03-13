/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#include "./compiler.h"
#include "../parser/parser.h"
#include "../parser/tokens.h"
#include "../utils/panic.h"
#include <stdio.h>
#include <stdlib.h>

struct Compiler* compiler_new(const char* p_filePath) {
	struct Compiler* lp_compiler = malloc(COMPILER_STRUCT_SIZE);

	if (!lp_compiler) {
		PANIC("failed to malloc Compiler struct");
	}

	lp_compiler->parser = parser_new(p_filePath);

	return lp_compiler;
}

void compiler_free(struct Compiler** p_self) {
	if (p_self && *p_self) {
		parser_free(&(*p_self)->parser);

		free(*p_self);
		*p_self = NULL;
	} else {
		PANIC("Compiler struct has already been freed");
	}
}

void compiler_compile_next(struct Compiler* p_self) {
	switch (p_self->parser->AST->identifier) {
	default:
		printf("unsupported parser token for compiler: %s\n",
			   ast_tokens_get_name(p_self->parser->AST->identifier)); // TODO: fix
		break;
	}
}

bool compiler_compile(struct Compiler* p_self) {
	if (!parser_parse(p_self->parser, true, true)) {
		return false;
	}

	compiler_compile_next(p_self);

	return true;
}
