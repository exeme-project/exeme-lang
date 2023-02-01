// Part of the Exeme language project, under the MIT license. See '/LICENSE' for
// license information. SPDX-License-Identifier: MIT License.

#pragma once

#include "../lexer/lexer.cpp"

class Parser {
  private:
	void parseNext() {
		auto token = this->lexer->tokens[this->lexer->tokens.size() - 1];

		std::cout << "identifier: " << token->identifier
				  << "\nfilePath: " << token->filePath
				  << "\nstartChrIndex: " << token->startChrIndex
				  << "\nchrIndex: " << token->chrIndex
				  << "\nlineNum: " << token->lineNum
				  << "\nvalue: " << token->value << "\n\n";
	}

  public:
	Lexer *lexer;

	Parser(std::string filePath) { this->lexer = new Lexer(filePath); }

	void parse() {
		while (this->lexer->lex(true, true)) {
			this->parseNext();
		}
	}
};