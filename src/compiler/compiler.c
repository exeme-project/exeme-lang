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
