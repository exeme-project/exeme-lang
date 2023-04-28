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
#include <stdio.h>

/**
 * Used to identify keywords.
 */
static const struct Array KEYWORDS = {
	15,
	(const void *[]){
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
enum LexerTokensIdentifiers {
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
	LEXERTOKENS_SINGLE_LINE_COMMENT,
	LEXERTOKENS_MULTI_LINE_COMMENT
};

/**
 * Contains the names of each of the lexer token identifiers.
 */
const struct Array LEXER_TOKEN_NAMES = {
	56,
	(const void *[]){
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
		"single line comment",
		"multi line comment",
	},
};

/**
 * Used to identify the precedence of different tokens. Comparison can
 * be done with 'strcmp(a, b) < 0' ('true' if 'a' precedes over 'b',
 * else 'false').
 */
const struct Array LEXER_TOKEN_PRECEDENCES = {
	56,
	(const void *[]){
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
		"a",
	},
};

/**
 * Represents a lexer token.
 */
struct LexerToken {
	enum LexerTokensIdentifiers identifier;
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
 * @param lineIndex        Line index of the token.
 *
 * @return The created LexerToken struct.
 */
const struct LexerToken *lexerToken_new(enum LexerTokensIdentifiers identifier,
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
void lexerToken_free(const struct LexerToken *self) {
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

/**
 * Creates a new Lexer struct.
 *
 * @param FILE_PATH The path of the file to lex.
 *
 * @return The created Lexer struct.
 */
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
 * Frees an Lexer struct.
 *
 * @param self The current Lexer struct.
 */
void lexer_free(const struct Lexer *self) {
	if (self) {
		if (self->filePointer) {
			fclose(self->filePointer);
		}

		array_free(self->tokens);

		free(self);
		self = NULL;
	} else {
		panic("Lexer struct has already been freed");
	}
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
	if (token) {
		FILE *filePointer = fopen(self->FILE_PATH, "r");
		struct String *line = string_new("\0", true);
		size_t lineIndex = 0;

		while (true) {
			char chr = fgetc(filePointer);

			if (chr == '\n') {
				if (self->lineIndex++ == lineIndex) {
					break;
				}

				string_clear(line);
			} else if (lineIndex == self->lineIndex) {
				string_append(line, chr);
			}
		}

		printf("--> %s\n", self->FILE_PATH);
	} else {
		printf("%s on line index %zu at char index %zu\n", ERROR_MSG,
			   self->lineIndex, self->chrIndex);
	}

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
			self->chrIndex--;

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
void lexer_lexOneChar(struct Lexer *self,
					  const enum LexerTokensIdentifiers IDENTIFIER) {
	const struct LexerToken *token =
		lexerToken_new(IDENTIFIER, string_new(chrToString(self->chr), false),
					   self->chrIndex, self->chrIndex, self->lineIndex);

	array_insert(self->tokens, self->tokens->length, token);
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
					  const enum LexerTokensIdentifiers IF_ONE,
					  const enum LexerTokensIdentifiers IF_TWO) {
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

	array_insert(self->tokens, self->tokens->length, token);
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
					   const enum LexerTokensIdentifiers IF_ONE,
					   const enum LexerTokensIdentifiers IF_TWO,
					   const enum LexerTokensIdentifiers IF_OTHER_TWO) {
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

	array_insert(self->tokens, self->tokens->length, token);
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
						const char THIRD_CHR,
						const enum LexerTokensIdentifiers IF_ONE,
						const enum LexerTokensIdentifiers IF_TWO,
						const enum LexerTokensIdentifiers IF_ONE_AND_ONE,
						const enum LexerTokensIdentifiers IF_TWO_AND_ONE) {
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

	array_insert(self->tokens, self->tokens->length, token);
	lexer_checkForContinuation(self, token);
}

/**
 * Escapes the current character in the lexer struct.
 *
 * @param self          The current lexer struct.
 * @param startChrIndex The start index of the char.
 *
 * @return The escaped char.
 */
char lexer_escapeChr(struct Lexer *self, const size_t startChrIndex) {
	switch (self->chr) {
	case 'b':
		return '\b';
	case 'f':
		return '\f';
	case 'n':
		return '\n';
	case 'r':
		return '\r';
	case 't':
		return '\t';
	case 'v':
		return '\v';
	case '\'':
		return '\'';
	case '"':
		return '"';
	case '\\':
		return '\\';
	}

	lexer_error(self, "invalid escape sequence",
				lexerToken_new(LEXERTOKENS_NONE, string_new("\0", true),
							   startChrIndex, self->chrIndex, self->lineIndex));

	return '\0';
}

/**
 * Creates a LexerToken for a char.
 *
 * @param self The current lexer struct.
 */
void lexer_lexChr(struct Lexer *self) {
	bool escapeChr = false;
	const size_t startChrIndex = self->chrIndex;
	struct String *chr = string_new("\0", true);

	while (lexer_getChr(self, false)) {
		if (self->chr == '\'') {
			array_insert(self->tokens, self->tokens->length,
						 lexerToken_new(LEXERTOKENS_CHR, chr, startChrIndex,
										self->chrIndex, self->lineIndex));
			return;
		} else if (chr->length == 1) {
			lexer_error(self, "multi-character char",
						lexerToken_new(LEXERTOKENS_NONE, string_new("\0", true),
									   startChrIndex, self->chrIndex,
									   self->lineIndex));
		}

		if (escapeChr) {
			string_append(chr, lexer_escapeChr(self, startChrIndex));
			escapeChr = false;
		} else if (self->chr == '\\') {
			escapeChr = true;
		} else {
			string_append(chr, self->chr);
		}
	}

	lexer_error(self, "unterminated char",
				lexerToken_new(LEXERTOKENS_NONE, chr, startChrIndex,
							   self->chrIndex, self->lineIndex));
}

/**
 * Creates a LexerToken for a string.
 *
 * @param self The current lexer struct.
 */
void lexer_lexString(struct Lexer *self) {
	bool escapeChr = false;
	const size_t startChrIndex = self->chrIndex,
				 startLineIndex = self->lineIndex;
	struct String *string = string_new("\0", true);

	while (lexer_getLine(self, false)) {
		while (lexer_getChr(self, false)) {
			if (self->chr == '"') {
				array_insert(self->tokens, self->tokens->length,
							 lexerToken_new(LEXERTOKENS_STRING, string,
											startChrIndex, self->chrIndex,
											self->lineIndex));
				return;
			}

			if (escapeChr) {
				string_append(string, lexer_escapeChr(self, startChrIndex));
				escapeChr = false;
			} else if (self->chr == '\\') {
				escapeChr = true;
			} else {
				string_append(string, self->chr);
			}
		}

		string_append(string, '\n');
	}

	lexer_error(
		self, "unterminated string",
		lexerToken_new(LEXERTOKENS_STRING, string,
					   self->lineIndex == startLineIndex ? startChrIndex : 0,
					   self->chrIndex, self->lineIndex));
}

/**
 * Creates a LexerToken for a multi-line comment.
 *
 * @param self          The current lexer struct.
 * @param startChrIndex The start char index of the comment.
 */
void lexer_lexMultiLineComment(struct Lexer *self, const size_t startChrIndex) {
	const size_t startLineIndex = self->lineIndex;

	while (lexer_getLine(self, false)) {
		while (lexer_getChr(self, true)) {
			if (self->chr == '=') {
				if (lexer_getChr(self, false)) {
					if (self->chr == '#') {
						array_insert(
							self->tokens, self->tokens->length,
							lexerToken_new(LEXERTOKENS_MULTI_LINE_COMMENT,
										   string_new("\0", true),
										   self->lineIndex == startLineIndex
											   ? startChrIndex
											   : 0,
										   self->chrIndex, startLineIndex));
						return;
					}
				}
			}
		}
	}

	lexer_error(
		self, "unterminated multi-line comment",
		lexerToken_new(LEXERTOKENS_NONE, string_new("\0", true),
					   self->lineIndex == startLineIndex ? startChrIndex : 0,
					   self->chrIndex, self->lineIndex));
}

/**
 * Creates a LexerToken for a single-line comment.
 *
 * @param self The current lexer struct.
 */
void lexer_lexSingleLineComment(struct Lexer *self) {
	const size_t startChrIndex = self->chrIndex;

	if (lexer_getChr(self, false)) {
		if (self->chr == '=') {
			lexer_lexMultiLineComment(self, startChrIndex);

			return;
		} else {
			while (lexer_getChr(self, true)) {
			}
		}
	}

	array_insert(self->tokens, self->tokens->length,
				 lexerToken_new(LEXERTOKENS_SINGLE_LINE_COMMENT,
								string_new("\0", true), startChrIndex,
								self->chrIndex, self->lineIndex));
}

bool lexer_lexKeywordOrIdentifier_match_(const char *element,
										 const char *match) {
	return strcmp(element, match) == 0;
}

/**
 * Creates a LexerToken for a keyword or identifier.
 *
 * @param self The current lexer struct.
 */
void lexer_lexKeywordOrIdentifier(struct Lexer *self) {
	const size_t startChrIndex = self->chrIndex;
	struct String *identifier = string_new(chrToString(self->chr), false);

	while (lexer_getChr(self, false)) {
		if (!isalnum(self->chr)) {
			lexer_unGetChr(self);
			break;
		}

		string_append(identifier, self->chr);
	}

	array_insert(self->tokens, self->tokens->length,
				 lexerToken_new(array_find(&KEYWORDS,
										   lexer_lexKeywordOrIdentifier_match_,
										   identifier->_value)
									? LEXERTOKENS_KEYWORD
									: LEXERTOKENS_IDENTIFIER,
								identifier, startChrIndex, self->chrIndex,
								self->lineIndex));
}

/**
 * Creates a LexerToken for an integer / float.
 *
 * @param self The current lexer struct.
 */
void lexer_lexNumber(struct Lexer *self) {
	bool isFloat = false;
	size_t startChrIndex = self->chrIndex;
	struct String *number = string_new(chrToString(self->chr), false);

	while (lexer_getChr(self, false)) {
		if (isspace(self->chr)) {
			break;
		} else if (isalpha(self->chr)) {
			lexer_error(self,
						stringConcatenate(2, "invalid character for ",
										  isFloat ? "float" : "integer"),
						lexerToken_new(LEXERTOKENS_NONE, number, startChrIndex,
									   self->chrIndex, self->lineIndex));
		} else if (self->chr == '.') {
			if (isFloat) {
				lexer_error(self, "too many decimal points for float",
							lexerToken_new(LEXERTOKENS_NONE, number,
										   startChrIndex, self->chrIndex,
										   self->lineIndex));
			} else {
				isFloat = true;
			}
		} else if (!isdigit(self->chr)) {
			lexer_unGetChr(self);
			break;
		}

		string_append(number, self->chr);
	}

	array_insert(
		self->tokens, self->tokens->length,
		lexerToken_new(isFloat ? LEXERTOKENS_FLOAT : LEXERTOKENS_INTEGER,
					   number, startChrIndex, self->chrIndex, self->lineIndex));
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
	case '\'':
		lexer_lexChr(self);
		break;
	case '"':
		lexer_lexString(self);
		break;

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
	case '#':
		lexer_lexSingleLineComment(self);
		break;
	default:
		if (isalpha(self->chr) || self->chr == '_') {
			lexer_lexKeywordOrIdentifier(self);
		} else if (isdigit(self->chr)) {
			lexer_lexNumber(self);
		} else {
			lexer_error(self, "unknown character", NULL);
		}

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
	while (!lexer_getChr(self, true)) { // EOL has been reached
		if (!nextLine || !lexer_getLine(self, true)) {
			return false;
		}
	}

	return lexer_lexNext(self);
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
