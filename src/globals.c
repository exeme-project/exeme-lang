/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include "./includes.c"
#include "./utils/array.c"

#define LONG_SIZE	sizeof(long)
#define FLOAT_SIZE	sizeof(float)
#define STRING_SIZE sizeof(char*)

const size_t negativeULL = (size_t)-1;

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
const struct Array VARIABLETYPE_NAMES = array_new_stack("none", "string", "int", "float", "bool");

/**
 * Gets the name of a variable type.
 *
 * @param TYPE The variable type.
 *
 * @return The name of the variable type.
 */
const char* variableType_get(const enum VariableType TYPE) {
	if ((size_t)TYPE + 1 > VARIABLETYPE_NAMES.length) {
		panic("VariableType get index out of bounds");
	}

	return VARIABLETYPE_NAMES._values[TYPE];
}
