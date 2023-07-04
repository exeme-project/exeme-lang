/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include "./includes.c"

#include "./utils/array.c"
#include "./utils/panic.c"

/**
 * Used to identify errors.
 */
enum ErrorIdentifiers {
	// Lexer
	L0001,
	L0002,
	L0003,
	L0004,
	L0005,
	L0006,
	L0007,

	// Parser
	P0001,
	P0002,
};

/**
 * Contains the names of each of the error identifiers.
 */
const struct Array ERRORIDENTIFIER_NAMES = {
	9,
	(const void *[]){
		// Lexer
		"L0001",
		"L0002",
		"L0003",
		"L0004",
		"L0005",
		"L0006",
		"L0007",

		// Parser
		"P0001",
		"P0002",
	},
};

/**
 * Gets the name of an error.
 *
 * @param IDENTIFIER The error's identifier.
 *
 * @return The name of the error.
 */
const char *error_get(const enum ErrorIdentifiers IDENTIFIER) {
	if ((size_t)IDENTIFIER + 1 > ERRORIDENTIFIER_NAMES.length) {
		panic("error get index out of bounds");
	}

	return ERRORIDENTIFIER_NAMES._values[IDENTIFIER];
}
