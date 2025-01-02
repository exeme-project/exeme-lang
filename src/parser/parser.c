/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include "../includes.c"

#include "../lexer/lexer.c"
#include "../utils/array.c"
#include "../utils/string.c"
#include "./tokens.c"

/**
 * Represents a parser.
 */
struct Parser {
	bool inParsing;
	struct Array *parserTokens;
	struct AST *AST;
	struct Lexer *lexer;
};

#define PARSER_STRUCT_SIZE sizeof(struct Parser)

/**
 * Creates a new Parser struct.
 *
 * @param FILE_PATH The path of the file to parse.
 *
 * @return The created Parser struct.
 */
struct Parser *parser_new(const char *FILE_PATH) {
	struct Parser *self = malloc(PARSER_STRUCT_SIZE);

	if (!self) {
		panic("failed to malloc Parser struct");
	}

	self->inParsing = false;

	self->parserTokens = array_new();
	self->AST = NULL;
	self->lexer = lexer_new(FILE_PATH);

	return self;
}

/* Forward declarations to silence warnings */
bool parser_parse(struct Parser *self, bool freeParserTokens, bool nextLine);

/**
 * Frees a Parser struct.
 *
 * @param self The current Parser struct.
 */
void parser_free(struct Parser **self) {
	struct AST *parserToken = NULL;

	if (self && *self) {
		if ((*self)->parserTokens) {
			for (size_t index = 0; index < (*self)->parserTokens->length; index++) {
				parserToken = (struct AST *)(*self)->parserTokens->_values[index];

				ast_free(&parserToken);
			}

			array_free(&(*self)->parserTokens);
		}

		if ((*self)->AST) {
			ast_free(&(*self)->AST);
		}

		lexer_free(&(*self)->lexer);

		free((*self));
		(*self) = NULL;
	} else {
		panic("Parser struct has already been freed");
	}
}

/**
 * Prints a parsing error and exits.
 *
 * @param self             The current Parser struct.
 * @param ERROR_MSG_NUMBER The error message number.
 * @param ERROR_MSG        The error message.
 * @param TOKEN            The erroneous token.
 */
