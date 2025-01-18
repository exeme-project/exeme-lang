/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include "../includes.c"

#include "../errors.c"
#include "../utils/array.c"
#include "../utils/conversions.c"
#include "../utils/panic.c"
#include "../utils/string.c"
#include "./tokens.c"

/**
 * Used to identify keywords.
 */
static const struct Array KEYWORDS = array_new_stack("break", "case", "cls", "else", "elif", "enum", "export", "for", "func",
													 "if", "import", "match", "pass", "return", "struct", "use", "while");

/**
 * Represents a lexer.
 */
struct Lexer {
	bool nextLine;
	char chr, prevChr;
	const char *FILE_PATH;
	FILE *filePointer;
	int chrStatus;
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
	self->lineIndex = negativeULL; // Will wrap around when a line is got
	self->tokenUnlexes = 0;
	self->tokens = array_new();

	if (!self->filePointer ||		 // Only POSIX requires errno is set, and so for
									 // other platforms we have to check for NULL
		ferror(self->filePointer)) { // Probably file doesn't exist
		error(stringConcatenate("failed to open file '", self->FILE_PATH, "'"));
	}

	return self;
}

/**
 * Frees a Lexer struct.
 *
 * @param self The current Lexer struct.
 */
void lexer_free(struct Lexer **self) {
	if (self && *self) {
		if ((*self)->filePointer) {
			fclose((*self)->filePointer);
		}

		array_free(&(*self)->tokens);

		free(*self);
		*self = NULL;
	} else {
		panic("Lexer struct has already been freed");
	}
}

/**
 * Prints a lexing error and exits.
 *
 * @param self             The current Lexer struct.
 * @param ERROR_MSG_NUMBER The error message number.
 * @param ERROR_MSG        The error message.
 * @param TOKEN            The erroneous token.
 */
__attribute__((noreturn)) void lexer_error(struct Lexer *self, const enum ErrorIdentifiers ERROR_MSG_NUMBER,
										   const char *ERROR_MSG, const struct LexerToken *TOKEN) {
	FILE *filePointer = fopen(self->FILE_PATH, "r");
	struct String *line = string_new("\0", true);

	size_t lineIndex = 0;

	while (true) {
		char chr = (char)fgetc(filePointer); // Get a char

		if (chr == '\n' || chr == EOF) {		  // EOL (or EOF)
			if (self->lineIndex == lineIndex++) { // If we have been copying the line we want
				break;
			}

			// string_clear(line);
			// I believe the above line is not needed. Keeping it here in case I'm mistaken.
			// Same as in the parser_error function.
		} else if (lineIndex == self->lineIndex) { // If this is the line we want
			string_appendChr(line, chr);
		}
	}

	const char *lineNumberString = ulToString(self->lineIndex + 1);
	size_t lineNumberStringLength = strlen(lineNumberString);

	printf("-%s> %s\n%s | %s\n%s", repeatChr('-', lineNumberStringLength), self->FILE_PATH, lineNumberString, line->_value,
		   repeatChr(' ', lineNumberStringLength + 3));

	if (TOKEN) { // We know what token the error occurred on, and telling the user it will help them
		printf("%s%s ", repeatChr(' ', TOKEN->startChrIndex), repeatChr('^', TOKEN->endChrIndex - TOKEN->startChrIndex + 1));
	} else { // We don't know what token the error occurred on
		printf("%s^ ", repeatChr(' ', self->chrIndex));
	}

	printf("%serror[%s]:%s %s\n", F_BRIGHT_RED, error_get(ERROR_MSG_NUMBER), S_RESET, ERROR_MSG);

	exit(EXIT_FAILURE);
}

