/**
 * Part of the Exeme Language Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include "../includes.c"

#include "../utils/array.c"
#include "../utils/conversions.c"
#include "../utils/panic.c"
#include "../utils/string.c"

/**
 * Used to identify keywords.
 */
static const struct Array KEYWORDS = {
	14,
	(void *[]){
		"break",
		"case",
		"class",
		"else",
		"elif",
		"enum",
		"for",
		"func",
		"if",
		"import",
		"match",
		"pass",
		"return",
		"using",
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
	LEXERTOKENS_COMMA,				// ','
	LEXERTOKENS_COLON,				// ':'
	LEXERTOKENS_SCOPE_RESOLUTION,	// '::'

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
		"multiplication operator",
		"exponent operator",
		"division operator",
		"floor division operator",
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
		"multiplication assignment operator",
		"exponent assignment operator",
		"divison assignment operator",
		"floor division assignment operator",
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
		"j",
		"b",
		"a",

		// Misc
		"a",
	},
};

/**
 * Represents a lexer token.
 */
struct LexerToken {
	enum LexerTokens identifier;
	size_t startChrIndex, endChrIndex, lineIndex;
	const struct String *value;
};

#define LEXERTOKEN_STRUCT_SIZE sizeof(struct LexerToken)

/**
 * Creates a new LexerToken struct.
 *
 * @param identifier       Token identifier.
 * @param value            Value of the token.
 * @param startChrIndex    Start char index of the token.
 * @param endChrIndex      End char index of the token.
 * @param lineIndex        Line num of the token.
 *
 * @return The created LexerToken struct.
 */
const struct LexerToken *lexerToken_new(enum LexerTokens identifier,
										struct String *value,
										size_t startChrIndex,
										size_t endChrIndex, size_t lineIndex) {
	struct LexerToken *self = malloc(LEXERTOKEN_STRUCT_SIZE);

	if (!self) {
		panic("failed to malloc LexerToken struct");
	}

	self->identifier = identifier;
	self->value = value;
	self->startChrIndex = startChrIndex;
	self->endChrIndex = endChrIndex;
	self->lineIndex = lineIndex;

	return self;
}

/**
 * Frees an LexerToken struct.
 *
 * @param self The current LexerToken struct.
 */
void lexerToken_free(struct LexerToken *self) {
	if (self) {
		string_free(self->value);
		free(self);

		self = NULL;
	} else {
		panic("LexerToken struct has already been freed");
	}
}

/**
 * Represents a lexer.
 */
struct Lexer {
	bool nextLine;
	char chr, prevChr;
	const char *FILE_PATH;
	FILE *filePointer;
	size_t chrIndex, lineIndex, tokenUnlexes;
	struct Array *tokens;
};

#define LEXER_STRUCT_SIZE sizeof(struct Lexer)

struct Lexer *lexer_new(const char *FILE_PATH) {
	struct Lexer *self = malloc(LEXER_STRUCT_SIZE);

	if (!self) {
		panic("failed to malloc Lexer struct");
	}

	self->nextLine = true;
	self->chr = '\n';
	self->prevChr = '\0';
	self->FILE_PATH = FILE_PATH;
	self->filePointer = fopen(FILE_PATH, "r");
	self->chrIndex = 0;
	self->lineIndex = negativeIndex; // Will wrap around when a line is got
	self->tokenUnlexes = 0;
	self->tokens = array_new();

	if (ferror(self->filePointer)) { // Probably file doesn't exist
		panic(stringConcatenate(3, "failed to open file '", self->FILE_PATH,
								"'"));
	}

	return self;
}

/**
 * Prints a lexing error and exits.
 *
 * @param self      The current lexer struct.
 * @param ERROR_MSG The error message.
 * @param token     The erroneous token.
 */
void lexer_error(struct Lexer *self, const char *ERROR_MSG,
				 const struct LexerToken *token) {
	printf("%s on line index %zu char index %zu\n", ERROR_MSG, token->lineIndex,
		   token->startChrIndex);

	exit(EXIT_FAILURE);
}

