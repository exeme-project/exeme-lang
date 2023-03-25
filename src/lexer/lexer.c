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
const struct Array KEYWORDS = {
	13,
	(const void *[]){
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

		// Other operators
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
	const char *value;
	enum LexerTokens identifier;
	size_t startChrIndex, chrIndex, lineNum;
};

#define LEXERTOKEN_STRUCT_SIZE sizeof(struct LexerToken)

/**
 * Creates a new LexerToken struct.
 *
 * @param value         Value of the token.
 * @param identifier    Token identifier.
 * @param startChrIndex Start char index of token.
 * @param chrIndex      End char index of the token.
 * @param lineNum       Line num of the token.
 *
 * @return The created LexerToken struct.
 */
const struct LexerToken *lexerToken_new(const char *value,
										enum LexerTokens identifier,
										size_t startChrIndex, size_t chrIndex,
										size_t lineNum) {
	struct LexerToken *self = malloc(LEXERTOKEN_STRUCT_SIZE);

	if (!self) {
		panic("failed to create LexerToken struct");
	}

	self->value = value;
	self->identifier = identifier;
	self->startChrIndex = startChrIndex;
	self->chrIndex = chrIndex;
	self->lineNum = lineNum;

	return self;
}

/**
 * Represents a lexer.
 */
struct Lexer {
	bool closed;
	char chr, prevChr;
	const char *FILE_PATH;
	FILE *filePointer;
	size_t chrIndex, lineNum, parsedTokensCount;
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
		panic("failed to create Lexer struct");
	}

	self->filePointer = fopen(FILE_PATH, "r");

	if (ferror(self->filePointer)) {
		panic(stringConcatenate(3, "failed to open file '", FILE_PATH, "'"));
	}

	self->closed = false;
	self->chr = '\n';
	self->prevChr = '\0';
	self->FILE_PATH = FILE_PATH;
	self->chrIndex = 0;
	self->lineNum = 0;
	self->parsedTokensCount = 0;
	self->tokens = array_new();

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
	size_t length, lineNum = 0;
	char *line;
	FILE *filePointer = fopen(self->FILE_PATH, "r");

	if (ferror(filePointer)) {
		panic(stringConcatenate(
			3, "failed to open file while reporting error '", ERROR_MSG, "'"));
	}

	printf("%s%s--> %s%s\n", F_BRIGHT_BLUE, S_BOLD, S_RESET, self->FILE_PATH);

	while (true) {
		line = "";

		while (true) {
			char chr = (char)fgetc(filePointer);

			if (chr == '\n' || chr == EOF) {
				break;
			}

			strncat(line, &chr, 1);
		}

		lineNum++;

		if (lineNum == self->lineNum) {
			break;
		}
	}

	length = strlen(ulToString(self->chrIndex)) + 3;

	printf("%zu | %s\n", self->lineNum, line);

	if (token == NULL) {
		printf("%s^ %s%serror: %s%s\n", repeatChr(' ', self->chrIndex),
			   F_BRIGHT_RED, S_BOLD, S_RESET, ERROR_MSG);
	} else {
		self->lineNum = token->lineNum;
		self->chrIndex = token->chrIndex;

		printf("%s%s %s%serror: %s%s\n",
			   repeatChr(' ', length + token->startChrIndex),
			   repeatChr('^', self->chrIndex - token->startChrIndex + 1),
			   F_BRIGHT_RED, S_BOLD, S_RESET, ERROR_MSG);
	}

	exit(EXIT_FAILURE);
}

/**
 * Gets the next character.
 *
 * @param self           The current lexer struct.
 * @param skipWhitespace Whether to skip whitespace characters.
 *
 * @return Whether the next character was got successfully.
 */
