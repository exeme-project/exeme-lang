/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#include "./lexer.h"
#include "../globals.h"
#include "../utils/conversions.h"
#include "../utils/files.h"
#include <ctype.h>
#include <errno.h>
#include <string.h>

const struct Array g_KEYWORDS =
	ARRAY_NEW_STACK("break", "case", "cls", "else", "elif", "enum", "export", "for", "func", "if",
					"impl", "import", "match", "pass", "return", "struct", "trait", "use", "while");

struct Lexer* lexer_new(const char* p_filePath) {
	struct Lexer* lp_self = malloc(LEXER_STRUCT_SIZE);

	if (!lp_self) {
		PANIC("failed to malloc Lexer struct");
	}

	lp_self->nextLine	  = true;
	lp_self->chr		  = '\n';
	lp_self->prevChr	  = '\0';
	lp_self->FILE_PATH	  = p_filePath;
	lp_self->filePointer  = fopen(p_filePath, "r");
	lp_self->chrIndex	  = 0;
	lp_self->lineIndex	  = g_NEGATIVE_ULL; // Will wrap around when a line is got
	lp_self->tokenUnlexes = 0;
	lp_self->tokens		  = array_new();

	if (!lp_self->filePointer ||		// Only POSIX requires errno is set, and so for
										// other platforms we have to check for NULL
		ferror(lp_self->filePointer)) { // Probably file doesn't exist
		error(CONCATENATE_STRING("failed to open file '", lp_self->FILE_PATH, "' - ",
								 strerror(errno))); // NOLINT(concurrency-mt-unsafe)
	}

	return lp_self;
}

void lexer_free(struct Lexer** p_self) {
	if (p_self && *p_self) {
		if ((*p_self)->filePointer) {
			fclose_safe((*p_self)->filePointer);
		}

		array_free(&(*p_self)->tokens);

		free(*p_self);
		*p_self = NULL;
	} else {
		PANIC("Lexer struct has already been freed");
	}
}

__attribute__((noreturn)) void lexer_error(struct Lexer*			   p_self,
										   const enum ErrorIdentifiers ERROR_MSG_NUMBER,
										   const char*				   p_errorMsg,
										   const struct LexerToken*	   p_token) {
	FILE*		   lp_filePointer = fopen(p_self->FILE_PATH, "r");
	struct String* lp_line		  = string_new("\0", true);

	size_t lineIndex = 0;

	while (true) {
		char chr = (char)fgetc_safe(lp_filePointer);

		if (chr == '\n' || chr == EOF) {			// EOL (or EOF)
			if (p_self->lineIndex == lineIndex++) { // If we have been copying the line we want
				break;
			}

			// string_clear(line);
			// I believe the above line is not needed. Keeping it here in case
			// I'm mistaken. Same as in the parser_error function.
		} else if (lineIndex == p_self->lineIndex) { // If this is the line we want
			string_append_chr(lp_line, chr);
		}
	}

	const char* lp_lineNumberString	   = ul_to_string(p_self->lineIndex + 1);
	size_t		lineNumberStringLength = strlen_safe(lp_lineNumberString);

	printf("-%s> %s\n%s | %s\n%s", repeat_chr('-', lineNumberStringLength), p_self->FILE_PATH,
		   lp_lineNumberString, lp_line->_value, repeat_chr(' ', lineNumberStringLength + 3));

	if (p_token) { // We know what token the error occurred on, and telling the
				   // user it will help them
		printf("%s%s ", repeat_chr(' ', p_token->startChrIndex),
			   repeat_chr('^', p_token->endChrIndex - p_token->startChrIndex + 1));
	} else { // We don't know what token the error occurred on
		printf("%s^ ", repeat_chr(' ', p_self->chrIndex));
	}

	printf("%serror[%s]:%s %s\n", gp_F_BRIGHT_RED, error_get(ERROR_MSG_NUMBER), gp_S_RESET,
		   p_errorMsg);

	exit(EXIT_FAILURE); // NOLINT(concurrency-mt-unsafe)
}