/**
 * Gets the next line.
 *
 * @param self     The current Lexer struct.
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

	self->chrIndex = negativeULL; // Will wrap around when a char is got
	self->lineIndex++;

	self->nextLine = false;

	return true;
}

/**
 * Un-gets the current character.
 *
 * @param self The current Lexer struct.
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

	if (ferror(self->filePointer)) {
		fclose(self->filePointer);
		self->filePointer = NULL; // Set to NULL to prevent future errors

		return false;
	}

	return true;
}

/**
 * Gets the next char.
 *
 * @param self           The current Lexer struct.
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
		self->chrStatus = fgetc(self->filePointer); // Specify cast to char to silence warnings
		self->chr = (char)self->chrStatus;
		self->chrIndex++;

		if (self->chrStatus == EOF || ferror(self->filePointer) || self->chr == '\n') { // EOF / error or EOL
			if (self->chrStatus == EOF || ferror(self->filePointer)) {					// EOF / error
				fclose(self->filePointer);
				self->filePointer = NULL; // Set to NULL to prevent future errors
			}

			// Doing the following before checking for an EOL would break the
			// code
			self->chr = self->prevChr;
			self->prevChr = prevChr;
			self->nextLine = true;
			self->chrIndex--;

			return false;
		}

		if (skipWhitespace) {		   // Keep going till we encounter a char that is not
									   // whitespace
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
 * @param self  The current Lexer struct.
 * @param token The current token.
 */
void lexer_checkForContinuation(struct Lexer *self, const struct LexerToken *token) {
	if (lexer_getChr(self, false)) {
		if (!isspace(self->chr) && !isalnum(self->chr)) {
			lexer_error(self, L0002, stringConcatenate("unexpected continuation of token '", token->value->_value, "'"),
						lexerToken_new(LEXERTOKENS_NONE, string_new(chrToString(self->chr), true), self->chrIndex,
									   self->chrIndex, self->lineIndex));
		}

		lexer_unGetChr(self); // Token was not continued, un-get the current char
	}
}

/**
 * Creates a LexerToken for a one-char token.
 *
 * @param self       The current Lexer struct.
 * @param IDENTIFIER The current token's identifier.
 */
void lexer_lexOneChar(struct Lexer *self, const enum LexerTokenIdentifiers IDENTIFIER) {
	const struct LexerToken *token = lexerToken_new(IDENTIFIER, string_new(chrToString(self->chr), false), self->chrIndex,
													self->chrIndex, self->lineIndex);

	array_append(self->tokens, token);
}

/**
 * Creates a LexerToken for a possible two-char token.
 *
 * @param self       The current Lexer struct.
 * @param SECOND_CHR The second char to check for.
 * @param IF_ONE     If the token is one char.
 * @param IF_TWO     If the token is two chars.
 */
void lexer_lexTwoChar(struct Lexer *self, const char SECOND_CHR, const enum LexerTokenIdentifiers IF_ONE,
					  const enum LexerTokenIdentifiers IF_TWO) {
	const struct LexerToken *token = NULL;

	if (lexer_getChr(self, false)) {
		if (self->chr == SECOND_CHR) {
			token = lexerToken_new(IF_TWO,
								   string_new(stringConcatenate(chrToString(self->prevChr), chrToString(self->chr)), false),
								   self->chrIndex - 1, self->chrIndex, self->lineIndex);
		} else { // SECOND_CHR was not found, un-get it
			lexer_unGetChr(self);
		}
	}

	if (!token) { // SECOND_CHR was not found
		token = lexerToken_new(IF_ONE, string_new(chrToString(self->chr), false), self->chrIndex, self->chrIndex,
							   self->lineIndex);
	}

	array_append(self->tokens, token);
	lexer_checkForContinuation(self, token);
}

/**
 * Creates a LexerToken for a possible two-char token with two possible second
 * chars.
 *
 * @param self             The current Lexer struct.
 * @param SECOND_CHR       The second char to check for.
 * @param OTHER_SECOND_CHR The other second char to check for.
 * @param IF_ONE           If the token is one char.
 * @param IF_TWO           If the token is two chars with SECOND_CHR.
 * @param IF_OTHER_TWO     If the token is two chars with OTHER_SECOND_CHR.
 */
