/**
 * Part of the Exeme Language Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#include "./includes.c"
#include "./lexer/lexer.c"

#pragma pack(1)

int main(void) {
	struct Lexer *lexer = lexer_new("../programs/complex.exl");

	while (lexer_lex(lexer, true)) {
		const struct LexerToken *lexerToken = lexer_getToken(lexer);

		if (lexerToken) {
			printf(
				"identifier: %s\nstartChrIndex: %zu\nendChrIndex: "
				"%zu\nlineIndex: "
				"%zu\nvalue: %s\n\n",
				(const char *)LEXER_TOKEN_NAMES._values[lexerToken->identifier],
				lexerToken->startChrIndex, lexerToken->endChrIndex,
				lexerToken->lineIndex, lexerToken->value->_value);

			lexerToken_free(lexerToken);
			array_pop(lexer->tokens);
		}
	}
}