bool lexer_get_line(struct Lexer* p_self, bool nextLine) {
	if (!p_self->filePointer) { // EOF
		return false;
	}
	if (!nextLine && !p_self->nextLine) { // EOL has not been reached
		return true;
	}

	p_self->prevChr = '\0';
	p_self->chr		= '\n';

	p_self->chrIndex = g_NEGATIVE_ULL; // Will wrap around when a char is got
	p_self->lineIndex++;

	p_self->nextLine = false;

	return true;
}

bool lexer_un_get_chr(struct Lexer* p_self) {
	if (!p_self->filePointer) { // EOF
		return false;
	}

	if (ungetc(p_self->chr, p_self->filePointer) == EOF) {
		PANIC("failed to ungetc character");
	}

	p_self->chr		= p_self->prevChr;
	p_self->prevChr = '\0'; // We don't know what it was before this
	p_self->chrIndex--;

	if (ferror(p_self->filePointer)) {
		fclose_safe(p_self->filePointer);

		p_self->filePointer = NULL; // Set to NULL to prevent future errors

		return false;
	}

	return true;
}

bool lexer_get_chr(struct Lexer* p_self, bool skipWhitespace) {
	if (!p_self->filePointer || p_self->nextLine) { // EOF / EOL
		return false;
	}

	while (true) {
		char prevChr	  = p_self->prevChr;
		p_self->prevChr	  = p_self->chr;
		p_self->chrStatus = fgetc_safe(p_self->filePointer);
		p_self->chr		  = (char)p_self->chrStatus;
		p_self->chrIndex++;

		if (p_self->chrStatus == EOF || p_self->chr == '\n') { // EOF / error or EOL
			if (p_self->chrStatus == EOF) {					   // EOF / error
				fclose_safe(p_self->filePointer);

				p_self->filePointer = NULL; // Set to NULL to prevent future errors
			}

			// Doing the following before checking for an EOL would break the
			// code
			p_self->chr		 = p_self->prevChr;
			p_self->prevChr	 = prevChr;
			p_self->nextLine = true;
			p_self->chrIndex--;

			return false;
		}

		if (skipWhitespace) {			 // Keep going till we encounter a char that is not
										 // whitespace
			if (!isspace(p_self->chr)) { // Not whitespace
				break;
			}
		} else { // Don't skip whitespace chars
			break;
		}
	}

	return true;
}

void lexer_check_for_continuation(struct Lexer* p_self, const struct LexerToken* p_token) {
	if (lexer_get_chr(p_self, false)) {
		if (!isspace(p_self->chr) && !isalnum(p_self->chr)) {
			lexer_error(p_self, L0002,
						CONCATENATE_STRING("unexpected continuation of token '",
										   p_token->value->_value, "'"),
						lexer_token_new(LEXERTOKENS_NONE,
										string_new(chr_to_string(p_self->chr), true),
										p_self->chrIndex, p_self->chrIndex, p_self->lineIndex));
		}

		lexer_un_get_chr(p_self); // Token was not continued, un-get the current char
	}
}

void lexer_lex_one_char(struct Lexer* p_self, const enum LexerTokenIdentifiers IDENTIFIER) {
	const struct LexerToken* lp_token =
		lexer_token_new(IDENTIFIER, string_new(chr_to_string(p_self->chr), false), p_self->chrIndex,
						p_self->chrIndex, p_self->lineIndex);

	array_append(p_self->tokens, lp_token);
}