void lexer_lex2TwoChar(struct Lexer *self, const char SECOND_CHR, const char OTHER_SECOND_CHR,
					   const enum LexerTokenIdentifiers IF_ONE, const enum LexerTokenIdentifiers IF_TWO,
					   const enum LexerTokenIdentifiers IF_OTHER_TWO) {
	const struct LexerToken *token = NULL;

	if (lexer_getChr(self, false)) {
		if (self->chr == SECOND_CHR) {
			token = lexerToken_new(IF_TWO,
								   string_new(stringConcatenate(chrToString(self->prevChr), chrToString(self->chr)), false),
								   self->chrIndex - 1, self->chrIndex, self->lineIndex);
		} else if (self->chr == OTHER_SECOND_CHR) {
			token = lexerToken_new(IF_OTHER_TWO,
								   string_new(stringConcatenate(chrToString(self->prevChr), chrToString(self->chr)), false),
								   self->chrIndex - 1, self->chrIndex, self->lineIndex);
		} else {
			lexer_unGetChr(self);
		}
	}

	if (!token) {
		token = lexerToken_new(IF_ONE, string_new(chrToString(self->chr), false), self->chrIndex, self->chrIndex,
							   self->lineIndex);
	}

	array_append(self->tokens, token);
	lexer_checkForContinuation(self, token);
}

/**
 * Creates a LexerToken for a possible three-char token.
 *
 * @param self           The current Lexer struct.
 * @param SECOND_CHR     The second char to check for.
 * @param THIRD_CHR      The third char to check for.
 * @param IF_ONE         If the token is one char.
 * @param IF_TWO         If the token is two SECOND_CHRs.
 * @param IF_ONE_AND_ONE If the token is SECOND_CHR and THIRD_CHR.
 * @param IF_TWO_AND_ONE If the token is two of SECOND_CHR and THIRD_CHR.
 */
void lexer_lexThreeChar(struct Lexer *self, const char SECOND_CHR, const char THIRD_CHR,
						const enum LexerTokenIdentifiers IF_ONE, const enum LexerTokenIdentifiers IF_TWO,
						const enum LexerTokenIdentifiers IF_ONE_AND_ONE, const enum LexerTokenIdentifiers IF_TWO_AND_ONE) {
	const struct LexerToken *token = NULL;

	if (lexer_getChr(self, false)) {
		if (self->chr == SECOND_CHR) {
			const char prevChr = self->prevChr;

			if (IF_TWO_AND_ONE && lexer_getChr(self, false)) {
				if (self->chr == THIRD_CHR) {
					token = lexerToken_new(IF_TWO_AND_ONE,
										   string_new(stringConcatenate(chrToString(prevChr), chrToString(self->prevChr),
																		chrToString(self->chr)),
													  false),
										   self->chrIndex - 2, self->chrIndex, self->lineIndex);
				}
			}

			if (!token) { // THIRD_CHR was not found
				token = lexerToken_new(
					IF_TWO, string_new(stringConcatenate(chrToString(self->prevChr), chrToString(self->chr)), false),
					self->chrIndex - 1, self->chrIndex, self->lineIndex);
			}
		} else if (self->chr == THIRD_CHR) {
			token = lexerToken_new(IF_ONE_AND_ONE,
								   string_new(stringConcatenate(chrToString(self->prevChr), chrToString(self->chr)), false),
								   self->chrIndex - 1, self->chrIndex, self->lineIndex);
		} else { // SECOND_CHR was not found, un-get it
			lexer_unGetChr(self);
		}
	}

	if (!token) { // SECOND_CHR was not found
		token = lexerToken_new(IF_ONE, string_new(chrToString(self->chr), false), self->chrIndex, self->chrIndex,
							   self->lineIndex);
	}

	array_append(self->tokens, token);
	lexer_checkForContinuation(self, token);
}