bool lexer_getChr(struct Lexer *self, bool skipWhitespace) {
	if (self->closed) {
		return false;
	} else if (self->chr == '\n') {
		return false;
	}

	while (true) {
		self->prevChr = self->chr;
		self->chr = (char)fgetc(self->filePointer);

		if (self->chr == EOF ||
			ferror(self->filePointer) != 0) { // End of file (hopefully)
			fclose(self->filePointer);
			self->closed = true;
			return false;
		}

		self->chrIndex++;

		if (self->chr == '\n') {
			return false;
		}

		if (skipWhitespace) { // Keep going till we encounter a character
							  // that is not whitespace
			if (!isspace(self->chr)) { // Not whitespace
				break;
			}
		} else {
			break;
		}
	}

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
	if (self->closed) {
		return false;
	}

	ungetc(self->chr, self->filePointer);

	self->chrIndex--;
	self->chr = self->prevChr;
	self->prevChr = '\0';

	if (ferror(self->filePointer) != 0) {
		fclose(self->filePointer);
		panic(
			stringConcatenate(2, "failed to unget char in ", self->FILE_PATH));
	}

	return true;
}

/**
 * Gets the next line.
 *
 * @param self        The current lexer struct.
 * @param getNextLine Get the next line even if the EOL has not been
 * reached.
 *
 * @return Whether the next line was got successfully.
 */
bool lexer_getLine(struct Lexer *self, bool getNextLine) {
	if (self->closed) {
		return false;
	}

	if (!getNextLine &&
		self->chr != '\n') { // Not reached the end of the line yet
		return true;
	}

	self->prevChr = self->chr;
	self->chr = '\0';

	self->chrIndex = negativeIndex;
	self->lineNum++;

	return true;
}

/**
 * Returns the escaped version of the current character.
 *
 * @param self The current lexer struct.
 *
 * @return The escaped version of the current character.
 */
char lexer_escapeChr(struct Lexer *self) {
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
				lexerToken_new("", LEXERTOKENS_NONE, self->chrIndex - 1,
							   self->chrIndex, self->lineNum));
}

/**
 * Checks for a trailing character.
 *
 * @param self The current lexer struct.
 *
 * @return Whether the trailing character was found.
 */
bool lexer_checkForTrailingChr(struct Lexer *self, char chr) {
	bool found = true;

	if (lexer_getChr(self, false)) {
		if (self->chr != chr) {
			found = false;
		}
	} else {
		found = false;
	}

	if (!found) {
		lexer_unGetChr(self); // Only if a match is not found.
	}

	return found;
}

/**
 * Checks for an unexpected continuation of the current token.
 *
 * @param self  The current lexer struct.
 * @param token The current token.
 */
void lexer_checkForContinuation(struct Lexer *self, const char *token) {
	if (lexer_getChr(self, false)) {
		if (!isspace(self->chr) && !isalnum(self->chr)) {
			lexer_error(self,
						stringConcatenate(3,
										  "unexpected continuation of token '",
										  token, "'"),
						NULL);
		}

		lexer_unGetChr(self);
	}
}

/**
 * Lexes a '=' token.
 *
 * @param self The current lexer struct.
 */
void lexer_lexEquals(struct Lexer *self) {
	const struct LexerToken *token = NULL;

	if (lexer_checkForTrailingChr(self, self->chr)) {
		token = lexerToken_new("==", LEXERTOKENS_EQUAL_TO, self->chrIndex - 1,
							   self->chrIndex, self->lineNum);
	} else {
		token = lexerToken_new("=", LEXERTOKENS_ASSIGNMENT, self->chrIndex - 1,
							   self->chrIndex, self->lineNum);
	}

	array_insert(self->tokens, self->tokens->length, token);

	lexer_checkForContinuation(self, token->value);
}

/**
 * Lexes a character.
 *
 * @param self The current lexer struct.
 */
