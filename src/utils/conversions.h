/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include <stddef.h>

/**
 * Represents the types of variables.
 */
enum VariableType {
	VARIABLE_TYPE_NONE,
	VARIABLE_TYPE_STRING,
	VARIABLE_TYPE_INT,
	VARIABLE_TYPE_FLOAT,
	VARIABLE_TYPE_BOOL
};

/**
 * Converts the char into a string.
 *
 * @param chr The char to convert into a string.
 *
 * @return The repeated string.
 */
char* chr_to_string(char chr);

/**
 * Converts the unsigned long into a string.
 *
 * @param num The unsigned long to convert into a string.
 *
 * @return The converted string.
 */
char* ul_to_string(size_t num);

/**
 * Converts the data contained in a string into its corresponding type.
 *
 * @param p_data The data to convert.
 * @param type The type to convert to.
 *
 * @return The converted data.
 */
void* convert_to_type(char* p_data, enum VariableType type);