__attribute__((noreturn)) void parser_error(struct Parser *self, const enum ErrorIdentifiers ERROR_MSG_NUMBER,
														  const char *ERROR_MSG, const struct AST *TOKEN) {
	FILE *filePointer = fopen(self->lexer->FILE_PATH, "r");
	struct String *line = string_new("\0", true);
	size_t lineIndex = 0;

	while (true) {
		char chr = (char)fgetc(filePointer); // Get a char

		if (chr == '\n' || chr == EOF) {						// EOL (or EOF)
			if (self->lexer->lineIndex == lineIndex++) { // If we have been copying the line we want
				break;
			}

			// string_clear(line);
			// I believe the above line is not needed. Keeping it here in case I'm mistaken.
			// Same as in the lexer_error function.
		} else if (lineIndex == self->lexer->lineIndex) { // If this is the line we want
			string_appendChr(line, chr);
		}
	}

	const char *lineNumberString = ulToString(self->lexer->lineIndex + 1);
	size_t lineNumberStringLength = strlen(lineNumberString);

	printf("-%s> %s\n%s | %s\n%s", repeatChr('-', lineNumberStringLength), self->lexer->FILE_PATH, lineNumberString,
			 line->_value, repeatChr(' ', lineNumberStringLength + 3));

	bool failed = false;
	size_t startChrIndex = 0, endChrIndex = 0;

	if (TOKEN) {
		switch (TOKEN->IDENTIFIER) {
		case ASTTOKENS_CHR:
			startChrIndex = TOKEN->data.AST_CHR->_token->startChrIndex;
			endChrIndex = TOKEN->data.AST_CHR->_token->endChrIndex;
			break;
		case ASTTOKENS_STRING:
			startChrIndex = TOKEN->data.AST_STRING->_token->startChrIndex;
			endChrIndex = TOKEN->data.AST_STRING->_token->endChrIndex;
			break;
		case ASTTOKENS_INTEGER:
			startChrIndex = TOKEN->data.AST_INTEGER->_token->startChrIndex;
			endChrIndex = TOKEN->data.AST_INTEGER->_token->endChrIndex;
			break;
		case ASTTOKENS_FLOAT:
			startChrIndex = TOKEN->data.AST_FLOAT->_token->startChrIndex;
			endChrIndex = TOKEN->data.AST_FLOAT->_token->endChrIndex;
			break;

		case ASTTOKENS_VARIABLE:
			startChrIndex = TOKEN->data.AST_VARIABLE->_token->startChrIndex;
			endChrIndex = TOKEN->data.AST_VARIABLE->_token->endChrIndex;
			break;
		case ASTTOKENS_ASSIGNMENT:
			startChrIndex = TOKEN->data.AST_ASSIGNMENT->_token->startChrIndex;
			endChrIndex = TOKEN->data.AST_ASSIGNMENT->_token->endChrIndex;
			break;
		case ASTTOKENS_MODULO_ASSIGNMENT:
			startChrIndex = TOKEN->data.AST_MODULO_ASSIGNMENT->_token->startChrIndex;
			endChrIndex = TOKEN->data.AST_MODULO_ASSIGNMENT->_token->endChrIndex;
			break;
		case ASTTOKENS_MULTIPLICATION_ASSIGNMENT:
			startChrIndex = TOKEN->data.AST_MULTIPLICATION_ASSIGNMENT->_token->startChrIndex;
			endChrIndex = TOKEN->data.AST_MULTIPLICATION_ASSIGNMENT->_token->endChrIndex;
			break;
		case ASTTOKENS_EXPONENT_ASSIGNMENT:
			startChrIndex = TOKEN->data.AST_EXPONENT_ASSIGNMENT->_token->startChrIndex;
			endChrIndex = TOKEN->data.AST_EXPONENT_ASSIGNMENT->_token->endChrIndex;
			break;
		case ASTTOKENS_DIVISION_ASSIGNMENT:
			startChrIndex = TOKEN->data.AST_DIVISION_ASSIGNMENT->_token->startChrIndex;
			endChrIndex = TOKEN->data.AST_DIVISION_ASSIGNMENT->_token->endChrIndex;
			break;
		case ASTTOKENS_FLOOR_DIVISION_ASSIGNMENT:
			startChrIndex = TOKEN->data.AST_FLOOR_DIVISION_ASSIGNMENT->_token->startChrIndex;
			endChrIndex = TOKEN->data.AST_FLOOR_DIVISION_ASSIGNMENT->_token->endChrIndex;
			break;
		case ASTTOKENS_ADDITION_ASSIGNMENT:
			startChrIndex = TOKEN->data.AST_ADDITION_ASSIGNMENT->_token->startChrIndex;
			endChrIndex = TOKEN->data.AST_ADDITION_ASSIGNMENT->_token->endChrIndex;
			break;
		case ASTTOKENS_SUBTRACTION_ASSIGNMENT:
			startChrIndex = TOKEN->data.AST_SUBTRACTION_ASSIGNMENT->_token->startChrIndex;
			endChrIndex = TOKEN->data.AST_SUBTRACTION_ASSIGNMENT->_token->endChrIndex;
			break;
		case ASTTOKENS_BITWISE_AND_ASSIGNMENT:
			startChrIndex = TOKEN->data.AST_BITWISE_AND_ASSIGNMENT->_token->startChrIndex;
			endChrIndex = TOKEN->data.AST_BITWISE_AND_ASSIGNMENT->_token->endChrIndex;
			break;
		case ASTTOKENS_BITWISE_OR_ASSIGNMENT:
			startChrIndex = TOKEN->data.AST_BITWISE_OR_ASSIGNMENT->_token->startChrIndex;
			endChrIndex = TOKEN->data.AST_BITWISE_OR_ASSIGNMENT->_token->endChrIndex;
			break;
		case ASTTOKENS_BITWISE_XOR_ASSIGNMENT:
			startChrIndex = TOKEN->data.AST_BITWISE_XOR_ASSIGNMENT->_token->startChrIndex;
			endChrIndex = TOKEN->data.AST_BITWISE_XOR_ASSIGNMENT->_token->endChrIndex;
			break;
		case ASTTOKENS_BITWISE_NOT_ASSIGNMENT:
			startChrIndex = TOKEN->data.AST_BITWISE_NOT_ASSIGNMENT->_token->startChrIndex;
			endChrIndex = TOKEN->data.AST_BITWISE_NOT_ASSIGNMENT->_token->endChrIndex;
			break;
		case ASTTOKENS_BITWISE_LEFT_SHIFT_ASSIGNMENT:
			startChrIndex = TOKEN->data.AST_BITWISE_LEFT_SHIFT_ASSIGNMENT->_token->startChrIndex;
			endChrIndex = TOKEN->data.AST_BITWISE_LEFT_SHIFT_ASSIGNMENT->_token->endChrIndex;
			break;
		case ASTTOKENS_BITWISE_RIGHT_SHIFT_ASSIGNMENT:
			startChrIndex = TOKEN->data.AST_BITWISE_RIGHT_SHIFT_ASSIGNMENT->_token->startChrIndex;
			endChrIndex = TOKEN->data.AST_BITWISE_RIGHT_SHIFT_ASSIGNMENT->_token->endChrIndex;
			break;
		case ASTTOKENS_OPEN_BRACE:
			startChrIndex = TOKEN->data.AST_OPEN_BRACE->_token->startChrIndex;
			endChrIndex = TOKEN->data.AST_OPEN_BRACE->_token->endChrIndex;
			break;
		case ASTTOKENS_CLOSE_BRACE:
			startChrIndex = TOKEN->data.AST_CLOSE_BRACE->_token->startChrIndex;
			endChrIndex = TOKEN->data.AST_CLOSE_BRACE->_token->endChrIndex;
			break;
		case ASTTOKENS_COMMA:
			startChrIndex = TOKEN->data.AST_COMMA->_token->startChrIndex;
			endChrIndex = TOKEN->data.AST_COMMA->_token->endChrIndex;
			break;
		case ASTTOKENS_COLON:
			startChrIndex = TOKEN->data.AST_COLON->_token->startChrIndex;
			endChrIndex = TOKEN->data.AST_COLON->_token->endChrIndex;
			break;
		case ASTTOKENS_FUNCTION_DEFINITION:
			startChrIndex = TOKEN->data.AST_FUNCTION_DEFINITION->_token->startChrIndex;
			endChrIndex = TOKEN->data.AST_FUNCTION_DEFINITION->_token->endChrIndex;
			break;
		default:
			failed = true;
		}
	}

	if (TOKEN && !failed) { // We know the start and end index of the erroneous token
		printf("%s%s ", repeatChr(' ', startChrIndex), repeatChr('^', endChrIndex - startChrIndex + 1));
	} else { // We don't since either we weren't given the token, or getting the start and end index failed, fallback to the
				// current lexer pointer
		printf("%s^ ", repeatChr(' ', self->lexer->chrIndex));
	}

	printf("%serror[%s]:%s %s\n", F_BRIGHT_RED, error_get(ERROR_MSG_NUMBER), S_RESET, ERROR_MSG);

	if (failed) {
		printf("%s^ parser received a token with an identifier it does not support yet; defaulting to current lexer index.\n",
				 repeatChr(' ', lineNumberStringLength + 3 + self->lexer->chrIndex));
	}

	exit(EXIT_FAILURE);
}

