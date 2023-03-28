#pragma once

#include "../includes.c"

#include "../utils/array.c"
#include "../utils/panic.c"

/**
 * Used to identify keywords.
 */
static const struct Array KEYWORDS = {
	13,
	(void *[]){
		"break",
		"catch",
		"class",
		"continue",
		"elif",
		"else",
		"fn",
		"for",
		"if",
		"import",
		"return",
		"try",
		"while",
	},
};

/**
 * Used to identify different lexer tokens.
 */
enum LexerTokens {
	LEXERTOKENS_NONE,

	LEXERTOKENS_KEYWORD,
	LEXERTOKENS_IDENTIFIER,

	LEXERTOKENS_CHR,
	LEXERTOKENS_STRING,
	LEXERTOKENS_INTEGER,
	LEXERTOKENS_FLOAT,

	// Arithmetic operators
	LEXERTOKENS_MODULO,			// '%'
	LEXERTOKENS_EXPONENT,		// '**'
	LEXERTOKENS_DIVISION,		// '/'
	LEXERTOKENS_FLOOR_DIVISION, // '//'
	LEXERTOKENS_MULTIPLICATION, // '*'
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
	LEXERTOKENS_EXPONENT_ASSIGNMENT,	   // '**='
	LEXERTOKENS_DIVISION_ASSIGNMENT,	   // '/='
	LEXERTOKENS_FLOOR_DIVISION_ASSIGNMENT, // '//='
	LEXERTOKENS_MULTIPLICATION_ASSIGNMENT, // '*='
	LEXERTOKENS_ADDITION_ASSIGNMENT,	   // '+='
	LEXERTOKENS_SUBTRACTION_ASSIGNMENT,	   // '-='

	LEXERTOKENS_BITWISE_AND_ASSIGNMENT,			// '&='
	LEXERTOKENS_BITWISE_OR_ASSIGNMENT,			// '|='
	LEXERTOKENS_BITWISE_XOR_ASSIGNMENT,			// '^='
	LEXERTOKENS_BITWISE_NOT_ASSIGNMENT,			// '~='
	LEXERTOKENS_BITWISE_LEFT_SHIFT_ASSIGNMENT,	// '<<='
	LEXERTOKENS_BITWISE_RIGHT_SHIFT_ASSIGNMENT, // '>>='

	// Member / Pointer operators
	LEXERTOKENS_DOT,   // '.'
	LEXERTOKENS_ARROW, // '->'
	LEXERTOKENS_AT,	   // '@'

	// Syntactic constructs
	LEXERTOKENS_OPEN_BRACE,			// '('
	LEXERTOKENS_OPEN_SQUARE_BRACE,	// '['
	LEXERTOKENS_OPEN_CURLY_BRACE,	// '{'
	LEXERTOKENS_CLOSE_BRACE,		// ')'
	LEXERTOKENS_CLOSE_SQUARE_BRACE, // ']'
	LEXERTOKENS_CLOSE_CURLY_BRACE,	// '}'

	// Other operators
	LEXERTOKENS_COMMA,			  // ','
	LEXERTOKENS_COLON,			  // ':'
	LEXERTOKENS_SCOPE_RESOLUTION, // '::'

	// Misc
	LEXERTOKENS_COMMENT,
};

/**
 * Contains the names of each of the lexer token identifiers.
 */
const struct Array LEXER_TOKEN_NAMES = {
	54,
	(void *[]){
		"",

		"keyword",
		"identifier",

		"char",
		"string",
		"integer",
		"float",

		// Arithmetic operators
		"modulo operator",
		"exponent operator",
		"division operator",
		"floor division operator",
		"multiplication operator",
		"addition operator",
		"subtraction operator",

		// Comparison / Relational operators
		"equal to operator",
		"not equal to operator",
		"greater then operator",
		"less than operator",
		"greater than or equal operator",
		"less than or equal operator",

		// Logical operators
		"logical and operator",
		"logical or operator",
		"logical not operator",

		// Bitwise operators
		"bitwise AND operator",
		"bitwise OR operator",
		"bitwise XOR operator",
		"bitwise NOT operator",
		"bitwise left shift",
		"bitwise right shift",

		// Assignment operators
		"assignment operator",

		"modulo assignment operator",
		"exponent assignment operator",
		"divison assignment operator",
		"floor division assignment operator",
		"multiplication assignment operator",
		"addition assignment operator",
		"subtraction assignment operator",

		"bitwise AND assignment operator",
		"bitwise OR assignment operator",
		"bitwise XOR assignment operator",
		"bitwise NOT assignment operator",
		"bitwise left shift assignment operator",
		"bitwise right shift assignment operator",

		// Member / Pointer operators
		"dot operator",
		"arrow operator",
		"at operator",

		// Syntactic constructs
		"open brace",
		"open square brace",
		"open curly brace",
		"close brace",
		"close sqaure brace",
		"close curly brace",

		// Other operators
		"comma",
		"colon",
		"scope resolution operator",

		// Misc
		"comment",
	},
};

/**
 * Used to identify the precedence of different tokens. Comparison can
 * be done with 'strcmp(a, b) < 0' ('true' if 'a' precedes over 'b',
 * else 'false').
 */
const struct Array LEXER_TOKEN_PRECEDENCES = {
	54,
	(void *[]){
		"a",

		"a",
		"a",

		"a",
		"a",
		"a",
		"a",

		// Arithmetic operators
		"d",
		"c",
		"d",
		"d",
		"d",
		"e",
		"e",

		// Comparison / Relational operators
		"g",
		"g",
		"f",
		"f",
		"f",
		"f",

		// Logical operators
		"h",
		"h",
		"h",

		// Bitwise operators
		"e",
		"e",
		"e",
		"e",
		"i",
		"i",

		// Assignment operators
		"i",

		"i",
		"i",
		"i",
		"i",
		"i",
		"i",
		"i",

		"i",
		"i",
		"i",
		"i",
		"i",
		"i",

		// Member / Pointer operators
		"b",
		"b",
		"c",

		// Syntactic constructs
		"b",
		"b",
		"b",
		"b",
		"b",
		"b",

		// Other operators
		"j",
		"b",
		"a",

		// Misc
		"a",
	},
};

struct Lexer {
	char chr, prevChr;
	const char *FILE_PATH;
	FILE *fptr;
	size_t chrIndex, lineIndex;
};

#define LEXER_STRUCT_SIZE sizeof(struct Lexer)

struct Lexer *lexer_new(const char *FILE_PATH) {
	struct Lexer *self = malloc(LEXER_STRUCT_SIZE);

	if (!self) {
		panic("failed to malloc Lexer struct");
	}

	self->FILE_PATH = FILE_PATH;

	return self;
}