/**
 * Escapes the current character in the lexer struct.
 *
 * @param self          The current Lexer struct.
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
	default:
		lexer_error(
			self, L0004, "invalid escape sequence",
			lexerToken_new(LEXERTOKENS_NONE, string_new("\0", true), startChrIndex, self->chrIndex, self->lineIndex));
	}
}

/**
 * Creates a LexerToken for a char.
 *
 * @param self The current Lexer struct.
 */
void lexer_lexChr(struct Lexer *self) {
	const size_t startChrIndex = self->chrIndex;
	size_t escapeChrIndex = negativeULL;
	struct String *chr = string_new("\0", true);

	while (lexer_getChr(self, false)) {
		if (self->chr == '\'') {
			array_append(self->tokens, lexerToken_new(LEXERTOKENS_CHR, chr, startChrIndex, self->chrIndex, self->lineIndex));
			return;
		} else if (chr->length == 1) {
			lexer_error(self, L0005, "multi-character char literal",
						lexerToken_new(LEXERTOKENS_NONE, string_new("\0", true), startChrIndex + 1, self->chrIndex,
									   self->lineIndex));
		}

		if (escapeChrIndex != negativeULL) {
			string_appendChr(chr, lexer_escapeChr(self, escapeChrIndex));
			escapeChrIndex = negativeULL;
		} else if (self->chr == '\\') {
			escapeChrIndex = self->chrIndex;
		} else {
			string_appendChr(chr, self->chr);
		}
	}

	lexer_error(self, L0003, "unterminated character literal",
				lexerToken_new(LEXERTOKENS_NONE, chr, startChrIndex, self->chrIndex, self->lineIndex));
}

/**
 * Creates a LexerToken for a string.
 *
 * @param self The current Lexer struct.
 */
void lexer_lexString(struct Lexer *self) {
	const size_t startChrIndex = self->chrIndex, startLineIndex = self->lineIndex;
	size_t escapeChrIndex = negativeULL;
	struct String *string = string_new("\0", true);

	while (lexer_getLine(self, false)) {
		while (lexer_getChr(self, false)) {
			if (self->chr == '"') {
				array_append(self->tokens,
							 lexerToken_new(LEXERTOKENS_STRING, string, startChrIndex, self->chrIndex, self->lineIndex));
				return;
			}

			if (escapeChrIndex != negativeULL) {
				string_appendChr(string, lexer_escapeChr(self, escapeChrIndex));
				escapeChrIndex = negativeULL;
			} else if (self->chr == '\\') {
				escapeChrIndex = self->chrIndex;
			} else {
				string_appendChr(string, self->chr);
			}
		}

		string_appendChr(string, '\n');
	}

	lexer_error(self, L0003, "unterminated string literal",
				lexerToken_new(LEXERTOKENS_STRING, string, self->lineIndex == startLineIndex ? startChrIndex : 0,
							   self->chrIndex, self->lineIndex));
}

/**
 * Creates a LexerToken for a multi-line comment.
 *
 * @param self          The current Lexer struct.
 * @param startChrIndex The start char index of the comment.
 */
void lexer_lexMultiLineComment(struct Lexer *self, const size_t startChrIndex) {
	const size_t startLineIndex = self->lineIndex;

	while (lexer_getLine(self, false)) {
		while (lexer_getChr(self, true)) {
			if (self->chr == '=') {
				if (lexer_getChr(self, false)) {
					if (self->chr == ';') {
						array_append(self->tokens, lexerToken_new(LEXERTOKENS_MULTI_LINE_COMMENT, string_new("\0", true),
																  self->lineIndex == startLineIndex ? startChrIndex : 0,
																  self->chrIndex, startLineIndex));
						return;
					}
				}
			}
		}
	}

	lexer_error(self, L0003, "unterminated multi-line comment",
				lexerToken_new(LEXERTOKENS_NONE, string_new("\0", true),
							   self->lineIndex == startLineIndex ? startChrIndex : 0, self->chrIndex, self->lineIndex));
}

