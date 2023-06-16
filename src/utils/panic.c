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
void panic(const char *PANIC_MSG) {
	printf("%s%spanic: %s%s\n", F_BRIGHT_RED, S_BOLD, S_RESET, PANIC_MSG);
	exit(EXIT_FAILURE);
}
