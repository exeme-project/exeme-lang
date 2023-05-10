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
	ERRORIDENTIFIERS_LEXER_1,
	ERRORIDENTIFIERS_LEXER_2,
	ERRORIDENTIFIERS_LEXER_3,
	ERRORIDENTIFIERS_LEXER_4,
	ERRORIDENTIFIERS_LEXER_5,
	ERRORIDENTIFIERS_LEXER_6,
	ERRORIDENTIFIERS_LEXER_7,

	// Parser
	ERRORIDENTIFIERS_PARSER_1,
	ERRORIDENTIFIERS_PARSER_2,
};

/**
 * Contains the names of each of the error identifiers.
 */
const struct Array ERRORIDENTIFIER_NAMES = {
	7,
	(const void *[]){
		// Lexer
		"L1",
		"L2",
		"L3",
		"L4",
		"L5",
		"L6",
		"L7",

		// Parser
		"P1",
		"P2",
	},
};
