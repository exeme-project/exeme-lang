/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include "../utils/array.h"
#include "../utils/panic.h"
#include "../utils/str.h"

/**
 * Used to identify different lexer tokens.
 */
enum LexerTokenIdentifiers {
	LEXERTOKENS_NONE,

	LEXERTOKENS_KEYWORD,
	LEXERTOKENS_IDENTIFIER,

	LEXERTOKENS_CHR,
	LEXERTOKENS_STRING,
	LEXERTOKENS_INTEGER,
	LEXERTOKENS_FLOAT,

	// Arithmetic operators
	LEXERTOKENS_MODULO,			// '%'
	LEXERTOKENS_MULTIPLICATION, // '*'
	LEXERTOKENS_EXPONENT,		// '**'
	LEXERTOKENS_DIVISION,		// '/'
	LEXERTOKENS_FLOOR_DIVISION, // '//'
	LEXERTOKENS_ADDITION,		// '+'
	LEXERTOKENS_SUBTRACTION,	// '-'

	// Comparison / Relational operators
	LEXERTOKENS_EQUAL_TO,			   // '=='
	LEXERTOKENS_NOT_EQUAL_TO,		   // '!='
	LEXERTOKENS_GREATER_THAN,		   // '>'
	LEXERTOKENS_LESS_THAN,			   // '<'
	LEXERTOKENS_GREATER_THAN_OR_EQUAL, // '>='
	LEXERTOKENS_LESS_THAN_OR_EQUAL,	   // '<='

	// Logical operators
	LEXERTOKENS_LOGICAL_AND, // '&&'
	LEXERTOKENS_LOGICAL_OR,	 // '||'
	LEXERTOKENS_LOGICAL_NOT, // '!'

	// Bitwise operators
	LEXERTOKENS_BITWISE_AND,		 // '&'
	LEXERTOKENS_BITWISE_OR,			 // '|'
	LEXERTOKENS_BITWISE_XOR,		 // '^'
	LEXERTOKENS_BITWISE_NOT,		 // '~'
	LEXERTOKENS_BITWISE_LEFT_SHIFT,	 // '<<'
	LEXERTOKENS_BITWISE_RIGHT_SHIFT, // '>>'

	// Assignment operators
	LEXERTOKENS_ASSIGNMENT, // '='

	LEXERTOKENS_MODULO_ASSIGNMENT,		   // '%='
	LEXERTOKENS_MULTIPLICATION_ASSIGNMENT, // '*='
	LEXERTOKENS_EXPONENT_ASSIGNMENT,	   // '**='
	LEXERTOKENS_DIVISION_ASSIGNMENT,	   // '/='
	LEXERTOKENS_FLOOR_DIVISION_ASSIGNMENT, // '//='
	LEXERTOKENS_ADDITION_ASSIGNMENT,	   // '+='
	LEXERTOKENS_SUBTRACTION_ASSIGNMENT,	   // '-='

	LEXERTOKENS_BITWISE_AND_ASSIGNMENT,			// '&='
	LEXERTOKENS_BITWISE_OR_ASSIGNMENT,			// '|='
	LEXERTOKENS_BITWISE_XOR_ASSIGNMENT,			// '^='
	LEXERTOKENS_BITWISE_NOT_ASSIGNMENT,			// '~='
	LEXERTOKENS_BITWISE_LEFT_SHIFT_ASSIGNMENT,	// '<<='
	LEXERTOKENS_BITWISE_RIGHT_SHIFT_ASSIGNMENT, // '>>='

	// Member / Pointer operators
	LEXERTOKENS_DOT,			  // '.'
	LEXERTOKENS_TYPE_ARROW,		  // '->'
	LEXERTOKENS_ASSIGNMENT_ARROW, // '=>'
	LEXERTOKENS_AT,				  // '@'

	// Syntactic constructs
	LEXERTOKENS_OPEN_BRACE,			// '('
	LEXERTOKENS_OPEN_SQUARE_BRACE,	// '['
	LEXERTOKENS_OPEN_CURLY_BRACE,	// '{'
	LEXERTOKENS_CLOSE_BRACE,		// ')'
	LEXERTOKENS_CLOSE_SQUARE_BRACE, // ']'
	LEXERTOKENS_CLOSE_CURLY_BRACE,	// '}'
	LEXERTOKENS_COMMA,				// ','
	LEXERTOKENS_COLON,				// ':'
	LEXERTOKENS_SCOPE_RESOLUTION,	// '::'

	// Misc
	LEXERTOKENS_SINGLE_LINE_COMMENT,
	LEXERTOKENS_MULTI_LINE_COMMENT
};

/**
 * Contains the names of each of the lexer token identifiers.
 */
extern const struct Array g_LEXERTOKEN_NAMES;

/**
 * Gets the name of a lexer token.
 *
 * @param IDENTIFIER The lexer token's identifier.
 *
 * @return The name of the lexer token.
 */
const char* lexer_tokens_get_name(
	const enum LexerTokenIdentifiers IDENTIFIER); // NOLINT(readability-avoid-const-params-in-decls)

/**
 * Used to identify the precedence of different tokens. Comparison can
 * be done with 'strcmp(a, b) < 0' ('true' if 'a' precedes over 'b',
 * else 'false').
 */
extern const struct Array g_LEXER_TOKEN_PRECEDENCES;

/**
 * Represents a lexer token.
 */
struct LexerToken {
	enum LexerTokenIdentifiers identifier;
	size_t					   startChrIndex, endChrIndex, lineIndex;
	const struct String*	   value;
};

#define LEXERTOKEN_STRUCT_SIZE sizeof(struct LexerToken)

/**
 * Creates a new LexerToken struct.
 *
 * @param identifier       Token identifier.
 * @param p_value          Value of the token.
 * @param startChrIndex    Start char index of the token.
 * @param endChrIndex      End char index of the token.
 * @param lineIndex        Line index of the token.
 *
 * @return The created LexerToken struct.
 */
const struct LexerToken* lexer_token_new(enum LexerTokenIdentifiers identifier,
										 struct String* p_value, size_t startChrIndex,
										 size_t endChrIndex, size_t lineIndex);

/**
 * Frees an LexerToken struct.
 *
 * @param p_self The current LexerToken struct.
 */
void lexer_token_free(struct LexerToken** p_self);
