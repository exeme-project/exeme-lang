/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include "../errors.h"
#include "../lexer/lexer.h"
#include <stdbool.h>

/**
 * Represents a parser.
 */
struct Parser {
	bool		  inParsing;
	struct Array* parserTokens;
	struct AST*	  AST;
	struct Lexer* lexer;
};

#define PARSER_STRUCT_SIZE sizeof(struct Parser)

/**
 * Creates a new Parser struct.
 *
 * @param p_filePath The path of the file to parse.
 *
 * @return The created Parser struct.
 */
struct Parser* parser_new(const char* p_filePath);

/**
 * Frees a Parser struct.
 *
 * @param p_self The current Parser struct.
 */
void parser_free(struct Parser** p_self);

/**
 * Prints a parsing error and exits.
 *
 * @param p_self The current Parser struct.
 * @param ERROR_MSG_NUMBER The error message number.
 * @param p_errorMsg The error message.
 * @param p_token The erroneous token.
 */
__attribute__((noreturn)) void parser_error(
	struct Parser*				p_self,
	const enum ErrorIdentifiers ERROR_MSG_NUMBER, // NOLINT(readability-avoid-const-params-in-decls)
	const char* p_errorMsg, const struct AST* p_token);

/**
 * Calls the correct function for lexing the current lexer token.
 *
 * @param p_self The current Parser struct.
 */
void parser_parse_next(struct Parser* p_self);

/**
 * Gets the next lexer token and parses it.
 *
 * @param p_self The current Parser struct.
 * @param freeParserTokens Whether to free the parser tokens when clearing the array.
 * @param nextLine Whether to get the next line from the lexer.
 *
 * @return Whether parsing succeeded.
 */
bool parser_parse(struct Parser* p_self, bool freeParserTokens, bool nextLine);