void lexer_lexChr(struct Lexer *self) {
	bool includeChr = false;
	char *chr = createHeapString();
	size_t startChrIndex = self->chrIndex;

	if (!chr) {
		panic("failed to malloc char");
	}

	while (lexer_getChr(self, false)) {
		if (chr[0] != '\0') {
			lexer_error(self, "multi-character char",
						lexerToken_new("", LEXERTOKENS_NONE, startChrIndex,
									   self->chrIndex, self->lineNum));
		}

		if (self->chr == '\'') {
			array_insert(self->tokens, self->tokens->length,
						 lexerToken_new(chr, LEXERTOKENS_CHR, startChrIndex,
										self->chrIndex, self->lineNum));
			return;
		}

		if (includeChr) {
			stringConcatenateChr(chr, lexer_escapeChr(self));
			includeChr = false;
		} else if (self->chr != '\\') {
			stringConcatenateChr(chr, self->chr);
		} else {
			includeChr = true;
		}
	}

	lexer_unGetChr(self);
	lexer_error(self, "unterminated char", NULL);
}

/**
 * Lexes a string.
 *
 * @param self The current lexer struct.
 */
void lexer_lexString(struct Lexer *self) {
	bool includeChr = false;
	char *string = createHeapString();
	size_t startChrIndex = self->chrIndex, startLineNum = self->lineNum;

	while (lexer_getLine(self, false)) {
		while (lexer_getChr(self, false)) {
			if (self->chr == '"') {
				array_insert(self->tokens, self->tokens->length,
							 lexerToken_new(string, LEXERTOKENS_STRING,
											startChrIndex, self->chrIndex,
											self->lineNum));
				return;
			}

			if (includeChr) {
				stringConcatenateChr(string, lexer_escapeChr(self));
				includeChr = false;
			} else if (self->chr == '\\') {
				includeChr = true;
			} else {
				stringConcatenateChr(string, self->chr);
			}
		}

		stringConcatenateChr(string, '\n');
	}

	lexer_error(
		self, "unterminated string",
		lexerToken_new("", LEXERTOKENS_NONE,
					   self->lineNum == startLineNum ? startChrIndex : 0,
					   self->chrIndex, self->lineNum));
}

/**
 * Lexes a float.
 *
 * @param self          The current lexer struct.
 * @param startChrIndex The index of the first character of
 * the float.
 * @param number        The float's value.
 */
void lexer_lexFloat(struct Lexer *self, size_t startChrIndex, char *number) {
	while (lexer_getChr(self, false)) {
		if (isspace(self->chr) || (self->chr != '.' && !isalpha(self->chr))) {
			lexer_unGetChr(self);
			break;
		}

		if (self->chr == '.') {
			lexer_error(self, "too many decimal points for float", NULL);
		} else if (!isdigit(self->chr)) {
			lexer_error(self, "invalid character for float", NULL);
		}

		stringConcatenateChr(number, self->chr);
	}

	array_insert(self->tokens, self->tokens->length,
				 lexerToken_new(number, LEXERTOKENS_FLOAT, startChrIndex,
								self->chrIndex, self->lineNum));
}

/**
 * Lexes an integer. Hands control over to lexFloat() if
 * needed.
 *
 * @param self The current lexer struct.
 */
void lexer_lexInteger(struct Lexer *self) {
	size_t startChrIndex = self->chrIndex;
	char *number = chrToString(self->chr);

	while (lexer_getChr(self, false)) {
		if (isspace(self->chr) || (self->chr != '.' && !isalpha(self->chr))) {
			lexer_unGetChr(self);
			break;
		}

		stringConcatenateChr(number, self->chr);

		if (self->chr == '.') {
			lexer_lexFloat(self, startChrIndex, number);
			return;
		} else if (!isdigit(self->chr)) {
			lexer_error(self, "invalid character for float", NULL);
		}
	}

	array_insert(self->tokens, self->tokens->length,
				 lexerToken_new(number, LEXERTOKENS_INTEGER, startChrIndex,
								self->chrIndex, self->lineNum));
}