// NOLINTBEGIN(bugprone-easily-swappable-parameters)
void lexer_lex_two_char(struct Lexer* p_self, const char SECOND_CHR,
						const enum LexerTokenIdentifiers IF_ONE,
						const enum LexerTokenIdentifiers IF_TWO, bool continuationCheckIfOne,
						bool continuationCheckIfTwo) {
	// NOLINTEND(bugprone-easily-swappable-parameters)
	const struct LexerToken* lp_token = NULL;

	if (lexer_get_chr(p_self, false)) {
		if (p_self->chr == SECOND_CHR) {
			lp_token = lexer_token_new(IF_TWO,
									   string_new(CONCATENATE_STRING(chr_to_string(p_self->prevChr),
																	 chr_to_string(p_self->chr)),
												  false),
									   p_self->chrIndex - 1, p_self->chrIndex, p_self->lineIndex);

			if (continuationCheckIfTwo) {
				lexer_check_for_continuation(p_self, lp_token);
			}
		} else { // SECOND_CHR was not found, un-get it
			lexer_un_get_chr(p_self);
		}
	}

	if (!lp_token) { // SECOND_CHR was not found
		lp_token = lexer_token_new(IF_ONE, string_new(chr_to_string(p_self->chr), false),
								   p_self->chrIndex, p_self->chrIndex, p_self->lineIndex);

		if (continuationCheckIfOne) {
			lexer_check_for_continuation(p_self, lp_token);
		}
	}

	array_append(p_self->tokens, lp_token);
}

// NOLINTBEGIN(bugprone-easily-swappable-parameters)
void lexer_lex2_two_char(struct Lexer* p_self, const char SECOND_CHR, const char OTHER_SECOND_CHR,
						 const enum LexerTokenIdentifiers IF_ONE,
						 const enum LexerTokenIdentifiers IF_TWO,
						 const enum LexerTokenIdentifiers IF_OTHER_TWO, bool continuationCheckIfOne,
						 bool continuationCheckIfTwo, bool continuationCheckIfOtherTwo) {
	// NOLINTEND(bugprone-easily-swappable-parameters)
	const struct LexerToken* lp_token = NULL;

	if (lexer_get_chr(p_self, false)) {
		if (p_self->chr == SECOND_CHR) {
			lp_token = lexer_token_new(IF_TWO,
									   string_new(CONCATENATE_STRING(chr_to_string(p_self->prevChr),
																	 chr_to_string(p_self->chr)),
												  false),
									   p_self->chrIndex - 1, p_self->chrIndex, p_self->lineIndex);

			if (continuationCheckIfTwo) {
				lexer_check_for_continuation(p_self, lp_token);
			}
		} else if (p_self->chr == OTHER_SECOND_CHR) {
			lp_token = lexer_token_new(IF_OTHER_TWO,
									   string_new(CONCATENATE_STRING(chr_to_string(p_self->prevChr),
																	 chr_to_string(p_self->chr)),
												  false),
									   p_self->chrIndex - 1, p_self->chrIndex, p_self->lineIndex);

			if (continuationCheckIfOtherTwo) {
				lexer_check_for_continuation(p_self, lp_token);
			}
		} else {
			lexer_un_get_chr(p_self);
		}
	}

	if (!lp_token) {
		lp_token = lexer_token_new(IF_ONE, string_new(chr_to_string(p_self->chr), false),
								   p_self->chrIndex, p_self->chrIndex, p_self->lineIndex);

		if (continuationCheckIfOne) {
			lexer_check_for_continuation(p_self, lp_token);
		}
	}

	array_append(p_self->tokens, lp_token);
}