/**
 * Gets the next line.
 *
 * @param self     The current lexer struct.
 * @param nextLine Get the next line even if the EOL has not been
 * reached.
 *
 * @return Whether the next line was got successfully.
 */
bool lexer_getLine(struct Lexer *self, bool nextLine) {
	if (!self->filePointer) { // EOF
		return false;
	} else if (!nextLine && !self->nextLine) { // EOL has not been reached
		return true;
	}

	self->prevChr = '\0';
	self->chr = '\n';

	self->chrIndex = negativeIndex; // Will wrap around when a char is got
	self->lineIndex++;

	self->nextLine = false;

	return true;
}

/**
 * Un-gets the current character.
 *
 * @param self The current lexer struct.
 *
 * @return Whether the current character was un-got successfully.
 */
bool lexer_unGetChr(struct Lexer *self) {
	if (!self->filePointer) { // EOF
		return false;
	}

	ungetc(self->chr, self->filePointer);

	self->chr = self->prevChr;
	self->prevChr = '\0'; // We don't know what it was before this
	self->chrIndex--;

	if (ferror(self->filePointer) != 0) {
		fclose(self->filePointer);
		self->filePointer = NULL; // Set to NULL to prevent future errors

		return false;
	}

	return true;
}

/**
 * Gets the next char.
 *
 * @param self           The current lexer struct.
 * @param skipWhitespace Whether to skip whitespace chars.
 *
 * @return Whether the next char was got successfully.
 */
bool lexer_getChr(struct Lexer *self, bool skipWhitespace) {
	if (!self->filePointer || self->nextLine) { // EOF / EOL
		return false;
	}

	while (true) {
		char prevChr = self->prevChr;
		self->prevChr = self->chr;
		self->chr = (char)fgetc(
			self->filePointer); // Specify cast to char to silence warnings
		self->chrIndex++;

		if (self->chr == EOF ||
			ferror(self->filePointer) != 0) { // EOF or error
			fclose(self->filePointer);
			self->filePointer = NULL; // Set to NULL to prevent future errors

			return false;
		}

		if (self->chr == '\n') { // EOL has been reached
			self->chr = self->prevChr;
			self->prevChr = prevChr;
			self->nextLine = true;

			return false;
		} else if (skipWhitespace) {   // Keep going till we encounter a chars
									   // that is not whitespace
			if (!isspace(self->chr)) { // Not whitespace
				break;
			}
		} else { // Don't skip whitespace chars
			break;
		}
	}

	return true;
}

/**
 * Checks for an unexpected continuation of the current token.
 *
 * @param self  The current lexer struct.
 * @param token The current token.
 */
void lexer_checkForContinuation(struct Lexer *self,
								const struct LexerToken *token) {
	if (lexer_getChr(self, false)) {
		if (!isspace(self->chr) && !isalnum(self->chr)) {
			lexer_error(self,
						stringConcatenate(3,
										  "unexpected continuation of token '",
										  token->value->_value, "'"),
						token);
		}

		lexer_unGetChr(
			self); // Token was not continued, un-get the current char
	}
}

/**
 * Creates a LexerToken for a one-char token.
 *
 * @param self       The current lexer struct.
 * @param IDENTIFIER The current token's identifier.
 */
void lexer_lexOneChar(struct Lexer *self, const enum LexerTokens IDENTIFIER) {
	const struct LexerToken *token =
		lexerToken_new(IDENTIFIER, string_new(chrToString(self->chr), false),
					   self->chrIndex, self->chrIndex, self->lineIndex);

	array_insert(self->tokens, 0, token);
}

/**
 * Creates a LexerToken for a possible two-char token.
 *
 * @param self       The current lexer struct.
 * @param SECOND_CHR The second char to check for.
 * @param IF_ONE     If the token is one char.
 * @param IF_TWO     If the token is two chars.
 */