bool lexer_lexKeywordOrIdentifier_matcher(const void *element,
										  const void *matchValue) {
	return strcmp((const char *)element, matchValue) == 0;
}

/**
 * Lexes a keyword or identifier.
 *
 * @param self The current lexer struct.
 */
void lexer_lexKeywordOrIdentifier(struct Lexer *self) {
	size_t startChrIndex = self->chrIndex;
	char *string = chrToString(self->chr);

	while (lexer_getChr(self, false)) {
		if (!isalnum(self->chr)) {
			lexer_unGetChr(self);
			break;
		}

		stringConcatenateChr(string, self->chr);
	}

	array_insert(
		self->tokens, self->tokens->length,
		lexerToken_new(
			string,
			array_find(&KEYWORDS, &lexer_lexKeywordOrIdentifier_matcher, string)
				? LEXERTOKENS_IDENTIFIER
				: LEXERTOKENS_KEYWORD,
			startChrIndex,
			self->chr == '\n' ? self->chrIndex - 1 : self->chrIndex,
			self->lineNum));
}

/**
 * Lexes '#='.
 *
 * @param self The current lexer struct.
 */
void lexer_lexMultiLineComment(struct Lexer *self, size_t startChrIndex) {
	bool end;
	size_t startLineNum = self->lineNum;

	while (lexer_getLine(self, false)) {
		end = false;

		while (lexer_getChr(self, true)) {
			if (end) {
				if (self->chr == '#') {
					return;
				}

				end = false;
			} else if (self->chr == '=') {
				end = true;
			}
		}
	}

	lexer_error(
		self, "unterminated multi-line comment",
		lexerToken_new("", LEXERTOKENS_NONE,
					   self->lineNum == startLineNum ? startChrIndex : 0,
					   self->chrIndex, self->lineNum));
}

/**
 * Lexes '#'.
 *
 * @param self The current lexer struct.
 */
void lexer_lexSingleLineComment(struct Lexer *self) {
	size_t startChrIndex = self->chrIndex - 1;

	if (lexer_getChr(self, false)) {
		if (self->chr == '=') {
			lexer_lexMultiLineComment(self, startChrIndex);
		} else {
			while (lexer_getChr(self, true)) {
			}
		}
	}

	array_insert(self->tokens, self->tokens->length,
				 lexerToken_new("", LEXERTOKENS_COMMENT, startChrIndex,
								self->chrIndex, self->lineNum));
}

/**
 * Lexes '%'.
 *
 * @param self The current lexer struct.
 */
void lexer_lexModulo(struct Lexer *self) {
	const struct LexerToken *token = NULL;

	if (lexer_checkForTrailingChr(self, '=')) {
		token =
			lexerToken_new("%=", LEXERTOKENS_MODULO_ASSIGNMENT,
						   self->chrIndex - 1, self->chrIndex, self->lineNum);
	} else {
		token = lexerToken_new("%", LEXERTOKENS_MODULO, self->chrIndex,
							   self->chrIndex, self->lineNum);
	}

	array_insert(self->tokens, self->tokens->length, token);
	lexer_checkForContinuation(self, token->value);
}

/**
 * Lexes '*'.
 *
 * @param self The current lexer struct.
 */
void lexer_lexMultiplication(struct Lexer *self) {
	const struct LexerToken *token = NULL;

	if (lexer_checkForTrailingChr(self, self->chr)) {
		if (lexer_checkForTrailingChr(self, '=')) {
			token = lexerToken_new("**=", LEXERTOKENS_EXPONENT_ASSIGNMENT,
								   self->chrIndex - 2, self->chrIndex,
								   self->lineNum);
		} else {
			token =
				lexerToken_new("**", LEXERTOKENS_EXPONENT, self->chrIndex - 1,
							   self->chrIndex, self->lineNum);
		}
	} else {
		if (lexer_checkForTrailingChr(self, '=')) {
			token = lexerToken_new("*=", LEXERTOKENS_MULTIPLICATION_ASSIGNMENT,
								   self->chrIndex, self->chrIndex - 1,
								   self->lineNum);

		} else {
			token =
				lexerToken_new("*", LEXERTOKENS_MULTIPLICATION, self->chrIndex,
							   self->chrIndex, self->lineNum);
		}
	}

	array_insert(self->tokens, self->tokens->length, token);
	lexer_checkForContinuation(self, token->value);
}

