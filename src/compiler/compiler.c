/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include "../includes.c"

#include "../parser/parser.c"
#include "./tokens.c"

struct Compiler {
	struct Parser *parser;
};

#define COMPILER_STRUCT_SIZE sizeof(struct Compiler)

/**
 * Creates a new Compiler struct.
 *
 * @param FILE_PATH The path to the file to compile.
 *
 * @return The created Compiler struct.
 */
struct Compiler *compiler_new(const char *FILE_PATH) {
	struct Compiler *compiler = malloc(COMPILER_STRUCT_SIZE);

	compiler->parser = parser_new(FILE_PATH);

	return compiler;
}

/**
 * Frees the Compiler struct.
 *
 * @param self The current Compiler struct.
 */
void compiler_free(struct Compiler **self) {
	if (self && *self) {
		parser_free(&(*self)->parser);

		free(*self);
		*self = NULL;
	} else {
		panic("Compiler struct has already been freed");
	}
}

/**
 * Calls the correct function for compiling the current parser token.
 *
 * @param self The current Compiler struct.
 */
void compiler_compileNext(struct Compiler *self) {
	switch (self->parser->AST->IDENTIFIER) {}
}

/**
 * Gets the next parser token and compiles it.
 *
 * @param self The current Compiler struct.
 *
 * @return bool Whether compiling succeeded.
 */
bool compiler_compile(struct Compiler *self) {
	if (!parser_parse(self->parser, true)) {
		return false;
	}

	compiler_compileNext(self);

	return true;
}
