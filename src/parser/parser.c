/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#include "./parser.h"
#include "../utils/conversions.h"
#include "../utils/files.h"
#include "./tokens.h"

struct Parser* parser_new(const char* p_filePath) {
	struct Parser* lp_self = malloc(PARSER_STRUCT_SIZE);

	if (!lp_self) {
		PANIC("failed to malloc Parser struct");
	}

	lp_self->inParsing = false;

	lp_self->parserTokens = array_new();
	lp_self->AST		  = NULL;
	lp_self->lexer		  = lexer_new(p_filePath);

	return lp_self;
}

void parser_free(struct Parser** p_self) {
	struct AST* lp_parserToken = NULL;

	if (p_self && *p_self) {
		if ((*p_self)->parserTokens) {
			for (size_t index = 0; index < (*p_self)->parserTokens->length; index++) {
				lp_parserToken = (struct AST*)(*p_self)->parserTokens->_values[index];

				ast_free(&lp_parserToken);
			}

			array_free(&(*p_self)->parserTokens);
		}

		if ((*p_self)->AST) {
			ast_free(&(*p_self)->AST);
		}

		lexer_free(&(*p_self)->lexer);

		free(*p_self);
		*p_self = NULL;
	} else {
		PANIC("Parser struct has already been freed");
	}
}

__attribute__((noreturn)) void parser_error(struct Parser*				p_self,
											const enum ErrorIdentifiers ERROR_MSG_NUMBER,
											const char* p_errorMsg, const struct AST* p_token) {
	FILE*		   lp_filePointer = fopen(p_self->lexer->FILE_PATH, "r");
	struct String* lp_line		  = string_new("\0", true);
	size_t		   lineIndex	  = 0;

	while (true) {
		char chr = (char)fgetc_safe(lp_filePointer);

		if (chr == '\n' || chr == EOF) { // EOL (or EOF)
			if (p_self->lexer->lineIndex
				== lineIndex++) { // If we have been copying the line we want
				break;
			}

			// string_clear(line);
			// I believe the above line is not needed. Keeping it here in case I'm mistaken.
			// Same as in the lexer_error function.
		} else if (lineIndex == p_self->lexer->lineIndex) { // If this is the line we want
			string_append_chr(lp_line, chr);
		}
	}

	const char* lp_lineNumberString	   = ul_to_string(p_self->lexer->lineIndex + 1);
	size_t		lineNumberStringLength = strlen_safe(lp_lineNumberString);

	printf("-%s> %s\n%s | %s\n%s", repeat_chr('-', lineNumberStringLength),
		   p_self->lexer->FILE_PATH, lp_lineNumberString, lp_line->_value,
		   repeat_chr(' ', lineNumberStringLength + 3));

	if (p_token) { // We know the start and end index of the erroneous token
		const struct LexerToken* lp_lexerToken = ast_get_inner_lexer_token(p_token);

		printf("%s%s ", repeat_chr(' ', lp_lexerToken->startChrIndex),
			   repeat_chr('^', lp_lexerToken->endChrIndex - lp_lexerToken->startChrIndex + 1));
	} else { // We don't since either we weren't given the token, or getting the start and end index
		// failed, fallback to the current lexer pointer
		printf("%s^ ", repeat_chr(' ', p_self->lexer->chrIndex));
	}

	printf("%serror[%s]:%s %s\n", gp_F_BRIGHT_RED, error_get(ERROR_MSG_NUMBER), gp_S_RESET,
		   p_errorMsg);

	if (!p_token) {
		printf("%s^ parser didn't receive a token; defaulting "
			   "to current lexer index.\n",
			   repeat_chr(' ', lineNumberStringLength + 3 + p_self->lexer->chrIndex));
	}

	exit(EXIT_FAILURE); // NOLINT(concurrency-mt-unsafe)
}

void parser_parse_next(struct Parser* p_self) {
	size_t lexerTokenIndex = 0;

	const struct LexerToken* lp_lexerToken = lexer_get_token(p_self->lexer, &lexerTokenIndex);

	switch (lp_lexerToken->identifier) {
	case LEXERTOKENS_CHR:

	default:
		printf("unsupported lexer token for parser: %s\n",
			   lexer_tokens_get_name(lp_lexerToken->identifier));
		break;
	}
}

bool parser_parse(struct Parser* p_self, bool freeParserTokens, bool nextLine) {
	bool oldInParsing = p_self->inParsing;

	if (!p_self->inParsing) {
		p_self->inParsing = true;
	}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wcast-function-type-strict"
	array_clear(p_self->parserTokens, freeParserTokens ? (void (*)(const void*))ast_free : NULL);
#pragma clang diagnostic pop

	if (p_self->AST) {
		ast_free(&p_self->AST);
	}

	do {
		if (!lexer_lex(p_self->lexer, nextLine)) {
			p_self->inParsing = oldInParsing;

			if (!nextLine
				&& p_self->parserTokens->length != 0) { // If not allowed to go to next line and
														// there has been a parser token parsed
				return true;
			}

			return false;
		}

		parser_parse_next(p_self);

		if (oldInParsing && p_self->parserTokens->length > 0) {
			return true;
		}
	} while (p_self->AST == NULL);

	p_self->inParsing = oldInParsing;

	return true;
}