/**
 * Lexes '/'.
 *
 * @param self The current lexer struct.
 */
void lexer_lexDivision(struct Lexer *self) {
	const struct LexerToken *token = NULL;

	if (lexer_checkForTrailingChr(self, self->chr)) {
		if (lexer_checkForTrailingChr(self, '=')) {
			token = lexerToken_new("//=", LEXERTOKENS_FLOOR_DIVISION_ASSIGNMENT,
								   self->chrIndex - 2, self->chrIndex,
								   self->lineNum);
		} else {
			token = lexerToken_new("//", LEXERTOKENS_FLOOR_DIVISION,
								   self->chrIndex - 1, self->chrIndex,
								   self->lineNum);
		}
	} else {
		if (lexer_checkForTrailingChr(self, '=')) {
			token = lexerToken_new("/=", LEXERTOKENS_DIVISION_ASSIGNMENT,
								   self->chrIndex, self->chrIndex - 1,
								   self->lineNum);

		} else {
			token = lexerToken_new("/", LEXERTOKENS_DIVISION, self->chrIndex,
								   self->chrIndex, self->lineNum);
		}
	}

	array_insert(self->tokens, self->tokens->length, token);
	lexer_checkForContinuation(self, token->value);
}

/**
 * Lexes '+'.
 *
 * @param self The current lexer struct.
 */
void lexer_lexAddition(struct Lexer *self) {
	const struct LexerToken *token = NULL;

	if (lexer_checkForTrailingChr(self, '=')) {
		token = lexerToken_new("+=", LEXERTOKENS_ADDITION_ASSIGNMENT,
							   self->chrIndex, self->chrIndex, self->lineNum);
	} else {
		token = lexerToken_new("+", LEXERTOKENS_ADDITION, self->chrIndex,
							   self->chrIndex, self->lineNum);
	}

	array_insert(self->tokens, self->tokens->length, token);
	lexer_checkForContinuation(self, token->value);
}

/**
 * Lexes '-'.
 *
 * @param self The current lexer struct.
 */
void lexer_lexSubtraction(struct Lexer *self) {
	const struct LexerToken *token = NULL;

	if (lexer_checkForTrailingChr(self, '=')) {
		token = lexerToken_new("-=", LEXERTOKENS_SUBTRACTION_ASSIGNMENT,
							   self->chrIndex, self->chrIndex, self->lineNum);
	} else {
		if (lexer_checkForTrailingChr(self, '>')) {
			token = lexerToken_new("->", LEXERTOKENS_ARROW, self->chrIndex,
								   self->chrIndex, self->lineNum);
		} else {
			token = lexerToken_new("-", LEXERTOKENS_SUBTRACTION, self->chrIndex,
								   self->chrIndex, self->lineNum);
		}
	}

	array_insert(self->tokens, self->tokens->length, token);
	lexer_checkForContinuation(self, token->value);
}

/**
 * Lexes '>'.
 *
 * @param self The current lexer struct.
 */