// NOLINTBEGIN(bugprone-easily-swappable-parameters)
void lexer_lex_three_char(struct Lexer* p_self, const char SECOND_CHR, const char THIRD_CHR,
						  const enum LexerTokenIdentifiers IF_ONE,
						  const enum LexerTokenIdentifiers IF_TWO,
						  const enum LexerTokenIdentifiers IF_TWO_AND_THIRD,
						  const enum LexerTokenIdentifiers IF_THREE_AND_THIRD,
						  bool continuationCheckIfOne, bool continuationCheckIfTwo,
						  bool continuationCheckIfTwoAndThird,
						  bool continuationCheckIfThreeAndThird) {
	// NOLINTEND(bugprone-easily-swappable-parameters)
	const struct LexerToken* lp_token = NULL;

	if (lexer_get_chr(p_self, false)) {
		if (p_self->chr == SECOND_CHR) {
			const char l_PREV_CHR = p_self->prevChr;

			if (IF_THREE_AND_THIRD && lexer_get_chr(p_self, false)) {
				if (p_self->chr == THIRD_CHR) {
					lp_token = lexer_token_new(
						IF_THREE_AND_THIRD,
						string_new(CONCATENATE_STRING(chr_to_string(l_PREV_CHR),
													  chr_to_string(p_self->prevChr),
													  chr_to_string(p_self->chr)),
								   false),
						p_self->chrIndex - 2, p_self->chrIndex, p_self->lineIndex);

					if (continuationCheckIfThreeAndThird) {
						lexer_check_for_continuation(p_self, lp_token);
					}
				}
			}

			if (!lp_token) { // THIRD_CHR was not found
				lp_token =
					lexer_token_new(IF_TWO,
									string_new(CONCATENATE_STRING(chr_to_string(p_self->prevChr),
																  chr_to_string(p_self->chr)),
											   false),
									p_self->chrIndex - 1, p_self->chrIndex, p_self->lineIndex);

				if (continuationCheckIfTwo) {
					lexer_check_for_continuation(p_self, lp_token);
				}
			}
		} else if (p_self->chr == THIRD_CHR) {
			lp_token = lexer_token_new(IF_TWO_AND_THIRD,
									   string_new(CONCATENATE_STRING(chr_to_string(p_self->prevChr),
																	 chr_to_string(p_self->chr)),
												  false),
									   p_self->chrIndex - 1, p_self->chrIndex, p_self->lineIndex);

			if (continuationCheckIfTwoAndThird) {
				lexer_check_for_continuation(p_self, lp_token);
			}
		} else { // SECOND_CHR was not found, un-get it
			lexer_un_get_chr(p_self);
		}
	}

	if (!lp_token) { // SECOND_CHR was not found
		lp_token = lexer_token_new(IF_ONE, string_new(chr_to_string(p_self->chr), false),
								   p_self->chrIndex, p_self->chrIndex, p_self->lineIndex);

		if (continuationCheckIfOne) {
			lexer_check_for_continuation(p_self, lp_token);
		}
	}

	array_append(p_self->tokens, lp_token);
}

char lexer_escape_chr(struct Lexer* p_self, const size_t START_CHR_INDEX) {
	switch (p_self->chr) {
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
		lexer_error(p_self, L0004, "invalid escape sequence",
					lexer_token_new(LEXERTOKENS_NONE, string_new("\0", true), START_CHR_INDEX,
									p_self->chrIndex, p_self->lineIndex));
	}
}

void lexer_lex_chr(struct Lexer* p_self) {
	const size_t   l_START_CHR_INDEX = p_self->chrIndex;
	size_t		   escapeChrIndex	 = g_NEGATIVE_ULL;
	struct String* lp_chr			 = string_new("\0", true);

	while (lexer_get_chr(p_self, false)) {
		if (p_self->chr == '\'') {
			array_append(p_self->tokens, lexer_token_new(LEXERTOKENS_CHR, lp_chr, l_START_CHR_INDEX,
														 p_self->chrIndex, p_self->lineIndex));
			return;
		}
		if (lp_chr->length == 1) {
			lexer_error(p_self, L0005, "multi-character char literal",
						lexer_token_new(LEXERTOKENS_NONE, string_new("\0", true),
										l_START_CHR_INDEX + 1, p_self->chrIndex,
										p_self->lineIndex));
		}

		if (escapeChrIndex != g_NEGATIVE_ULL) {
			string_append_chr(lp_chr, lexer_escape_chr(p_self, escapeChrIndex));
			escapeChrIndex = g_NEGATIVE_ULL;
		} else if (p_self->chr == '\\') {
			escapeChrIndex = p_self->chrIndex;
		} else {
			string_append_chr(lp_chr, p_self->chr);
		}
	}

	lexer_error(p_self, L0003, "unterminated character literal",
				lexer_token_new(LEXERTOKENS_NONE, lp_chr, l_START_CHR_INDEX, p_self->chrIndex,
								p_self->lineIndex));
}

