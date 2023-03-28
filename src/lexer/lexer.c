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
	13,
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

/**
 * Represents a lexer token.
 */
struct LexerToken {
	enum LexerTokens identifier;
	size_t startChrIndex, chrIndex, lineIndex;
	const struct String *value;
};

#define LEXERTOKEN_STRUCT_SIZE sizeof(struct LexerToken)

/**
 * Creates a new LexerToken struct.
 *
 * @param identifier    Token identifier.
 * @param value         Value of the token.
 * @param startChrIndex Start char index of the token.
 * @param chrIndex      End char index of the token.
 * @param lineIndex     Line num of the token.
 *
 * @return The created LexerToken struct.
 */
const struct LexerToken *lexerToken_new(enum LexerTokens identifier,
										struct String *value,
										size_t startChrIndex, size_t chrIndex,
										size_t lineIndex) {
	struct LexerToken *self = malloc(LEXERTOKEN_STRUCT_SIZE);

	if (!self) {
		panic("failed to malloc LexerToken struct");
	}

	self->identifier = identifier;
	self->value = value;
	self->startChrIndex = startChrIndex;
	self->chrIndex = chrIndex;
	self->lineIndex = lineIndex;

	return self;
}

/**
 * Represents a lexer.
 */
struct Lexer {
	char chr, prevChr;
	const char *FILE_PATH;
	FILE *filePointer;
	size_t chrIndex, lineIndex;
	struct Array *tokens;
};

#define LEXER_STRUCT_SIZE sizeof(struct Lexer)

struct Lexer *lexer_new(const char *FILE_PATH) {
	struct Lexer *self = malloc(LEXER_STRUCT_SIZE);

	if (!self) {
		panic("failed to malloc Lexer struct");
	}