/**
 * Parses the current chr or string.
 *
 * @param self       The current Parser struct.
 * @param lexerToken The current lexer token.
 */
void parser_parseChrOrString(struct Parser *self, const struct LexerToken *lexerToken) {
	if (lexerToken->identifier == LEXERTOKENS_CHR) {
		array_append(self->parserTokens,
						 ast_new(ASTTOKENS_CHR, AST_CHR, lexerToken, string_new(lexerToken->value->_value, true)));
	} else {
		array_append(self->parserTokens,
						 ast_new(ASTTOKENS_STRING, AST_STRING, lexerToken, string_new(lexerToken->value->_value, true)));
	}
}

/**
 * Parses the current number.
 *
 * @param self The current Parser struct.
 * @param lexerToken The current lexer token.
 */
void parser_parseNumber(struct Parser *self, const struct LexerToken *lexerToken) {
	if (lexerToken->identifier == LEXERTOKENS_INTEGER) {
		array_append(self->parserTokens,
						 ast_new(ASTTOKENS_INTEGER, AST_INTEGER, lexerToken, string_new(lexerToken->value->_value, true)));
	} else {
		array_append(self->parserTokens,
						 ast_new(ASTTOKENS_FLOAT, AST_FLOAT, lexerToken, string_new(lexerToken->value->_value, true)));
	}
}