void lexer_lexGreaterThan(struct Lexer *self) {
	const struct LexerToken *token = NULL;

	if (lexer_checkForTrailingChr(self, self->chr)) {
		if (lexer_checkForTrailingChr(self, '=')) {
			token = lexerToken_new(
				">>=", LEXERTOKENS_BITWISE_RIGHT_SHIFT_ASSIGNMENT,
				self->chrIndex - 2, self->chrIndex, self->lineNum);
		} else {
			token = lexerToken_new(">>", LEXERTOKENS_BITWISE_RIGHT_SHIFT,
								   self->chrIndex - 1, self->chrIndex,
								   self->lineNum);
		}
	} else {
		if (lexer_checkForTrailingChr(self, '=')) {
			token = lexerToken_new(">=", LEXERTOKENS_GREATER_THAN_OR_EQUAL,
								   self->chrIndex - 1, self->chrIndex,
								   self->lineNum);
		} else {
			token =
				lexerToken_new(">", LEXERTOKENS_GREATER_THAN, self->chrIndex,
							   self->chrIndex, self->lineNum);
		}
	}

	array_insert(self->tokens, self->tokens->length, token);
	lexer_checkForContinuation(self, token->value);
}

/**
 * Lexes '<'.
 *
 * @param self The current lexer struct.
 */
void lexer_lexLessThan(struct Lexer *self) {
	const struct LexerToken *token = NULL;

	if (lexer_checkForTrailingChr(self, self->chr)) {
		if (lexer_checkForTrailingChr(self, '=')) {
			token = lexerToken_new(
				"<<=", LEXERTOKENS_BITWISE_LEFT_SHIFT_ASSIGNMENT,
				self->chrIndex - 2, self->chrIndex, self->lineNum);
		} else {
			token = lexerToken_new("<<", LEXERTOKENS_BITWISE_LEFT_SHIFT,
								   self->chrIndex - 1, self->chrIndex,
								   self->lineNum);
		}
	} else {
		if (lexer_checkForTrailingChr(self, '=')) {
			token = lexerToken_new("<=", LEXERTOKENS_LESS_THAN_OR_EQUAL,
								   self->chrIndex - 1, self->chrIndex,
								   self->lineNum);
		} else {
			token = lexerToken_new("<", LEXERTOKENS_LESS_THAN, self->chrIndex,
								   self->chrIndex, self->lineNum);
		}
	}

	array_insert(self->tokens, self->tokens->length, token);
	lexer_checkForContinuation(self, token->value);
}

/**
 * Lexes '!'.
 *
 * @param self The current lexer struct.
 */
void lexer_lexLogicalNot(struct Lexer *self) {
	const struct LexerToken *token = NULL;

	if (lexer_checkForTrailingChr(self, '=')) {
		token = lexerToken_new("!=", LEXERTOKENS_NOT_EQUAL_TO, self->chrIndex,
							   self->chrIndex, self->lineNum);
	} else {
		token = lexerToken_new("!", LEXERTOKENS_LOGICAL_NOT, self->chrIndex,
							   self->chrIndex, self->lineNum);
	}

	array_insert(self->tokens, self->tokens->length, token);
	lexer_checkForContinuation(self, token->value);
}

/**
 * Lexes '&'.
 *
 * @param self The current lexer struct.
 */
void lexer_lexBitwiseAND(struct Lexer *self) {
	const struct LexerToken *token = NULL;

	if (lexer_checkForTrailingChr(self, self->chr)) {
		token =
			lexerToken_new("&&", LEXERTOKENS_LOGICAL_AND, self->chrIndex - 1,
						   self->chrIndex, self->lineNum);
	} else {
		if (lexer_checkForTrailingChr(self, '=')) {
			token = lexerToken_new("&=", LEXERTOKENS_BITWISE_AND_ASSIGNMENT,
								   self->chrIndex - 1, self->chrIndex,
								   self->lineNum);
		} else {
			token = lexerToken_new("&", LEXERTOKENS_BITWISE_AND, self->chrIndex,
								   self->chrIndex, self->lineNum);
		}
	}

	array_insert(self->tokens, self->tokens->length, token);
	lexer_checkForContinuation(self, token->value);
}

/**
 * Lexes '|'.
 *
 * @param self The current lexer struct.
 */
