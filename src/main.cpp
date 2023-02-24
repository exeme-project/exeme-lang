// Part of the Exeme Language Project, under the MIT license. See '/LICENSE' for
// license information. SPDX-License-Identifier: MIT License.

#include "lexer/lexer.cpp"

#pragma pack(1)

int main() {
	auto lexer = new Lexer(OS == 1 ? "../../src/main.exl" : "../src/main.exl");

	while (lexer->lex(true, true)) {
		auto token = lexer->tokens[lexer->tokens.size() - 1];

		std::cout << "identifier: "
				  << LexerTokenNames[static_cast<size_t>(token->identifier)]
				  << "\nstartChrIndex: " << token->startChrIndex
				  << "\nchrIndex: " << token->chrIndex
				  << "\nlineNum: " << token->lineNum
				  << "\nvalue: " << token->value << "\n\n";

		lexer->clearTokens();
	}
}