/**
 * Parses a class.
 *
 * @param self                   The current Parser struct.
 * @param classKeywordLexerToken The current lexer token.
 */
void parser_parseClass(struct Parser *self, const struct LexerToken *classKeywordLexerToken) {
	struct AST *class = NULL, *identifier = NULL, *openingBrackets = NULL;

	if (!parser_parse(self, false, false)) {
		lexer_error(self->lexer, P0001, "expected 1 parser token after 'class' keyword, got 0", classKeywordLexerToken);
	}

	identifier = (struct AST *)array_get(self->parserTokens, 0);

	if (identifier->IDENTIFIER != ASTTOKENS_VARIABLE) {
		parser_error(self, P0002,
						 stringConcatenate("expected parser token of type '", astTokens_getName(ASTTOKENS_VARIABLE),
												 "' after 'class' keyword, got '", astTokens_getName(identifier->IDENTIFIER), "'"),
						 identifier);
	}

	if (!parser_parse(self, false, false)) {
		parser_error(self, P0001,
						 "expected 1 parser token after class identifier, "
						 "got 0",
						 identifier);
	}

	openingBrackets = (struct AST *)array_get(self->parserTokens, 1);

	if (openingBrackets->IDENTIFIER != ASTTOKENS_OPEN_BRACE) {
		parser_error(self, P0002,
						 stringConcatenate("expected parser token of type '", astTokens_getName(ASTTOKENS_OPEN_BRACE),
												 "' after class identifier, got '", astTokens_getName(openingBrackets->IDENTIFIER), "'"),
						 openingBrackets);
	}
}

/**
 * Parses the current class.
 *
 * @param self                   The current Parser struct.
 * @param classKeywordLexerToken The current lexer token.
 */
void parser_parseKeyword_class(struct Parser *self, const struct LexerToken *classKeywordLexerToken) {
	parser_parseClass(self, classKeywordLexerToken);
}

/**
 * Parses a function.
 *
 * @param self                  The current Parser struct.
 * @param funcKeywordLexerToken The current lexer token.
 */
