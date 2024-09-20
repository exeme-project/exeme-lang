/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include "../includes.c"

#include "./console.c"

/**
 * Prints a panic and exits.
 *
 * @param PANIC_MSG The panic message.
 */
__attribute__((noreturn)) void _panic(const char *PANIC_MSG, const char *FILE, const int LINE, const char *FUNCTION) {
    printf("%s%spanic%s(line=%s%d%s, function=%s%s%s, file=%s%s%s): %s\n", F_BRIGHT_RED, S_BOLD, S_RESET, S_BOLD, LINE,
           S_RESET, S_ITALIC, FUNCTION, S_RESET, S_UNDERLINE, FILE, S_RESET, PANIC_MSG);
    exit(EXIT_FAILURE);
}

#define panic(PANIC_MSG) _panic(PANIC_MSG, __FILE__, __LINE__, __func__)
