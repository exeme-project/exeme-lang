// Part of the Exeme language project, under the MIT license. See '/LICENSE' for
// license information. SPDX-License-Identifier: MIT License.

#pragma once

#include "../lexer/lexer.cpp"
#include "../utils/hash.cpp"

class Parser {
  private:
	void parseKeywordFn(LexerToken *token) {
		if (!this->lexer->lex(false, true)) {
			this->lexer->error("expected function name", -1);
		}

		auto fnNameToken = this->lexer->tokens[this->lexer->tokens.size() - 1];

		if (fnNameToken->identifier == Keyword) {
			this->lexer->error(
				"functions are not allowed keywords as their names",
				fnNameToken->startChrIndex);
		} else if (fnNameToken->identifier != Variable) {
		}
	}

	void parseKeyword(LexerToken *token) {
		switch (hash(token->value)) {
		case hash("fn"):
			parseKeywordFn(token);
		}
	}

	void parseNext(LexerToken *token) {
		std::cout << "identifier: " << token->identifier
				  << "\nfilePath: " << token->filePath
				  << "\nstartChrIndex: " << token->startChrIndex
				  << "\nchrIndex: " << token->chrIndex
				  << "\nlineNum: " << token->lineNum
				  << "\nvalue: " << token->value << "\n\n";

		switch (token->identifier) {
		case Keyword:
			parseKeyword(token);
			break;
		default:
			break;
		}
	}

  public:
	Lexer *lexer;

	Parser(std::string filePath) { this->lexer = new Lexer(filePath); }

	void parse() {
		while (this->lexer->lex(true, true)) {
			this->parseNext(
				this->lexer->tokens[this->lexer->tokens.size() - 1]);
		}
	}
};