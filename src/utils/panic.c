/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#include "../globals.h"
#include <stdio.h>
#include <stdlib.h>

__attribute__((noreturn)) void panic_internal(const char* p_panicMsg, const char* p_file,
											  const int LINE, const char* p_function) {
	printf("%s%spanic%s(line=%s%d%s, function=%s%s%s, file=%s%s%s): %s\n", gp_F_BRIGHT_RED,
		   gp_S_BOLD, gp_S_RESET, gp_S_BOLD, LINE, gp_S_RESET, gp_S_ITALIC, p_function, gp_S_RESET,
		   gp_S_UNDERLINE, p_file, gp_S_RESET, p_panicMsg);

	exit(EXIT_FAILURE); // NOLINT(concurrency-mt-unsafe)
}

__attribute__((noreturn)) void error(const char* p_errorMsg) {
	printf("%s%serror: %s%s\n", gp_F_BRIGHT_RED, gp_S_BOLD, gp_S_RESET, p_errorMsg);

	exit(EXIT_FAILURE); // NOLINT(concurrency-mt-unsafe)
}
