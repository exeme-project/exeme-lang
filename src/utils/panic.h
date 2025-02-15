/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

/**
 * Prints a panic and exits. Used for when our code is failing.
 *
 * @param p_panicMsg The panic message.
 * @param p_file The file where the panic occurred.
 * @param LINE The line where the panic occurred.
 * @param p_function The function where the panic occurred.
 */
__attribute__((noreturn)) void
panic_internal(const char* p_panicMsg, const char* p_file,
			   const int   LINE, // NOLINT(readability-avoid-const-params-in-decls)
			   const char* p_function);

/**
 * Macro to call the _panic function with the current file, line, and function information.
 *
 * @param PANIC_MSG The panic message.
 */
#define PANIC(PANIC_MSG) panic_internal(PANIC_MSG, __FILE__, __LINE__, __func__)

/**
 * Prints an error and exits. Used for when something related to the user (and their input(s))
 * are failing.
 *
 * @param p_errorMsg The error message.
 */
__attribute__((noreturn)) void error(const char* p_errorMsg);
