/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include "../errors.h"
#include "../utils/array.h"
#include "./tokens.h"
#include <stdio.h>

/**
 * Used to identify keywords.
 */
extern const struct Array g_KEYWORDS;

/**
 * Represents a lexer.
 */
struct Lexer {
	bool		  nextLine;
	char		  chr, prevChr;
	const char*	  FILE_PATH;
	FILE*		  filePointer;
	int			  chrStatus;
	size_t		  chrIndex, lineIndex, tokenUnlexes;
	struct Array* tokens;
};

#define LEXER_STRUCT_SIZE sizeof(struct Lexer)

/**
 * Creates a new Lexer struct.
 *
 * @param p_filePath The path of the file to lex.
 *
 * @return The created Lexer struct.
 */
struct Lexer* lexer_new(const char* p_filePath);

/**
 * Frees a Lexer struct.
 *
 * @param p_self The current Lexer struct.
 */
void lexer_free(struct Lexer** p_self);

/**
 * Prints a lexing error and exits.
 *
 * @param p_self The current Lexer struct.
 * @param ERROR_MSG_NUMBER The error message number.
 * @param p_errorMsg The error message.
 * @param p_token The erroneous token.
 */
__attribute__((noreturn)) void lexer_error(
	struct Lexer*				p_self,
	const enum ErrorIdentifiers ERROR_MSG_NUMBER, // NOLINT(readability-avoid-const-params-in-decls)
	const char* p_errorMsg, const struct LexerToken* p_token);

/**
 * Gets the next line.
 *
 * @param p_self The current Lexer struct.
 * @param nextLine Get the next line even if the EOL has not been reached.
 *
 * @return Whether the next line was got successfully.
 */
bool lexer_get_line(struct Lexer* p_self, bool nextLine);

/**
 * Un-gets the current character.
 *
 * @param p_self The current Lexer struct.
 *
 * @return Whether the current character was un-got successfully.
 */
bool lexer_un_get_chr(struct Lexer* p_self);

/**
 * Gets the next char.
 *
 * @param p_self The current Lexer struct.
 * @param skipWhitespace Whether to skip whitespace chars.
 *
 * @return Whether the next char was got successfully.
 */
bool lexer_get_chr(struct Lexer* p_self, bool skipWhitespace);

/**
 * Checks for an unexpected continuation of the current token.
 *
 * @param p_self The current Lexer struct.
 * @param p_token The current token.
 */
void lexer_check_for_continuation(struct Lexer* p_self, const struct LexerToken* p_token);

/**
 * Creates a LexerToken for a one-char token.
 *
 * @param p_self The current Lexer struct.
 * @param IDENTIFIER The current token's identifier.
 */
void lexer_lex_one_char(
	struct Lexer*					 p_self,
	const enum LexerTokenIdentifiers IDENTIFIER); // NOLINT(readability-avoid-const-params-in-decls)

/**
 * Creates a LexerToken for a possible two-char token.
 *
 * @param p_self The current Lexer struct.
 * @param SECOND_CHR The second char to check for.
 * @param IF_ONE If the token is one char.
 * @param IF_TWO If the token is two chars.
 * @param continuationCheckIfOne Whether to check for an unexpected
 * continuation if the token is one char.
 * @param continuationCheckIfTwo Whether to check for an unexpected continuation if the token is
 * two chars.
 */
// NOLINTBEGIN(readability-avoid-const-params-in-decls)
void lexer_lex_two_char(struct Lexer* p_self, const char SECOND_CHR,
						const enum LexerTokenIdentifiers IF_ONE,
						const enum LexerTokenIdentifiers IF_TWO, bool continuationCheckIfOne,
						bool continuationCheckIfTwo);
// NOLINTEND(readability-avoid-const-params-in-decls)

/**
 * Creates a LexerToken for a possible two-char token with two possible second chars.
 *
 * @param p_self The current Lexer struct.
 * @param SECOND_CHR The second char to check for.
 * @param OTHER_SECOND_CHR The other second char to check for.
 * @param IF_ONE If the token is one char.
 * @param IF_TWO If the token is two chars, ending with SECOND_CHR.
 * @param IF_OTHER_TWO If the token is two chars, ending with OTHER_SECOND_CHR.
 * @param continuationCheckIfOne Whether to check for an unexpected continuation if the token is
 * one char.
 * @param continuationCheckIfTwo Whether to check for an unexpected continuation if the token is
 * two chars, ending with SECOND_CHR.
 * @param continuationCheckIfOtherTwo Whether to check for an unexpected continuation if the
 * token is two chars, ending with OTHER_SECOND_CHR.
 */