void lexer_lexBitwiseOR(struct Lexer *self) {
	const struct LexerToken *token = NULL;

	if (lexer_checkForTrailingChr(self, self->chr)) {
		token = lexerToken_new("||", LEXERTOKENS_LOGICAL_OR, self->chrIndex - 1,
							   self->chrIndex, self->lineNum);
	} else {
		if (lexer_checkForTrailingChr(self, '=')) {
			token = lexerToken_new("|=", LEXERTOKENS_BITWISE_OR_ASSIGNMENT,
								   self->chrIndex - 1, self->chrIndex,
								   self->lineNum);
		} else {
			token = lexerToken_new("|", LEXERTOKENS_BITWISE_OR, self->chrIndex,
								   self->chrIndex, self->lineNum);
		}
	}

	array_insert(self->tokens, self->tokens->length, token);
	lexer_checkForContinuation(self, token->value);
}

/**
 * Lexes '^'.
 *
 * @param self The current lexer struct.
 */
void lexer_lexBitwiseXOR(struct Lexer *self) {
	const struct LexerToken *token = NULL;

	if (lexer_checkForTrailingChr(self, '=')) {
		token = lexerToken_new("^=", LEXERTOKENS_BITWISE_XOR_ASSIGNMENT,
							   self->chrIndex, self->chrIndex, self->lineNum);
	} else {
		token = lexerToken_new("^", LEXERTOKENS_BITWISE_XOR, self->chrIndex,
							   self->chrIndex, self->lineNum);
	}

	array_insert(self->tokens, self->tokens->length, token);
	lexer_checkForContinuation(self, token->value);
}

/**
 * Lexes '~'.
 *
 * @param self The current lexer struct.
 */
void lexer_lexBitwiseNOT(struct Lexer *self) {
	const struct LexerToken *token = NULL;

	if (lexer_checkForTrailingChr(self, '=')) {
		token = lexerToken_new("~=", LEXERTOKENS_BITWISE_NOT_ASSIGNMENT,
							   self->chrIndex, self->chrIndex, self->lineNum);
	} else {
		token = lexerToken_new("~", LEXERTOKENS_BITWISE_NOT, self->chrIndex,
							   self->chrIndex, self->lineNum);
	}

	array_insert(self->tokens, self->tokens->length, token);
	lexer_checkForContinuation(self, token->value);
}

/**
 * Lexes ':'.
 *
 * @param self The current lexer struct.
 */
