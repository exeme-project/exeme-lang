/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include "./tokens.h"

const struct Array g_LEXERTOKEN_NAMES = ARRAY_NEW_STACK(
	"",

	"keyword", "identifier",

	"char", "string", "integer", "float",

	// Arithmetic operators
	"modulo operator", "multiplication operator", "exponent operator", "division operator",
	"floor division operator", "addition operator", "subtraction operator",

	// Comparison / Relational operators
	"equal to operator", "not equal to operator", "greater then operator", "less than operator",
	"greater than or equal operator", "less than or equal operator",

	// Logical operators
	"logical and operator", "logical or operator", "logical not operator",

	// Bitwise operators
	"bitwise AND operator", "bitwise OR operator", "bitwise XOR operator", "bitwise NOT operator",
	"bitwise left shift", "bitwise right shift",

	// Assignment operators
	"assignment operator",

	"modulo assignment operator", "multiplication assignment operator",
	"exponent assignment operator", "division assignment operator",
	"floor division assignment operator", "addition assignment operator",
	"subtraction assignment operator",

	"bitwise AND assignment operator", "bitwise OR assignment operator",
	"bitwise XOR assignment operator", "bitwise NOT assignment operator",
	"bitwise left shift assignment operator", "bitwise right shift assignment operator",

	// Member / Pointer operators
	"dot operator", "type arrow operator", "assignment arrow operator", "at operator",

	// Syntactic constructs
	"open brace", "open square brace", "open curly brace", "close brace", "close square brace",
	"close curly brace", "comma", "colon", "scope resolution operator",

	// Misc
	"single line comment", "multi line comment");

const char* lexer_tokens_get_name(const enum LexerTokenIdentifiers IDENTIFIER) {
	if ((size_t)IDENTIFIER + 1 > g_LEXERTOKEN_NAMES.length) {
		PANIC("g_LEXERTOKEN_NAMES get index out of bounds");
	}

	return g_LEXERTOKEN_NAMES._values[IDENTIFIER];
}

const struct Array g_LEXER_TOKEN_PRECEDENCES =
	ARRAY_NEW_STACK("a",

					"a", "a",

					"a", "a", "a", "a",

					// Arithmetic operators
					"d", "c", "d", "d", "d", "e", "e",

					// Comparison / Relational operators
					"g", "g", "f", "f", "f", "f",

					// Logical operators
					"h", "h", "h",

					// Bitwise operators
					"e", "e", "e", "e", "i", "i",

					// Assignment operators
					"i",

					"i", "i", "i", "i", "i", "i", "i",

					"i", "i", "i", "i", "i", "i",

					// Member / Pointer operators
					"b", "b", "b", "c",

					// Syntactic constructs
					"b", "b", "b", "b", "b", "b", "j", "b", "a",

					// Misc
					"a", "a");

// NOLINTBEGIN(bugprone-easily-swappable-parameters)
const struct LexerToken* lexer_token_new(enum LexerTokenIdentifiers identifier,
										 struct String* p_value, size_t startChrIndex,
										 size_t endChrIndex, size_t lineIndex) {
	// NOLINTEND(bugprone-easily-swappable-parameters)
	struct LexerToken* lp_self = malloc(LEXERTOKEN_STRUCT_SIZE);

	if (!lp_self) {
		PANIC("failed to malloc LexerToken struct");
	}

	lp_self->identifier	   = identifier;
	lp_self->value		   = p_value;
	lp_self->startChrIndex = startChrIndex;
	lp_self->endChrIndex   = endChrIndex;
	lp_self->lineIndex	   = lineIndex;

	return lp_self;
}

void lexer_token_free(struct LexerToken** p_self) {
	if (p_self && *p_self) {
		string_free((struct String**)&(*p_self)->value);

		free(*p_self);
		*p_self = NULL;
	} else {
		PANIC("LexerToken struct has already been freed");
	}
}