	self->chr = '\n';
	self->prevChr = '\n';
	self->FILE_PATH = FILE_PATH;
	self->filePointer = fopen(FILE_PATH, "r");
	self->chrIndex = 0;
	self->lineIndex = negativeIndex; // Will wrap around when a line is got

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
noreturn void lexer_error(struct Lexer *self, const char *ERROR_MSG,
						  const struct LexerToken *token) {
	char *line;
	size_t length = strlen(ulToString(self->chrIndex)) + 3, lineIndex = 0;

	self->filePointer = fopen(self->FILE_PATH, "r");

	if (ferror(self->filePointer)) { // File should exist at this stage, but
									 // just in case
		panic(stringConcatenate(
			3, "failed to open file while reporting error '", ERROR_MSG, "'"));
	}

	printf("%s%s--> %s%s\n", F_BRIGHT_BLUE, S_BOLD, S_RESET,
		   self->FILE_PATH); // Print file

	while (true) {
		line = "";

		while (true) {
			char chr = (char)fgetc(
				self->filePointer); // Specify cast to char to silence warnings

			if (chr == '\n' || chr == EOF) { // EOL or EOF
				break;
			}

			line += chr;
		}

		if (lineIndex++ == self->lineIndex) { // Works because lineIndex starts
											  // at '0', not 'negativeIndex'
			break;
		}
	}

	printf("%zu | %s\n", self->lineIndex + 1, line); // Print line

	if (token == NULL) { // The error realtes to only one char
		printf("%s^ %s%serror: %s%s\n", repeatChr(' ', self->chrIndex),
			   F_BRIGHT_RED, S_BOLD, S_RESET, ERROR_MSG); // Print error
	} else { // The error relates to a string (multiple chars)
		self->lineIndex =
			token->lineIndex; // Make sure the current line and char are correct
		self->chrIndex = token->chrIndex;

		printf("%s%s %s%serror: %s%s\n",
			   repeatChr(' ', length + token->startChrIndex),
			   repeatChr('^', self->chrIndex - token->startChrIndex + 1),
			   F_BRIGHT_RED, S_BOLD, S_RESET, ERROR_MSG); // Print error
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
	if (self->filePointer == NULL) { // EOF
		return false;
	} else if (!nextLine && self->chr != '\n') { // EOL has not been reached
		return true;
	}

	self->prevChr = self->chr;
	self->chr = '\n';

	self->chrIndex = negativeIndex; // Will wrap around when a char is got
	self->lineIndex++;

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
	if (self->filePointer == NULL) { // EOF
		return false;
	} else if (self->chrIndex != negativeIndex) { // EOL has been reached
		return false;
	}

	while (true) {
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
 * Un-gets the current character.
 *
 * @param self The current lexer struct.
 *
 * @return Whether the current character was un-got successfully.
 */
bool lexer_unGetChr(struct Lexer *self) {
	if (self->filePointer == NULL) { // EOF
		return false;
	}

	ungetc(self->chr, self->filePointer);

	self->chr = self->prevChr;
	self->prevChr = '\0';
	self->chrIndex--;

	if (ferror(self->filePointer) != 0) {
		fclose(self->filePointer);
		self->filePointer = NULL; // Set to NULL to prevent future errors

		return false;
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
						NULL);
		}

		lexer_unGetChr(self);
	}
}

/**
 * Creates a LexerToken for a single-char token.
 *
 * @param self       The current lexer struct.
 * @param identifier The current token's identifier.
 */
void lexer_lexSingle(struct Lexer *self, const enum LexerTokens IDENTIFIER) {
	const struct LexerToken *token =
		lexerToken_new(IDENTIFIER, string_new(chrToString(self->chr), false),
					   self->chrIndex, self->chrIndex, self->lineIndex);

	array_insert(self->tokens, 0, token);
	lexer_checkForContinuation(self, token);
}

/**
 * Creates a LexerToken for a double-char token.
 *
 * @param self       The current lexer struct.
 * @param chr        The second char to check for.
 * @param NOT_DOUBLE If the second char was not found.
 * @param DOUBLE     If the second char was found.
 */
void lexer_lexDouble(struct Lexer *self, char chr,
					 const enum LexerTokens NOT_DOUBLE,
					 const enum LexerTokens DOUBLE) {
	const struct LexerToken *token = NULL;

	if (lexer_getChr(self, false)) {
		if (self->chr == chr) {
			token = lexerToken_new(
				DOUBLE,
				string_new(chrToString(self->prevChr + self->chr), false),
				self->chrIndex - 1, self->chrIndex, self->lineIndex);
		} else { // Second char was not found, un-get it
			lexer_unGetChr(self);
		}
	}

	if (token == NULL) { // Second char was not found
		token = lexerToken_new(NOT_DOUBLE,
							   string_new(chrToString(self->chr), false),
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
	case '.':
		lexer_lexSingle(self, LEXERTOKENS_DOT);
		break;
	case '@':
		lexer_lexSingle(self, LEXERTOKENS_AT);
		break;
	case '(':
		lexer_lexSingle(self, LEXERTOKENS_OPEN_BRACE);
		break;
	case '[':
		lexer_lexSingle(self, LEXERTOKENS_OPEN_SQUARE_BRACE);
		break;
	case '{':
		lexer_lexSingle(self, LEXERTOKENS_OPEN_CURLY_BRACE);
		break;
	case ')':
		lexer_lexSingle(self, LEXERTOKENS_CLOSE_BRACE);
		break;
	case ']':
		lexer_lexSingle(self, LEXERTOKENS_CLOSE_SQUARE_BRACE);
		break;
	case '}':
		lexer_lexSingle(self, LEXERTOKENS_CLOSE_CURLY_BRACE);
		break;
	case ',':
		lexer_lexSingle(self, LEXERTOKENS_COMMA);
		break;
	case ':':
		lexer_lexDouble(self, self->chr, LEXERTOKENS_COLON,
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
		if (!lexer_getChr(self, true)) { // EOL has been reached
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