void lexer_lexTwoChar(struct Lexer *self, const char SECOND_CHR,
					  const enum LexerTokens IF_ONE,
					  const enum LexerTokens IF_TWO) {
	const struct LexerToken *token = NULL;

	if (lexer_getChr(self, false)) {
		if (self->chr == SECOND_CHR) {
			token = lexerToken_new(
				IF_TWO,
				string_new(stringConcatenate(2, chrToString(self->prevChr),
											 chrToString(self->chr)),
						   false),
				self->chrIndex - 1, self->chrIndex, self->lineIndex);
		} else { // SECOND_CHR was not found, un-get it
			lexer_unGetChr(self);
		}
	}

	if (!token) { // SECOND_CHR was not found
		token =
			lexerToken_new(IF_ONE, string_new(chrToString(self->chr), false),
						   self->chrIndex, self->chrIndex, self->lineIndex);
	}

	array_insert(self->tokens, 0, token);
	lexer_checkForContinuation(self, token);
}

/**
 * Creates a LexerToken for a possible two-char token with two possible second
 * chars.
 *
 * @param self             The current lexer struct.
 * @param SECOND_CHR       The second char to check for.
 * @param OTHER_SECOND_CHR The other second char to check for.
 * @param IF_ONE           If the token is one char.
 * @param IF_TWO           If the token is two chars with SECOND_CHR.
 * @param IF_OTHER_TWO     If the token is two chars with OTEHR_SECOND_CHR.
 */
void lexer_lex2TwoChar(struct Lexer *self, const char SECOND_CHR,
					   const char OTHER_SECOND_CHR,
					   const enum LexerTokens IF_ONE,
					   const enum LexerTokens IF_TWO,
					   const enum LexerTokens IF_OTHER_TWO) {
	const struct LexerToken *token = NULL;

	if (lexer_getChr(self, false)) {
		if (self->chr == SECOND_CHR) {
			token = lexerToken_new(
				IF_TWO,
				string_new(stringConcatenate(2, chrToString(self->prevChr),
											 chrToString(self->chr)),
						   false),
				self->chrIndex - 1, self->chrIndex, self->lineIndex);
		} else if (self->chr == OTHER_SECOND_CHR) {
			token = lexerToken_new(
				IF_OTHER_TWO,
				string_new(stringConcatenate(2, chrToString(self->prevChr),
											 chrToString(self->chr)),
						   false),
				self->chrIndex - 1, self->chrIndex, self->lineIndex);
		} else {
			lexer_unGetChr(self);
		}
	}

	if (!token) {
		token =
			lexerToken_new(IF_ONE, string_new(chrToString(self->chr), false),
						   self->chrIndex, self->chrIndex, self->lineIndex);
	}

	array_insert(self->tokens, 0, token);
	lexer_checkForContinuation(self, token);
}

/**
 * Creates a LexerToken for a possible three-char token.
 *
 * @param self           The current lexer struct.
 * @param SECOND_CHR     The second char to check for.
 * @param THIRD_CHR      The third char to check for.
 * @param IF_ONE         If the token is one char.
 * @param IF_TWO         If the token is two SECOND_CHRs.
 * @param IF_ONE_AND_ONE If the token is SECOND_CHR and THIRD_CHR.
 * @param IF_TWO_AND_ONE If the token is two of SECOND_CHR and THIRD_CHR.
 */