void parser_parseFunction(struct Parser *self, const struct LexerToken *funcKeywordLexerToken,
								  struct AST *parsedIdentifier) {
	struct AST *function = NULL, *identifier = NULL, *openingBrackets = NULL, *arguments = NULL, *argumentIdentifier = NULL,
				  *argumentTypeSeparator = NULL, *argumentType = NULL, *closingBrackets = NULL,
				  *lastToken = NULL; // arguments are placeholders, so its better to use that word rather than parameters
	bool firstArgument = true;

	if (parsedIdentifier) { // if this is a function call
		identifier = parsedIdentifier;
	} else {
		if (!parser_parse(self, false, false)) {
			lexer_error(self->lexer, P0001, "expected 1 parser token after 'func', got 0", funcKeywordLexerToken);
		}

		identifier = (struct AST *)array_get(self->parserTokens, 0);
	}

	if (identifier->IDENTIFIER != ASTTOKENS_VARIABLE) {
		parser_error(self, P0002,
						 stringConcatenate("expected parser token of type '", astTokens_getName(ASTTOKENS_VARIABLE),
												 "' after 'func' keyword, got '", astTokens_getName(identifier->IDENTIFIER), "'"),
						 identifier);
	}

	if (!parsedIdentifier) { // if this is a function declaration
		if (!parser_parse(self, false, false)) {
			parser_error(self, P0001,
							 "expected 1 parser token after function identifier, "
							 "got 0",
							 identifier);
		}

		openingBrackets = (struct AST *)array_get(self->parserTokens, 0);

		if (openingBrackets->IDENTIFIER != ASTTOKENS_OPEN_BRACE) {
			parser_error(self, P0002,
							 stringConcatenate("expected parser token of type '", astTokens_getName(ASTTOKENS_OPEN_BRACE),
													 "' after function identifier, got '", astTokens_getName(openingBrackets->IDENTIFIER),
													 "'"),
							 openingBrackets);
		}

		lastToken = openingBrackets;
	}

	while (true) {
		if (!parser_parse(self, false, true)) {
			parser_error(self, P0001,
							 stringConcatenate("expected parser token of type '", astTokens_getName(ASTTOKENS_CLOSE_BRACE),
													 "' after function arguments, got 'EOF'"),
							 lastToken);
		}

		argumentIdentifier = (struct AST *)array_get(self->parserTokens, 0);

		if (argumentIdentifier->IDENTIFIER == ASTTOKENS_CLOSE_BRACE) {
			closingBrackets = argumentIdentifier; // TODO: You know... do stuff here.
			break;
		} else if (!firstArgument && argumentIdentifier->IDENTIFIER == ASTTOKENS_COMMA) {
			continue;
		} else if (argumentIdentifier->IDENTIFIER != ASTTOKENS_VARIABLE) {
			parser_error(self, P0002,
							 stringConcatenate("expected parser token of type '", astTokens_getName(ASTTOKENS_VARIABLE),
													 "' for function argument, got '", astTokens_getName(argumentIdentifier->IDENTIFIER),
													 "'"),
							 argumentIdentifier);
		}

		if (!parser_parse(self, false, true)) {
			parser_error(self, P0001,
							 "expected 1 parser token after function argument, "
							 "got 0",
							 argumentIdentifier);
		}

		argumentTypeSeparator = (struct AST *)array_get(self->parserTokens, 0);

		if (argumentTypeSeparator->IDENTIFIER != ASTTOKENS_COLON) {
			parser_error(self, P0002,
							 stringConcatenate("expected parser token of type '", astTokens_getName(ASTTOKENS_COLON),
													 "' after function argument, got '",
													 astTokens_getName(argumentTypeSeparator->IDENTIFIER), "'"),
							 argumentTypeSeparator);
		}

		if (!parser_parse(self, false, true)) {
			parser_error(self, P0001,
							 "expected 1 parser token after function argument "
							 "type separator, got 0",
							 argumentTypeSeparator);
		}

		argumentType = (struct AST *)array_get(self->parserTokens, 0);

		if (argumentType->IDENTIFIER != ASTTOKENS_VARIABLE) {
			parser_error(self, P0002,
							 stringConcatenate("expected parser token of type '", astTokens_getName(ASTTOKENS_VARIABLE),
													 "' for function argument type, got '", astTokens_getName(argumentType->IDENTIFIER),
													 "'"),
							 argumentType);
		}

		printf("argumentIdentifier: %s\n", argumentIdentifier->data.AST_VARIABLE->NAME->_value);
		printf("argumentType: %s\n\n", argumentType->data.AST_VARIABLE->NAME->_value);

		// TODO: You know... work out if it is actually a type. And that requires types to be supported, and so classes...

		lastToken = argumentType;

		if (firstArgument) {
			firstArgument = false;
		}
	}

	if (!parsedIdentifier) { // if this is a function declaration
	}
}

/**
 * Parses the current function.
 *
 * @param self                  The current Parser struct.
 * @param funcKeywordLexerToken The current lexer token.
 */
void parser_parseKeyword_func(struct Parser *self, const struct LexerToken *funcKeywordLexerToken) {
	parser_parseFunction(self, funcKeywordLexerToken, NULL);
}

/**
 * Parses the current keyword.
 *
 * @param self The current Parser struct.
 * @param lexerToken The current lexer token.
 */
