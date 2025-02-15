/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include <stdbool.h>

/**
 * Represents a compiler.
 */
struct Compiler {
	struct Parser* parser;
};

#define COMPILER_STRUCT_SIZE sizeof(struct Compiler)

/**
 * Creates a new Compiler struct.
 *
 * @param p_filePath The path to the file to compile.
 *
 * @return The created Compiler struct.
 */
struct Compiler* compiler_new(const char* p_filePath);

/**
 * Frees the Compiler struct.
 *
 * @param p_self The current Compiler struct.
 */
void compiler_free(struct Compiler** p_self);

/**
 * Calls the correct function for compiling the current parser token.
 *
 * @param p_self The current Compiler struct.
 */
void compiler_compile_next(struct Compiler* p_self);

/**
 * Gets the next parser token and compiles it.
 *
 * @param p_self The current Compiler struct.
 *
 * @return Whether compiling succeeded.
 */
bool compiler_compile(struct Compiler* p_self);
