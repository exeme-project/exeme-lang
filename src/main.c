/**
 * Part of the Exeme Language Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#include "./includes.c"
#include "./lexer/lexer.c"

#pragma pack(1)

int main(void) {
	struct Lexer *lexer = lexer_new("../programs/complex.exl");

	while (lexer_lex(lexer, true, true)) {
		const struct LexerToken *token = lexer_getToken(lexer);

		printf("identifier: %s\nstartChrIndex: %zu\nchrIndex: %zu\nlineNum: "
			   "%zu\nvalue: %s\n\n",
			   (const char *)LEXER_TOKEN_NAMES._values[token->identifier],
			   token->startChrIndex, token->chrIndex, token->lineNum,
			   token->value);
	}
}
