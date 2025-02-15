/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include "./utils/array.h"
#include <stddef.h>

#define _CRT_SECURE_NO_WARNINGS // What is deprecated on Windows isn't always on other OSes.

#define DENARY_BASE		  10
#define MAX_STRING_LENGTH 1024U

#define LONG_SIZE	sizeof(long)
#define FLOAT_SIZE	sizeof(float)
#define STRING_SIZE sizeof(char*)

extern const char* const gp_F_BLACK;
extern const char* const gp_F_RED;
extern const char* const gp_F_GREEN;
extern const char* const gp_F_YELLOW;
extern const char* const gp_F_BLUE;
extern const char* const gp_F_MAGENTA;
extern const char* const gp_F_CYAN;
extern const char* const gp_F_WHITE;

extern const char* const gp_F_BRIGHT_BLACK;
extern const char* const gp_F_BRIGHT_RED;
extern const char* const gp_F_BRIGHT_GREEN;
extern const char* const gp_F_BRIGHT_YELLOW;
extern const char* const gp_F_BRIGHT_BLUE;
extern const char* const gp_F_BRIGHT_MAGENTA;
extern const char* const gp_F_BRIGHT_CYAN;
extern const char* const gp_F_BRIGHT_WHITE;

extern const char* const gp_B_BLACK;
extern const char* const gp_B_RED;
extern const char* const gp_B_GREEN;
extern const char* const gp_B_YELLOW;
extern const char* const gp_B_BLUE;
extern const char* const gp_B_MAGENTA;
extern const char* const gp_B_CYAN;
extern const char* const gp_B_WHITE;

extern const char* const gp_B_BRIGHT_BLACK;
extern const char* const gp_B_BRIGHT_RED;
extern const char* const gp_B_BRIGHT_GREEN;
extern const char* const gp_B_BRIGHT_YELLOW;
extern const char* const gp_B_BRIGHT_BLUE;
extern const char* const gp_B_BRIGHT_MAGENTA;
extern const char* const gp_B_BRIGHT_CYAN;
extern const char* const gp_B_BRIGHT_WHITE;

extern const char* const gp_S_RESET;
extern const char* const gp_S_BOLD;
extern const char* const gp_S_DIM;
extern const char* const gp_S_ITALIC;
extern const char* const gp_S_UNDERLINE;
extern const char* const gp_S_BLINKING;
extern const char* const gp_S_REVERSE;
extern const char* const gp_S_INVISIBLE;
extern const char* const gp_S_STRIKE_THROUGH;

extern const size_t g_NEGATIVE_ULL;

/**
 * Used to identify variable types.
 */
enum VariableType {
	VARIABLE_TYPE_NONE,
	VARIABLE_TYPE_STRING,
	VARIABLE_TYPE_INT,
	VARIABLE_TYPE_FLOAT,
	VARIABLE_TYPE_BOOL,
};

/**
 * Contains the names of each of the variable types.
 */
extern const struct Array g_VARIABLETYPE_NAMES;

/**
 * Gets the name of a variable type.
 *
 * @param TYPE The variable type.
 *
 * @return The name of the variable type.
 */
const char*
variable_type_get(const enum VariableType TYPE); // NOLINT(readability-avoid-const-params-in-decls)
