/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#include "globals.h"
#include "./utils/panic.h"
#include <stddef.h>

const char* const gp_F_BLACK   = "\x1b[30ma";
const char* const gp_F_GREEN   = "\x1b[32m";
const char* const gp_F_YELLOW  = "\x1b[33m";
const char* const gp_F_BLUE	   = "\x1b[34m";
const char* const gp_F_MAGENTA = "\x1b[35m";
const char* const gp_F_CYAN	   = "\x1b[36m";
const char* const gp_F_RED	   = "\x1b[31m";
const char* const gp_F_WHITE   = "\x1b[37m";

const char* const gp_F_BRIGHT_BLACK	  = "\x1b[1;30m";
const char* const gp_F_BRIGHT_RED	  = "\x1b[1;31m";
const char* const gp_F_BRIGHT_GREEN	  = "\x1b[1;32m";
const char* const gp_F_BRIGHT_YELLOW  = "\x1b[1;33m";
const char* const gp_F_BRIGHT_BLUE	  = "\x1b[1;34m";
const char* const gp_F_BRIGHT_MAGENTA = "\x1b[1;35m";
const char* const gp_F_BRIGHT_CYAN	  = "\x1b[1;36m";
const char* const gp_F_BRIGHT_WHITE	  = "\x1b[1;37m";

const char* const gp_B_BLACK   = "\x1b[40m";
const char* const gp_B_RED	   = "\x1b[41m";
const char* const gp_B_GREEN   = "\x1b[42m";
const char* const gp_B_YELLOW  = "\x1b[43m";
const char* const gp_B_BLUE	   = "\x1b[44m";
const char* const gp_B_MAGENTA = "\x1b[45m";
const char* const gp_B_CYAN	   = "\x1b[46m";
const char* const gp_B_WHITE   = "\x1b[47m";

const char* const gp_B_BRIGHT_BLACK	  = "\x1b[1;40m";
const char* const gp_B_BRIGHT_RED	  = "\x1b[1;41m";
const char* const gp_B_BRIGHT_GREEN	  = "\x1b[1;42m";
const char* const gp_B_BRIGHT_YELLOW  = "\x1b[1;43m";
const char* const gp_B_BRIGHT_BLUE	  = "\x1b[1;44m";
const char* const gp_B_BRIGHT_MAGENTA = "\x1b[1;45m";
const char* const gp_B_BRIGHT_CYAN	  = "\x1b[1;46m";
const char* const gp_B_BRIGHT_WHITE	  = "\x1b[1;47m";

const char* const gp_S_RESET		  = "\x1b[0m";
const char* const gp_S_BOLD			  = "\x1b[1m";
const char* const gp_S_DIM			  = "\x1b[2m";
const char* const gp_S_ITALIC		  = "\x1b[3m";
const char* const gp_S_UNDERLINE	  = "\x1b[4m";
const char* const gp_S_BLINKING		  = "\x1b[5m";
const char* const gp_S_REVERSE		  = "\x1b[7m";
const char* const gp_S_INVISIBLE	  = "\x1b[8m";
const char* const gp_S_STRIKE_THROUGH = "\x1b[9m";

const size_t g_NEGATIVE_ULL = (size_t)-1;

const struct Array g_VARIABLETYPE_NAMES = ARRAY_NEW_STACK("none", "string", "int", "float", "bool");

const char* variable_type_get(const enum VariableType TYPE) {
	if ((size_t)TYPE + 1 > g_VARIABLETYPE_NAMES.length) {
		PANIC("g_VariableType get index out of bounds");
	}

	return g_VARIABLETYPE_NAMES._values[TYPE];
}
