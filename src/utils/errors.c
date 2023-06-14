/**
 * Part of the Exeme Language Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include "../includes.c"

#include "./array.c"

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
};

/**
 * Contains the names of each of the error identifiers.
 */
const struct Array ERRORIDENTIFIER_NAMES = {
	7,
	(const void *[]){
		// Lexer
		"L0001",
		"L0002",
		"L0003",
		"L0004",
		"L0005",
		"L0006",
		"L0007",
	},
};