void lexer_lexThreeChar(struct Lexer *self, const char SECOND_CHR,
						const char THIRD_CHR, const enum LexerTokens IF_ONE,
						const enum LexerTokens IF_TWO,
						const enum LexerTokens IF_ONE_AND_ONE,
						const enum LexerTokens IF_TWO_AND_ONE) {
	const struct LexerToken *token = NULL;

	if (lexer_getChr(self, false)) {
		if (self->chr == SECOND_CHR) {
			const char prevChr = self->prevChr;

			if (IF_TWO_AND_ONE && lexer_getChr(self, false)) {
				if (self->chr == THIRD_CHR) {
					token = lexerToken_new(
						IF_TWO_AND_ONE,
						string_new(stringConcatenate(3, chrToString(prevChr),
													 chrToString(self->prevChr),
													 chrToString(self->chr)),
								   false),
						self->chrIndex - 2, self->chrIndex, self->lineIndex);
				}
			}

			if (!token) { // THIRD_CHR was not found
				token = lexerToken_new(
					IF_TWO,
					string_new(stringConcatenate(2, chrToString(self->prevChr),
												 chrToString(self->chr)),
							   false),
					self->chrIndex - 1, self->chrIndex, self->lineIndex);
			}
		} else if (self->chr == THIRD_CHR) {
			token = lexerToken_new(
				IF_ONE_AND_ONE,
				string_new(stringConcatenate(2, chrToString(self->prevChr),
											 chrToString(self->chr)),
						   false),
				self->chrIndex - 1, self->chrIndex, self->lineIndex);
		} else { // SECOND_CHR was not found, un-get it
			lexer_unGetChr(self);
		}
	}

	if (!token) { // SECOND_CHR was not found
		token =
			lexerToken_new(IF_ONE, string_new(chrToString(self->chr), false),
						   self->chrIndex, self->chrIndex, self->lineIndex);
	}

	array_insert(self->tokens, 0, token);
	lexer_checkForContinuation(self, token);
}

/**
 * Calls the correct function for lexing the current character.
 *
 * @param self The current lexer struct.
 *
 * @return bool - Whether lexing succeeded.
 */