void parser_parseKeyword(struct Parser *self, const struct LexerToken *lexerToken) {
	if (strcmp(lexerToken->value->_value, "class") == 0) {
		parser_parseKeyword_class(self, lexerToken);
	} else if (strcmp(lexerToken->value->_value, "func") == 0) {
		parser_parseKeyword_func(self, lexerToken);
	} else if (strcmp(lexerToken->value->_value, "import") == 0) {
		// TODO: Add import handling logic
	} else { // TODO: Add support for all keywords
		printf("unsupported keyword for parser's keyword parser: %s\n", lexerToken->value->_value);
	}
}

/**
 * Parses the current identifier.
 *
 * @param self            The current Parser struct.
 * @param lexerToken      The current lexer token.
 * @param lexerTokenIndex The current lexer token's index.
 */
void parser_parseIdentifier(struct Parser *self, const struct LexerToken *lexerToken, size_t lexerTokenIndex) {
	bool pointer = false;

	if (self->lexer->tokens->length > 1) {
		if (((const struct LexerToken *)array_get(self->lexer->tokens, lexerTokenIndex - 1))->identifier ==
			 LEXERTOKENS_MULTIPLICATION) {
			pointer = true;
		}
	}

	array_append(self->parserTokens,
					 ast_new(ASTTOKENS_VARIABLE, AST_VARIABLE, pointer, lexerToken, string_new(lexerToken->value->_value, true)));
}

/**
 * Parses the current assignment.
 *
 * @param self       The current Parser struct.
 * @param lexerToken The current lexer token.
 */