void lexer_lexColon(struct Lexer *self) {
	const struct LexerToken *token = NULL;

	if (lexer_checkForTrailingChr(self, ':')) {
		token = lexerToken_new("::", LEXERTOKENS_SCOPE_RESOLUTION,
							   self->chrIndex, self->chrIndex, self->lineNum);
	} else {
		token = lexerToken_new(":", LEXERTOKENS_COLON, self->chrIndex,
							   self->chrIndex, self->lineNum);
	}

	array_insert(self->tokens, self->tokens->length, token);
	lexer_checkForContinuation(self, token->value);
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
	case '=':
		lexer_lexEquals(self);
		break;
	case '(':
		array_insert(self->tokens, self->tokens->length,
					 lexerToken_new("", LEXERTOKENS_OPEN_BRACE, self->chrIndex,
									self->chrIndex, self->lineNum));
		break;
	case '[':
		array_insert(self->tokens, self->tokens->length,
					 lexerToken_new("", LEXERTOKENS_OPEN_SQUARE_BRACE,
									self->chrIndex, self->chrIndex,
									self->lineNum));
		break;
	case '{':
		array_insert(self->tokens, self->tokens->length,
					 lexerToken_new("", LEXERTOKENS_OPEN_CURLY_BRACE,
									self->chrIndex, self->chrIndex,
									self->lineNum));
		break;
	case ')':
		array_insert(self->tokens, self->tokens->length,
					 lexerToken_new("", LEXERTOKENS_CLOSE_BRACE, self->chrIndex,
									self->chrIndex, self->lineNum));
		break;
	case ']':
		array_insert(self->tokens, self->tokens->length,
					 lexerToken_new("", LEXERTOKENS_CLOSE_SQUARE_BRACE,
									self->chrIndex, self->chrIndex,
									self->lineNum));
		break;
	case '}':
		array_insert(self->tokens, self->tokens->length,
					 lexerToken_new("", LEXERTOKENS_CLOSE_CURLY_BRACE,
									self->chrIndex, self->chrIndex,
									self->lineNum));
		break;
	case '.':
		array_insert(self->tokens, self->tokens->length,
					 lexerToken_new("", LEXERTOKENS_DOT, self->chrIndex,
									self->chrIndex, self->lineNum));
		break;
	case '@':
		array_insert(self->tokens, self->tokens->length,
					 lexerToken_new("", LEXERTOKENS_AT, self->chrIndex,
									self->chrIndex, self->lineNum));
		break;
	case '#':
		lexer_lexSingleLineComment(self);
		break;
	case '%':
		lexer_lexModulo(self);
		break;
	case '*':
		lexer_lexMultiplication(self);
		break;
	case '/':
		lexer_lexDivision(self);
		break;
	case '+':
		lexer_lexAddition(self);
		break;
	case '-':
		lexer_lexSubtraction(self);
		break;
	case '>':
		lexer_lexGreaterThan(self);
		break;
	case '<':
		lexer_lexLessThan(self);
		break;
	case '!':
		lexer_lexLogicalNot(self);
		break;
	case '&':
		lexer_lexBitwiseAND(self);
		break;
	case '|':
		lexer_lexBitwiseOR(self);
		break;
	case '^':
		lexer_lexBitwiseXOR(self);
		break;
	case '~':
		lexer_lexBitwiseNOT(self);
		break;
	case ',':
		array_insert(self->tokens, self->tokens->length,
					 lexerToken_new("", LEXERTOKENS_COMMA, self->chrIndex,
									self->chrIndex, self->lineNum));
		break;
	case ':':
		lexer_lexColon(self);
		break;
	default:
		if (isalpha(self->chr) || self->chr == '_') {
			lexer_lexKeywordOrIdentifier(self);
		} else if (isdigit(self->chr)) {
			lexer_lexInteger(self);
		} else {
			lexer_error(self, "unknown character", NULL);
		}

		break;
	}

	return true;
}

/**
 * Clears the tokens list.
 *
 * @param self The current lexer struct.
 */
void lexer_clearTokens(struct Lexer *self) { array_clear(self->tokens); }

/**
 * Gets the next character and lexes it.
 *
 * @param self        The current lexer struct.
 * @param mustLexChar Whether a character must be lexed.
 * @param nextLine    Whether the character can be on the next line.
 *
 * @return bool Whether lexing succeeded.
 */
bool lexer_lex(struct Lexer *self, bool mustLexChar, bool nextLine) {
	if (mustLexChar) {
		while (true) {
			while (!lexer_getChr(self, true)) {
				if (!nextLine || !lexer_getLine(self, nextLine)) {
					return false;
				}
			}

			if (lexer_lexNext(self)) {
				return true;
			}
		}
	} else {
		while (!lexer_getChr(self, true)) {
			if (!lexer_getLine(self, nextLine)) {
				return false;
			}
		}
	}

	return lexer_lexNext(self);
}

/**
 * Un-lexes the last token.
 *
 * @param self The current lexer struct.
 */
void lexer_unLex(struct Lexer *self) { self->parsedTokensCount++; }

/**
 * Retreives the last token. Respects un-lexes.
 *
 * @param self The current lexer struct.
 *
 * @return The retreived token.
 */
const struct LexerToken *lexer_getToken(struct Lexer *self) {
	if (self->parsedTokensCount > 0) {
		return self->tokens
			->_values[self->tokens->length - 1 - (self->parsedTokensCount--)];
	}

	return self->tokens->_values[self->tokens->length - 1];
}
