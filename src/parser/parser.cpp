// Part of the Exeme language project, under the MIT license. See '/LICENSE' for
// license information. SPDX-License-Identifier: MIT License.

#pragma once

#include "../lexer/lexer.cpp"

class Parser {
  public:
	Lexer *lexer;

	Parser(std::string filePath) { this->lexer = new Lexer(filePath); }

	void parse() {
		while (this->lexer->lex()) {
			;
		}
	}
};