void parser_parseAssignment(struct Parser *self, const struct LexerToken *lexerToken) {
	struct AST *identifier = NULL, *value = NULL;

	if (self->parserTokens->length != 1) {
		lexer_error(
			 self->lexer, P0001,
			 stringConcatenate("expected 1 parser token before assignment, got ", ulToString(self->parserTokens->length)),
			 lexerToken);
	}

	identifier = (struct AST *)self->parserTokens->_values[0];

	if (identifier->IDENTIFIER != ASTTOKENS_VARIABLE) {
		parser_error(self, P0002,
						 stringConcatenate("expected parser token of type '", astTokens_getName(ASTTOKENS_VARIABLE),
												 "' before assignment, got '", astTokens_getName(identifier->IDENTIFIER), "'"),
						 identifier);
	}

	array_clear(self->parserTokens,
					NULL); // Clear the array without freeing the inner data

	if (!parser_parse(self, false, false)) {
		lexer_error(self->lexer, P0001, "expected 1 parser token after assignment, got 0", lexerToken);
	}

	if (self->parserTokens->length != 1) {
		lexer_error(
			 self->lexer, P0001,
			 stringConcatenate("expected 1 parser token after assignment, got ", ulToString(self->parserTokens->length)),
			 lexerToken);
	}

	value = (struct AST *)self->parserTokens->_values[0];

	switch (lexerToken->identifier) { //  For the different types of assignment
	case LEXERTOKENS_ASSIGNMENT:
		self->AST = ast_new(ASTTOKENS_ASSIGNMENT, AST_ASSIGNMENT, lexerToken,
								  (const struct AST_VARIABLE *)identifier->data.AST_ASSIGNMENT, value);
		break;
	case LEXERTOKENS_MODULO_ASSIGNMENT:
		self->AST = ast_new(ASTTOKENS_MODULO_ASSIGNMENT, AST_MODULO_ASSIGNMENT, lexerToken,
								  (const struct AST_VARIABLE *)identifier->data.AST_ASSIGNMENT, value);
		break;
	case LEXERTOKENS_MULTIPLICATION_ASSIGNMENT:
		self->AST = ast_new(ASTTOKENS_MULTIPLICATION_ASSIGNMENT, AST_MULTIPLICATION_ASSIGNMENT, lexerToken,
								  (const struct AST_VARIABLE *)identifier->data.AST_ASSIGNMENT, value);
		break;
	case LEXERTOKENS_EXPONENT_ASSIGNMENT:
		self->AST = ast_new(ASTTOKENS_EXPONENT_ASSIGNMENT, AST_EXPONENT_ASSIGNMENT, lexerToken,
								  (const struct AST_VARIABLE *)identifier->data.AST_ASSIGNMENT, value);
		break;
	case LEXERTOKENS_DIVISION_ASSIGNMENT:
		self->AST = ast_new(ASTTOKENS_DIVISION_ASSIGNMENT, AST_DIVISION_ASSIGNMENT, lexerToken,
								  (const struct AST_VARIABLE *)identifier->data.AST_ASSIGNMENT, value);
		break;
	case LEXERTOKENS_FLOOR_DIVISION_ASSIGNMENT:
		self->AST = ast_new(ASTTOKENS_FLOOR_DIVISION_ASSIGNMENT, AST_FLOOR_DIVISION_ASSIGNMENT, lexerToken,
								  (const struct AST_VARIABLE *)identifier->data.AST_ASSIGNMENT, value);
		break;
	case LEXERTOKENS_ADDITION_ASSIGNMENT:
		self->AST = ast_new(ASTTOKENS_ADDITION_ASSIGNMENT, AST_ADDITION_ASSIGNMENT, lexerToken,
								  (const struct AST_VARIABLE *)identifier->data.AST_ASSIGNMENT, value);
		break;
	case LEXERTOKENS_SUBTRACTION_ASSIGNMENT:
		self->AST = ast_new(ASTTOKENS_SUBTRACTION_ASSIGNMENT, AST_SUBTRACTION_ASSIGNMENT, lexerToken,
								  (const struct AST_VARIABLE *)identifier->data.AST_ASSIGNMENT, value);
		break;
	case LEXERTOKENS_BITWISE_AND_ASSIGNMENT:
		self->AST = ast_new(ASTTOKENS_BITWISE_AND_ASSIGNMENT, AST_BITWISE_AND_ASSIGNMENT, lexerToken,
								  (const struct AST_VARIABLE *)identifier->data.AST_ASSIGNMENT, value);
		break;
	case LEXERTOKENS_BITWISE_OR_ASSIGNMENT:
		self->AST = ast_new(ASTTOKENS_BITWISE_OR_ASSIGNMENT, AST_BITWISE_OR_ASSIGNMENT, lexerToken,
								  (const struct AST_VARIABLE *)identifier->data.AST_ASSIGNMENT, value);
		break;
	case LEXERTOKENS_BITWISE_XOR_ASSIGNMENT:
		self->AST = ast_new(ASTTOKENS_BITWISE_XOR_ASSIGNMENT, AST_BITWISE_XOR_ASSIGNMENT, lexerToken,
								  (const struct AST_VARIABLE *)identifier->data.AST_ASSIGNMENT, value);
		break;
	case LEXERTOKENS_BITWISE_NOT_ASSIGNMENT:
		self->AST = ast_new(ASTTOKENS_BITWISE_NOT_ASSIGNMENT, AST_BITWISE_NOT_ASSIGNMENT, lexerToken,
								  (const struct AST_VARIABLE *)identifier->data.AST_ASSIGNMENT, value);
		break;
	case LEXERTOKENS_BITWISE_LEFT_SHIFT_ASSIGNMENT:
		self->AST = ast_new(ASTTOKENS_BITWISE_LEFT_SHIFT_ASSIGNMENT, AST_BITWISE_LEFT_SHIFT_ASSIGNMENT, lexerToken,
								  (const struct AST_VARIABLE *)identifier->data.AST_ASSIGNMENT, value);
		break;
	case LEXERTOKENS_BITWISE_RIGHT_SHIFT_ASSIGNMENT:
		self->AST = ast_new(ASTTOKENS_BITWISE_RIGHT_SHIFT_ASSIGNMENT, AST_BITWISE_RIGHT_SHIFT_ASSIGNMENT, lexerToken,
								  (const struct AST_VARIABLE *)identifier->data.AST_ASSIGNMENT, value);
		break;
	default:
		printf("unsupported lexer token for parser: %s\n",
				 lexerTokens_getName(lexerToken->identifier)); // TODO: Fix
		break;
	}

	free(identifier); // Free the identifier of the assignment, which is not
							// needed anymore
	array_clear(self->parserTokens,
					NULL); // Clear the array without freeing the inner data
}

/**
 * Calls the correct function for lexing the current lexer token.
 *
 * @param self The current Parser struct.
 */