bool lexer_lexNext(struct Lexer *self) {
	switch (self->chr) {
	// Arithmetic operators
	case '%':
		lexer_lexTwoChar(self, '=', LEXERTOKENS_MODULO,
						 LEXERTOKENS_MODULO_ASSIGNMENT);
		break;
	case '*':
		lexer_lexThreeChar(self, self->chr, '=', LEXERTOKENS_MULTIPLICATION,
						   LEXERTOKENS_EXPONENT,
						   LEXERTOKENS_MULTIPLICATION_ASSIGNMENT,
						   LEXERTOKENS_EXPONENT_ASSIGNMENT);
		break;
	case '/':
		lexer_lexThreeChar(self, self->chr, '=', LEXERTOKENS_DIVISION,
						   LEXERTOKENS_FLOOR_DIVISION,
						   LEXERTOKENS_DIVISION_ASSIGNMENT,
						   LEXERTOKENS_FLOOR_DIVISION_ASSIGNMENT);
		break;
	case '+':
		lexer_lexTwoChar(self, '=', LEXERTOKENS_ADDITION,
						 LEXERTOKENS_ADDITION_ASSIGNMENT);
		break;
	case '-':
		lexer_lex2TwoChar(self, '=', '>', LEXERTOKENS_SUBTRACTION,
						  LEXERTOKENS_SUBTRACTION_ASSIGNMENT,
						  LEXERTOKENS_ARROW);
		break;

	// Comparison / Relational operators
	case '=':
		lexer_lexTwoChar(self, self->chr, LEXERTOKENS_ASSIGNMENT,
						 LEXERTOKENS_EQUAL_TO);
		break;
	case '!':
		lexer_lexTwoChar(self, '=', LEXERTOKENS_LOGICAL_NOT,
						 LEXERTOKENS_NOT_EQUAL_TO);
		break;
	case '>':
		lexer_lexThreeChar(self, self->chr, '=', LEXERTOKENS_GREATER_THAN,
						   LEXERTOKENS_BITWISE_RIGHT_SHIFT,
						   LEXERTOKENS_GREATER_THAN_OR_EQUAL,
						   LEXERTOKENS_BITWISE_RIGHT_SHIFT_ASSIGNMENT);
		break;
	case '<':
		lexer_lexThreeChar(self, self->chr, '=', LEXERTOKENS_LESS_THAN,
						   LEXERTOKENS_BITWISE_LEFT_SHIFT,
						   LEXERTOKENS_LESS_THAN_OR_EQUAL,
						   LEXERTOKENS_BITWISE_LEFT_SHIFT_ASSIGNMENT);
		break;

	// Logical operators
	case '&':
		lexer_lexThreeChar(self, self->chr, '=', LEXERTOKENS_BITWISE_AND,
						   LEXERTOKENS_LOGICAL_AND,
						   LEXERTOKENS_BITWISE_AND_ASSIGNMENT,
						   LEXERTOKENS_NONE);
		break;
	case '|':
		lexer_lexThreeChar(self, self->chr, '=', LEXERTOKENS_BITWISE_OR,
						   LEXERTOKENS_LOGICAL_OR,
						   LEXERTOKENS_BITWISE_OR_ASSIGNMENT, LEXERTOKENS_NONE);
		break;

	// Bitwise operators
	case '^':
		lexer_lexTwoChar(self, '=', LEXERTOKENS_BITWISE_XOR,
						 LEXERTOKENS_BITWISE_XOR_ASSIGNMENT);
		break;
	case '~':
		lexer_lexTwoChar(self, '=', LEXERTOKENS_BITWISE_NOT,
						 LEXERTOKENS_BITWISE_NOT_ASSIGNMENT);
		break;

	// Member / Pointer operators
	case '.':
		lexer_lexOneChar(self, LEXERTOKENS_DOT);
		break;
	case '@':
		lexer_lexOneChar(self, LEXERTOKENS_AT);
		break;

	// Syntactic constructs
	case '(':
		lexer_lexOneChar(self, LEXERTOKENS_OPEN_BRACE);
		break;
	case '[':
		lexer_lexOneChar(self, LEXERTOKENS_OPEN_SQUARE_BRACE);
		break;
	case '{':
		lexer_lexOneChar(self, LEXERTOKENS_OPEN_CURLY_BRACE);
		break;
	case ')':
		lexer_lexOneChar(self, LEXERTOKENS_CLOSE_BRACE);
		break;
	case ']':
		lexer_lexOneChar(self, LEXERTOKENS_CLOSE_SQUARE_BRACE);
		break;
	case '}':
		lexer_lexOneChar(self, LEXERTOKENS_CLOSE_CURLY_BRACE);
		break;
	case ',':
		lexer_lexOneChar(self, LEXERTOKENS_COMMA);
		break;
	case ':':
		lexer_lexTwoChar(self, self->chr, LEXERTOKENS_COLON,
						 LEXERTOKENS_SCOPE_RESOLUTION);
		break;
	}

	return true;
}

/**
 * Gets the next char and lexes it.
 *
 * @param self     The current lexer struct.
 * @param nextLine Whether the char can be on the next line.
 *
 * @return bool Whether lexing succeeded.
 */
bool lexer_lex(struct Lexer *self, bool nextLine) {
	while (true) {
		while (!lexer_getChr(self, true)) { // EOL has been reached
			if (!nextLine || !lexer_getLine(self, true)) {
				return false;
			}
		}

		if (lexer_lexNext(self)) { // If a token was lexed (because some
								   // tokens like comments don't count)
			return true;
		}
	}
}

/**
 * Un-lexes the last token.
 *
 * @param self The current lexer struct.
 */
void lexer_unLex(struct Lexer *self) { self->tokenUnlexes++; }

/**
 * Retreives the last token.
 *
 * @param self The current lexer struct.
 *
 * @return The retreived token.
 */
const struct LexerToken *lexer_getToken(struct Lexer *self) {
	if (self->tokens->length == 0) {
		return NULL;
	} else if (self->tokenUnlexes > 0) {
		return self->tokens
			->_values[self->tokens->length - 1 -
					  (self->tokenUnlexes--)]; // For future me, yes it does
											   // decrement
	}

	return self->tokens->_values[self->tokens->length - 1];
}