void lexer_lex_string(struct Lexer* p_self) {
	const size_t   l_START_CHR_INDEX  = p_self->chrIndex;
	const size_t   l_START_LINE_INDEX = p_self->lineIndex;
	size_t		   escapeChrIndex	  = g_NEGATIVE_ULL;
	struct String* lp_string		  = string_new("\0", true);

	while (lexer_get_line(p_self, false)) {
		while (lexer_get_chr(p_self, false)) {
			if (p_self->chr == '"') {
				array_append(p_self->tokens,
							 lexer_token_new(LEXERTOKENS_STRING, lp_string, l_START_CHR_INDEX,
											 p_self->chrIndex, p_self->lineIndex));
				return;
			}

			if (escapeChrIndex != g_NEGATIVE_ULL) {
				string_append_chr(lp_string, lexer_escape_chr(p_self, escapeChrIndex));
				escapeChrIndex = g_NEGATIVE_ULL;
			} else if (p_self->chr == '\\') {
				escapeChrIndex = p_self->chrIndex;
			} else {
				string_append_chr(lp_string, p_self->chr);
			}
		}

		string_append_chr(lp_string, '\n');
	}

	lexer_error(p_self, L0003, "unterminated string literal",
				lexer_token_new(LEXERTOKENS_STRING, lp_string,
								p_self->lineIndex == l_START_LINE_INDEX ? l_START_CHR_INDEX : 0,
								p_self->chrIndex, p_self->lineIndex));
}

void lexer_lex_multi_line_comment(struct Lexer* p_self, const size_t START_CHR_INDEX) {
	const size_t l_START_LINE_INDEX = p_self->lineIndex;

	while (lexer_get_line(p_self, false)) {
		while (lexer_get_chr(p_self, true)) {
			if (p_self->chr == '=') {
				if (lexer_get_chr(p_self, false)) {
					if (p_self->chr == ';') {
						array_append(
							p_self->tokens,
							lexer_token_new(
								LEXERTOKENS_MULTI_LINE_COMMENT, string_new("\0", true),
								p_self->lineIndex == l_START_LINE_INDEX ? START_CHR_INDEX : 0,
								p_self->chrIndex, l_START_LINE_INDEX));
						return;
					}
				}
			}
		}
	}

	lexer_error(p_self, L0003, "unterminated multi-line comment",
				lexer_token_new(LEXERTOKENS_NONE, string_new("\0", true),
								p_self->lineIndex == l_START_LINE_INDEX ? START_CHR_INDEX : 0,
								p_self->chrIndex, p_self->lineIndex));
}

void lexer_lex_single_line_comment(struct Lexer* p_self) {
	const size_t l_START_CHR_INDEX = p_self->chrIndex;

	if (lexer_get_chr(p_self, false)) {
		if (p_self->chr == '=') {
			lexer_lex_multi_line_comment(p_self, l_START_CHR_INDEX);

			return;
		}
		while (lexer_get_chr(p_self, true)) {
		}
	}

	array_append(p_self->tokens,
				 lexer_token_new(LEXERTOKENS_SINGLE_LINE_COMMENT, string_new("\0", true),
								 l_START_CHR_INDEX, p_self->chrIndex, p_self->lineIndex));
}

void lexer_lex_keyword_or_identifier(struct Lexer* p_self) {
	const size_t   l_START_CHR_INDEX = p_self->chrIndex;
	struct String* lp_identifier	 = string_new(chr_to_string(p_self->chr), false);

	while (lexer_get_chr(p_self, false)) {
		if (!isalnum(p_self->chr)) {
			lexer_un_get_chr(p_self);
			break;
		}

		string_append_chr(lp_identifier, p_self->chr);
	}

	array_append(p_self->tokens,
				 lexer_token_new(array_contains((struct Array*)&g_KEYWORDS, &array___match_string,
												lp_identifier->_value)
									 ? LEXERTOKENS_KEYWORD
									 : LEXERTOKENS_IDENTIFIER,
								 lp_identifier, l_START_CHR_INDEX, p_self->chrIndex,
								 p_self->lineIndex));
}