void parser_parseNext(struct Parser *self) {
	size_t lexerTokenIndex = 0;

	const struct LexerToken *lexerToken = lexer_getToken(self->lexer, &lexerTokenIndex);

	switch (lexerToken->identifier) {
	case LEXERTOKENS_CHR:
	case LEXERTOKENS_STRING:
		parser_parseChrOrString(self, lexerToken);
		break;
	case LEXERTOKENS_INTEGER:
	case LEXERTOKENS_FLOAT:
		parser_parseNumber(self, lexerToken);
		break;
	case LEXERTOKENS_KEYWORD:
		parser_parseKeyword(self, lexerToken);
		break;
	case LEXERTOKENS_IDENTIFIER:
		parser_parseIdentifier(self, lexerToken, lexerTokenIndex);
		break;
	case LEXERTOKENS_ASSIGNMENT:
	case LEXERTOKENS_MODULO_ASSIGNMENT:
	case LEXERTOKENS_MULTIPLICATION_ASSIGNMENT:
	case LEXERTOKENS_EXPONENT_ASSIGNMENT:
	case LEXERTOKENS_DIVISION_ASSIGNMENT:
	case LEXERTOKENS_FLOOR_DIVISION_ASSIGNMENT:
	case LEXERTOKENS_ADDITION_ASSIGNMENT:
	case LEXERTOKENS_SUBTRACTION_ASSIGNMENT:
	case LEXERTOKENS_BITWISE_AND_ASSIGNMENT:
	case LEXERTOKENS_BITWISE_OR_ASSIGNMENT:
	case LEXERTOKENS_BITWISE_XOR_ASSIGNMENT:
	case LEXERTOKENS_BITWISE_NOT_ASSIGNMENT:
	case LEXERTOKENS_BITWISE_LEFT_SHIFT_ASSIGNMENT:
	case LEXERTOKENS_BITWISE_RIGHT_SHIFT_ASSIGNMENT:
		parser_parseAssignment(self, lexerToken);
		break;
	case LEXERTOKENS_OPEN_BRACE:
		array_append(self->parserTokens, ast_new(ASTTOKENS_OPEN_BRACE, AST_OPEN_BRACE, lexerToken));
		break;
	case LEXERTOKENS_CLOSE_BRACE:
		array_append(self->parserTokens, ast_new(ASTTOKENS_CLOSE_BRACE, AST_CLOSE_BRACE, lexerToken));
		break;
	case LEXERTOKENS_COMMA:
		array_append(self->parserTokens, ast_new(ASTTOKENS_COMMA, AST_COMMA, lexerToken));
		break;
	case LEXERTOKENS_COLON:
		array_append(self->parserTokens, ast_new(ASTTOKENS_COLON, AST_COLON, lexerToken));
		break;
	default:
		printf("unsupported lexer token for parser: %s\n", lexerTokens_getName(lexerToken->identifier));
		break;
	}
}

/**
 * Gets the next lexer token and parses it.
 *
 * @param self             The current Parser struct.
 * @param freeParserTokens Whether to free the parser tokens when clearing
 * the array.
 * @param nextLine         Whether to get the next line from the lexer.
 *
 * @return bool Whether parsing succeeded.
 */
bool parser_parse(struct Parser *self, bool freeParserTokens, bool nextLine) {
	bool old_inParsing = self->inParsing;

	if (!self->inParsing) {
		self->inParsing = true;
	}

	array_clear(self->parserTokens, freeParserTokens ? (void (*)(const void *))ast_free : NULL);

	if (self->AST) {
		ast_free(&self->AST);
	}

	do {
		if (!lexer_lex(self->lexer, nextLine)) {
			self->inParsing = old_inParsing;

			if (!nextLine && self->parserTokens->length != 0) { // If not allowed to go to next line and
																				 // there has been a parser token parsed
				return true;
			}

			return false;
		}

		parser_parseNext(self);

		if (old_inParsing && self->parserTokens->length > 0) {
			return true;
		}
	} while (self->AST == NULL);

	self->inParsing = old_inParsing;

	return true;
}
