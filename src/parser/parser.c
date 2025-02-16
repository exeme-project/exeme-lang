/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#include "./parser.h"
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

		free((*p_self));
		(*p_self) = NULL;
	} else {
		PANIC("Parser struct has already been freed");
	}
}

void parser_parse_next(struct Parser* p_self) {
	size_t lexerTokenIndex = 0;

	const struct LexerToken* lp_lexerToken = lexer_get_token(p_self->lexer, &lexerTokenIndex);

	switch (lp_lexerToken->identifier) {
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