void lexer_lex_number(struct Lexer* p_self) {
	bool		   isFloat		 = false;
	size_t		   startChrIndex = p_self->chrIndex;
	struct String* lp_number	 = string_new(chr_to_string(p_self->chr), false);

	while (lexer_get_chr(p_self, false)) {
		if (isspace(p_self->chr)) {
			break;
		}
		if (isalpha(p_self->chr)) {
			lexer_error(p_self, L0006,
						CONCATENATE_STRING("invalid character for ", isFloat ? "float" : "integer"),
						lexer_token_new(LEXERTOKENS_NONE, lp_number, p_self->chrIndex,
										p_self->chrIndex, p_self->lineIndex));
		} else if (p_self->chr == '.') {
			if (isFloat) {
				lexer_error(p_self, L0007, "too many decimal points for float",
							lexer_token_new(LEXERTOKENS_NONE, lp_number, p_self->chrIndex,
											p_self->chrIndex, p_self->lineIndex));
			} else {
				isFloat = true;
			}
		} else if (!isdigit(p_self->chr)) {
			lexer_un_get_chr(p_self);
			break;
		}

		string_append_chr(lp_number, p_self->chr);
	}

	array_append(p_self->tokens,
				 lexer_token_new(isFloat ? LEXERTOKENS_FLOAT : LEXERTOKENS_INTEGER, lp_number,
								 startChrIndex, p_self->chrIndex, p_self->lineIndex));
}

