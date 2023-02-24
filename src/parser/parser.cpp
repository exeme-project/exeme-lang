// Part of the Exeme Language Project, under the MIT license. See '/LICENSE' for
// license information. SPDX-License-Identifier: MIT License.

#pragma once

#include "../lexer/lexer.cpp"

class Parser {
  private:
	Lexer *lexer;

	void parseNext(LexerToken *token) {
		std::cout << "identifier: "
				  << LexerTokenNames[static_cast<size_t>(token->identifier)]
				  << "\nstartChrIndex: " << token->startChrIndex
				  << "\nchrIndex: " << token->chrIndex
				  << "\nlineNum: " << token->lineNum
				  << "\nvalue: " << token->value << "\n\n";
	}

  public:
	Parser(std::string filePath) { this->lexer = new Lexer(filePath); }

	bool parse() {
		if (!this->lexer->lex(true, true)) {
			return false;
		}

		this->parseNext(lexer->tokens[lexer->tokens.size() - 1]);

		return true;
	}
};
