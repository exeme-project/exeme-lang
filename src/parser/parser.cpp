// Part of the Exeme language project, under the MIT license. See '/LICENSE' for
// license information. SPDX-License-Identifier: MIT License.

#pragma once

#include "../lexer/lexer.cpp"
#include "../utils/hash.cpp"
#include "../utils/variables.cpp"
#include <type_traits>

class Parser {
  private:
	std::vector<Variable *> parseFnArgs(LexerToken *openBrace,
										bool declaration) {
		bool expectingComma = false;

		while (true) {
			if (!this->lexer->lex(false, false)) {
				this->lexer->error(
					"unterminated function " +
						std::string(declaration ? "parameters" : "arguments"),
					openBrace->startChrIndex);
			}

			auto funcParameter =
				this->lexer->tokens[this->lexer->tokens.size() - 1];

			if (funcParameter->identifier == LexerTokenIdentifier::Comma) {
				if (!expectingComma) {
					this->lexer->error("unexpected comma", -1);
				}

				expectingComma = false;
				continue;
			}
		}
	}

	void parseKeywordFn() {
		if (!this->lexer->lex(false, false)) {
			this->lexer->error("expected function name", -1);
		}

		auto fnNameToken = this->lexer->tokens[this->lexer->tokens.size() - 1];

		if (fnNameToken->identifier == LexerTokenIdentifier::Keyword) {
			this->lexer->error(
				"functions are not allowed keywords as their names",
				fnNameToken->startChrIndex);
		} else if (fnNameToken->identifier != LexerTokenIdentifier::Variable) {
			this->lexer->error("expected variable (as function name), got '" +
								   LexerTokenIdentifierNames[static_cast<int>(
									   fnNameToken->identifier)] +
								   "'",
							   fnNameToken->startChrIndex);
		}

		if (!this->lexer->lex(false, false)) {
			this->lexer->error(
				"expected open brace to start function arguments", -1);
		}

		auto fnArgsOpenBrace =
			this->lexer->tokens[this->lexer->tokens.size() - 1];

		if (fnArgsOpenBrace->identifier !=
			LexerTokenIdentifier::OpenCurlyBrace) {
			this->lexer->error("expected open curly brace, got '" +
								   LexerTokenIdentifierNames[static_cast<int>(
									   fnNameToken->identifier)] +
								   "'",
							   fnArgsOpenBrace->startChrIndex);
		}

		auto fnArgs = this->parseFnArgs(fnArgsOpenBrace, true);
	}

	void parseKeyword(LexerToken *token) {
		switch (hash(token->value)) {
		case hash("fn"):
			parseKeywordFn();
		}
	}

	void parseNext(LexerToken *token) {
		std::cout
			<< "identifier: "
			<< LexerTokenIdentifierNames[static_cast<int>(token->identifier)]
			<< "\nfilePath: " << token->filePath
			<< "\nstartChrIndex: " << token->startChrIndex
			<< "\nchrIndex: " << token->chrIndex
			<< "\nlineNum: " << token->lineNum << "\nvalue: " << token->value
			<< "\n\n";

		switch (token->identifier) {
		case LexerTokenIdentifier::Keyword:
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