bool lexer_lex_next(struct Lexer* p_self) {
	switch (p_self->chr) {
	case '\'':
		lexer_lex_chr(p_self);
		break;
	case '"':
		lexer_lex_string(p_self);
		break;

	// Arithmetic operators
	case '%':
		lexer_lex_two_char(p_self, '=', LEXERTOKENS_MODULO, LEXERTOKENS_MODULO_ASSIGNMENT, true,
						   true);
		break;
	case '*':
		lexer_lex_three_char(p_self, p_self->chr, '=', LEXERTOKENS_MULTIPLICATION,
							 LEXERTOKENS_EXPONENT, LEXERTOKENS_MULTIPLICATION_ASSIGNMENT,
							 LEXERTOKENS_EXPONENT_ASSIGNMENT, true, true, true, true);
		break;
	case '/':
		lexer_lex_three_char(p_self, p_self->chr, '=', LEXERTOKENS_DIVISION,
							 LEXERTOKENS_FLOOR_DIVISION, LEXERTOKENS_DIVISION_ASSIGNMENT,
							 LEXERTOKENS_FLOOR_DIVISION_ASSIGNMENT, true, true, true, true);
		break;
	case '+':
		lexer_lex_two_char(p_self, '=', LEXERTOKENS_ADDITION, LEXERTOKENS_ADDITION_ASSIGNMENT, true,
						   true);
		break;
	case '-':
		lexer_lex2_two_char(p_self, '=', '>', LEXERTOKENS_SUBTRACTION,
							LEXERTOKENS_SUBTRACTION_ASSIGNMENT, LEXERTOKENS_TYPE_ARROW, true, true,
							true);
		break;

	// Comparison / Relational operators
	case '=':
		lexer_lex2_two_char(p_self, p_self->chr, '>', LEXERTOKENS_ASSIGNMENT, LEXERTOKENS_EQUAL_TO,
							LEXERTOKENS_ASSIGNMENT_ARROW, true, true, true);
		break;
	case '!':
		lexer_lex_two_char(p_self, '=', LEXERTOKENS_LOGICAL_NOT, LEXERTOKENS_NOT_EQUAL_TO, true,
						   true);
		break;
	case '>':
		lexer_lex_three_char(p_self, p_self->chr, '=', LEXERTOKENS_GREATER_THAN,
							 LEXERTOKENS_BITWISE_RIGHT_SHIFT, LEXERTOKENS_GREATER_THAN_OR_EQUAL,
							 LEXERTOKENS_BITWISE_RIGHT_SHIFT_ASSIGNMENT, false, true, true,
							 true); // 'e.g., 'Example: trait<io::Stringify>(Type) = {}'
		break;
	case '<':
		lexer_lex_three_char(p_self, p_self->chr, '=', LEXERTOKENS_LESS_THAN,
							 LEXERTOKENS_BITWISE_LEFT_SHIFT, LEXERTOKENS_LESS_THAN_OR_EQUAL,
							 LEXERTOKENS_BITWISE_LEFT_SHIFT_ASSIGNMENT, true, true, true, true);
		break;

	// Logical operators
	case '&':
		lexer_lex_three_char(p_self, p_self->chr, '=', LEXERTOKENS_BITWISE_AND,
							 LEXERTOKENS_LOGICAL_AND, LEXERTOKENS_BITWISE_AND_ASSIGNMENT,
							 LEXERTOKENS_NONE, true, true, true, true);
		break;
	case '|':
		lexer_lex_three_char(p_self, p_self->chr, '=', LEXERTOKENS_BITWISE_OR,
							 LEXERTOKENS_LOGICAL_OR, LEXERTOKENS_BITWISE_OR_ASSIGNMENT,
							 LEXERTOKENS_NONE, true, true, true, true);
		break;

	// Bitwise operators
	case '^':
		lexer_lex_two_char(p_self, '=', LEXERTOKENS_BITWISE_XOR, LEXERTOKENS_BITWISE_XOR_ASSIGNMENT,
						   true, true);
		break;
	case '~':
		lexer_lex_two_char(p_self, '=', LEXERTOKENS_BITWISE_NOT, LEXERTOKENS_BITWISE_NOT_ASSIGNMENT,
						   true, true);
		break;

	// Member / Pointer operators
	// NOTE: These all don't call lexer_checkForContinuation
	case '.':
		lexer_lex_one_char(p_self, LEXERTOKENS_DOT);
		break;
	case '@':
		lexer_lex_one_char(p_self, LEXERTOKENS_AT);
		break;

	// Syntactic constructs
	// NOTE: These all don't call lexer_checkForContinuation
	case '(':
		lexer_lex_one_char(p_self, LEXERTOKENS_OPEN_BRACE);
		break;
	case '[':
		lexer_lex_one_char(p_self, LEXERTOKENS_OPEN_SQUARE_BRACE);
		break;
	case '{':
		lexer_lex_one_char(p_self, LEXERTOKENS_OPEN_CURLY_BRACE);
		break;
	case ')':
		lexer_lex_one_char(p_self, LEXERTOKENS_CLOSE_BRACE);
		break;
	case ']':
		lexer_lex_one_char(p_self, LEXERTOKENS_CLOSE_SQUARE_BRACE);
		break;
	case '}':
		lexer_lex_one_char(p_self, LEXERTOKENS_CLOSE_CURLY_BRACE);
		break;
	case ',':
		lexer_lex_one_char(p_self, LEXERTOKENS_COMMA);
		break;
	case ':':
		lexer_lex_two_char(p_self, p_self->chr, LEXERTOKENS_COLON, LEXERTOKENS_SCOPE_RESOLUTION,
						   true, false);
		break;
	case ';':
		lexer_lex_single_line_comment(p_self);
		break;

	// Misc
	default:
		if (isalpha(p_self->chr) || p_self->chr == '_') {
			lexer_lex_keyword_or_identifier(p_self);
		} else if (isdigit(p_self->chr)) {
			lexer_lex_number(p_self);
		} else {
			lexer_error(p_self, L0001, "unknown character", NULL);
		}

		break;
	}

	return true;
}

bool lexer_lex(struct Lexer* p_self, bool nextLine) {
	while (!lexer_get_chr(p_self, true)) { // EOL has been reached
		if (!nextLine || !lexer_get_line(p_self, true)) {
			return false;
		}
	}

	return lexer_lex_next(p_self);
}

void lexer_un_lex(struct Lexer* p_self) { p_self->tokenUnlexes++; }

const struct LexerToken* lexer_get_token(struct Lexer* p_self, size_t* p_index) {
	if (p_self->tokens->length == 0) {
		return NULL;
	}
	if (p_self->tokenUnlexes > 0) {
		*p_index = p_self->tokens->length - 1 - (p_self->tokenUnlexes--);

		return p_self->tokens->_values[*p_index]; // For future me, yes it does
												  // decrement
	}

	*p_index = p_self->tokens->length - 1;

	return p_self->tokens->_values[*p_index];
}