// NOLINTBEGIN(readability-avoid-const-params-in-decls)
void lexer_lex2_two_char(struct Lexer* p_self, const char SECOND_CHR, const char OTHER_SECOND_CHR,
						 const enum LexerTokenIdentifiers IF_ONE,
						 const enum LexerTokenIdentifiers IF_TWO,
						 const enum LexerTokenIdentifiers IF_OTHER_TWO, bool continuationCheckIfOne,
						 bool continuationCheckIfTwo, bool continuationCheckIfOtherTwo);
// NOLINTEND(readability-avoid-const-params-in-decls)

/**
 * Creates a LexerToken for a possible three-char token.
 *
 * @param p_self The current Lexer struct.
 * @param SECOND_CHR The second char to check for.
 * @param THIRD_CHR The third char to check for.
 * @param IF_ONE If the token is one char.
 * @param IF_TWO If the token is two chars, ending with SECOND_CHR.
 * @param IF_TWO_AND_THIRD If the token is two chars, ending with THIRD_CHR.
 * @param IF_THREE_AND_THIRD If the token is three chars, ending with THIRD_CHR.
 * @param continuationCheckIfOne Whether to check for an unexpected continuation if the token is
 * one char.
 * @param continuationCheckIfTwo Whether to check for an unexpected continuation if the token is
 * two chars, ending with SECOND_CHR.
 * @param continuationCheckIfTwoAndThird Whether to check for an unexpected continuation if the
 * token is two chars, ending with THIRD_CHR.
 * @param continuationCheckIfThreeAndThird Whether to check for an unexpected continuation if
 * the token is three chars, ending with THIRD_CHR.
 */
// NOLINTBEGIN(readability-avoid-const-params-in-decls)
void lexer_lex_three_char(struct Lexer* p_self, const char SECOND_CHR, const char THIRD_CHR,
						  const enum LexerTokenIdentifiers IF_ONE,
						  const enum LexerTokenIdentifiers IF_TWO,
						  const enum LexerTokenIdentifiers IF_TWO_AND_THIRD,
						  const enum LexerTokenIdentifiers IF_THREE_AND_THIRD,
						  bool continuationCheckIfOne, bool continuationCheckIfTwo,
						  bool continuationCheckIfTwoAndThird,
						  bool continuationCheckIfThreeAndThird);
// NOLINTEND(readability-avoid-const-params-in-decls)

/**
 * Escapes the current character in the lexer struct.
 *
 * @param p_self The current Lexer struct.
 * @param START_CHR_INDEX The start index of the char.
 *
 * @return The escaped char.
 */
char lexer_escape_chr(
	struct Lexer* p_self,
	const size_t  START_CHR_INDEX); // NOLINT(readability-avoid-const-params-in-decls)

/**
 * Creates a LexerToken for a char.
 *
 * @param p_self The current Lexer struct.
 */
void lexer_lex_chr(struct Lexer* p_self);

/**
 * Creates a LexerToken for a string.
 *
 * @param p_self The current Lexer struct.
 */
void lexer_lex_string(struct Lexer* p_self);

/**
 * Creates a LexerToken for a multi-line comment.
 *
 * @param p_self The current Lexer struct.
 * @param START_CHR_INDEX The start char index of the comment.
 */
void lexer_lex_multi_line_comment(
	struct Lexer* p_self,
	const size_t  START_CHR_INDEX); // NOLINT(readability-avoid-const-params-in-decls)

/**
 * Creates a LexerToken for a single-line comment.
 *
 * @param p_self The current Lexer struct.
 */
void lexer_lex_single_line_comment(struct Lexer* p_self);

/**
 * Creates a LexerToken for a keyword or identifier.
 *
 * @param p_self The current Lexer struct.
 */
void lexer_lex_keyword_or_identifier(struct Lexer* p_self);

/**
 * Creates a LexerToken for an integer / float.
 *
 * @param p_self The current Lexer struct.
 */
void lexer_lex_number(struct Lexer* p_self);

/**
 * Calls the correct function for lexing the current character.
 *
 * @param p_self The current Lexer struct.
 *
 * @return Whether lexing succeeded.
 */
bool lexer_lex_next(struct Lexer* p_self);

/**
 * Gets the next char and lexes it.
 *
 * @param p_self The current Lexer struct.
 * @param nextLine Whether the char can be on the next line.
 *
 * @return Whether lexing succeeded.
 */
bool lexer_lex(struct Lexer* p_self, bool nextLine);

/**
 * Un-lexes the last token.
 *
 * @param p_self The current Lexer struct.
 */
void lexer_un_lex(struct Lexer* p_self);

/**
 * Retrieves the last token.
 *
 * @param p_self The current Lexer struct.
 * @param p_index The index of the token.
 *
 * @return The retrieved token.
 */
const struct LexerToken* lexer_get_token(struct Lexer* p_self, size_t* p_index);