/**
 * Creates a LexerToken for a single-line comment.
 *
 * @param self The current Lexer struct.
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

	array_append(self->tokens, lexerToken_new(LEXERTOKENS_SINGLE_LINE_COMMENT, string_new("\0", true), startChrIndex,
											  self->chrIndex, self->lineIndex));
}

/**
 * Creates a LexerToken for a keyword or identifier.
 *
 * @param self The current Lexer struct.
 */
void lexer_lexKeywordOrIdentifier(struct Lexer *self) {
	const size_t startChrIndex = self->chrIndex;
	struct String *identifier = string_new(chrToString(self->chr), false);

	while (lexer_getChr(self, false)) {
		if (!isalnum(self->chr)) {
			lexer_unGetChr(self);
			break;
		}

		string_appendChr(identifier, self->chr);
	}

	array_append(self->tokens,
				 lexerToken_new(array_contains((struct Array *)&KEYWORDS, &array___match_string, identifier->_value)
									? LEXERTOKENS_KEYWORD
									: LEXERTOKENS_IDENTIFIER,
								identifier, startChrIndex, self->chrIndex, self->lineIndex));
}

/**
 * Creates a LexerToken for an integer / float.
 *
 * @param self The current Lexer struct.
 */
void lexer_lexNumber(struct Lexer *self) {
	bool isFloat = false;
	size_t startChrIndex = self->chrIndex;
	struct String *number = string_new(chrToString(self->chr), false);

	while (lexer_getChr(self, false)) {
		if (isspace(self->chr)) {
			break;
		} else if (isalpha(self->chr)) {
			lexer_error(self, L0006, stringConcatenate("invalid character for ", isFloat ? "float" : "integer"),
						lexerToken_new(LEXERTOKENS_NONE, number, self->chrIndex, self->chrIndex, self->lineIndex));
		} else if (self->chr == '.') {
			if (isFloat) {
				lexer_error(self, L0007, "too many decimal points for float",
							lexerToken_new(LEXERTOKENS_NONE, number, self->chrIndex, self->chrIndex, self->lineIndex));
			} else {
				isFloat = true;
			}
		} else if (!isdigit(self->chr)) {
			lexer_unGetChr(self);
			break;
		}

		string_appendChr(number, self->chr);
	}

	array_append(self->tokens, lexerToken_new(isFloat ? LEXERTOKENS_FLOAT : LEXERTOKENS_INTEGER, number, startChrIndex,
											  self->chrIndex, self->lineIndex));
}

/**
 * Calls the correct function for lexing the current character.
 *
 * @param self The current Lexer struct.
 *
 * @return Whether lexing succeeded.
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
		lexer_lexTwoChar(self, '=', LEXERTOKENS_MODULO, LEXERTOKENS_MODULO_ASSIGNMENT);
		break;
	case '*':
		lexer_lexThreeChar(self, self->chr, '=', LEXERTOKENS_MULTIPLICATION, LEXERTOKENS_EXPONENT,
						   LEXERTOKENS_MULTIPLICATION_ASSIGNMENT, LEXERTOKENS_EXPONENT_ASSIGNMENT);
		break;
	case '/':
		lexer_lexThreeChar(self, self->chr, '=', LEXERTOKENS_DIVISION, LEXERTOKENS_FLOOR_DIVISION,
						   LEXERTOKENS_DIVISION_ASSIGNMENT, LEXERTOKENS_FLOOR_DIVISION_ASSIGNMENT);
		break;
	case '+':
		lexer_lexTwoChar(self, '=', LEXERTOKENS_ADDITION, LEXERTOKENS_ADDITION_ASSIGNMENT);
		break;
	case '-':
		lexer_lex2TwoChar(self, '=', '>', LEXERTOKENS_SUBTRACTION, LEXERTOKENS_SUBTRACTION_ASSIGNMENT,
						  LEXERTOKENS_TYPE_ARROW);
		break;

	// Comparison / Relational operators
	case '=':
		lexer_lex2TwoChar(self, self->chr, '>', LEXERTOKENS_ASSIGNMENT, LEXERTOKENS_EQUAL_TO, LEXERTOKENS_ASSIGNMENT_ARROW);
		break;
	case '!':
		lexer_lexTwoChar(self, '=', LEXERTOKENS_LOGICAL_NOT, LEXERTOKENS_NOT_EQUAL_TO);
		break;
	case '>':
		lexer_lexThreeChar(self, self->chr, '=', LEXERTOKENS_GREATER_THAN, LEXERTOKENS_BITWISE_RIGHT_SHIFT,
						   LEXERTOKENS_GREATER_THAN_OR_EQUAL, LEXERTOKENS_BITWISE_RIGHT_SHIFT_ASSIGNMENT);
		break;
	case '<':
		lexer_lexThreeChar(self, self->chr, '=', LEXERTOKENS_LESS_THAN, LEXERTOKENS_BITWISE_LEFT_SHIFT,
						   LEXERTOKENS_LESS_THAN_OR_EQUAL, LEXERTOKENS_BITWISE_LEFT_SHIFT_ASSIGNMENT);
		break;

	// Logical operators
	case '&':
		lexer_lexThreeChar(self, self->chr, '=', LEXERTOKENS_BITWISE_AND, LEXERTOKENS_LOGICAL_AND,
						   LEXERTOKENS_BITWISE_AND_ASSIGNMENT, LEXERTOKENS_NONE);
		break;
	case '|':
		lexer_lexThreeChar(self, self->chr, '=', LEXERTOKENS_BITWISE_OR, LEXERTOKENS_LOGICAL_OR,
						   LEXERTOKENS_BITWISE_OR_ASSIGNMENT, LEXERTOKENS_NONE);
		break;

	// Bitwise operators
	case '^':
		lexer_lexTwoChar(self, '=', LEXERTOKENS_BITWISE_XOR, LEXERTOKENS_BITWISE_XOR_ASSIGNMENT);
		break;
	case '~':
		lexer_lexTwoChar(self, '=', LEXERTOKENS_BITWISE_NOT, LEXERTOKENS_BITWISE_NOT_ASSIGNMENT);
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
		lexer_lexTwoChar(self, self->chr, LEXERTOKENS_COLON, LEXERTOKENS_SCOPE_RESOLUTION);
		break;
	case ';':
		lexer_lexSingleLineComment(self);
		break;
	default:
		if (isalpha(self->chr) || self->chr == '_') {
			lexer_lexKeywordOrIdentifier(self);
		} else if (isdigit(self->chr)) {
			lexer_lexNumber(self);
		} else {
			lexer_error(self, L0001, "unknown character", NULL);
		}

		break;
	}

	return true;
}

/**
 * Gets the next char and lexes it.
 *
 * @param self     The current Lexer struct.
 * @param nextLine Whether the char can be on the next line.
 *
 * @return Whether lexing succeeded.
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
 * @param self The current Lexer struct.
 */
void lexer_unLex(struct Lexer *self) { self->tokenUnlexes++; }

/**
 * Retrieves the last token.
 *
 * @param self The current Lexer struct.
 *
 * @return The retrieved token.
 */
const struct LexerToken *lexer_getToken(struct Lexer *self, size_t *index) {
	if (self->tokens->length == 0) {
		return NULL;
	} else if (self->tokenUnlexes > 0) {
		*index = self->tokens->length - 1 - (self->tokenUnlexes--);

		return self->tokens->_values[*index]; // For future me, yes it does
											  // decrement
	}

	*index = self->tokens->length - 1;

	return self->tokens->_values[*index];
}
