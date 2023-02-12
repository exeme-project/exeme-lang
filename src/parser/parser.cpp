// Part of the Exeme Language Project, under the MIT license. See '/LICENSE' for
// license information. SPDX-License-Identifier: MIT License.

#pragma once

#include <type_traits>
#include <variant>
#include <vector>

#include "../lexer/lexer.cpp"
#include "../utils/hash.cpp"

enum class ParserTokenIdentifiers { FN_DEF };

struct ParserTokens {
	struct FN_DEF {
		bool unnamedArgs;

		FN_DEF(bool unnamedArgs) { this->unnamedArgs = unnamedArgs; }
	};
};

struct ParserToken {
	std::variant<ParserTokens::FN_DEF *> data;

	ParserToken(std::variant<ParserTokens::FN_DEF *> data) {
		this->data = data;
	}
};

class Parser {
  private:
	struct ParserToken *token;

	ParserToken *parseFnArgs(const std::string fnName, LexerToken *openBrace,
							 bool declaration) {
		bool expectingComma = false;
		const std::string functionType =
			(declaration ? "parameters" : "arguments");

		auto token = new ParserToken(new ParserTokens::FN_DEF(false));

		while (true) {
			if (!this->lexer->lex(true, true)) {
				this->lexer->error("unterminated function " + functionType,
								   openBrace->startChrIndex);
			}

			auto funcParameter =
				this->lexer->tokens[this->lexer->tokens.size() - 1];

			if (funcParameter->identifier == LexerTokenIdentifiers::Comma) {
				if (!expectingComma) {
					this->lexer->error("unexpected comma", -1);
				}

				expectingComma = false;
				continue;
			}

			// parser variable declaration
			// check for default parameter value
			// set 'expectingComma' to true
		}

		this->lexer->error("unterminated function " + functionType, -1);
	}

	void parseKeywordFn() {
		this->lexer->clearTokens();

		if (!this->lexer->lex(false, false)) {
			this->lexer->error("expected function name", -1);
		}

		auto fnNameToken = this->lexer->tokens[this->lexer->tokens.size() - 1];

		if (fnNameToken->identifier == LexerTokenIdentifiers::Keyword) {
			this->lexer->error(
				"functions are not allowed keywords as their names",
				fnNameToken->startChrIndex);
		} else if (fnNameToken->identifier != LexerTokenIdentifiers::Variable) {
			this->lexer->error("expected variable (as function name), got '" +
								   LexerTokenIdentifiersNames[static_cast<int>(
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

		if (fnArgsOpenBrace->identifier != LexerTokenIdentifiers::OpenBrace) {
			this->lexer->error("expected open brace, got '" +
								   LexerTokenIdentifiersNames[static_cast<int>(
									   fnNameToken->identifier)] +
								   "'",
							   fnArgsOpenBrace->startChrIndex);
		}

		auto fnArgs =
			this->parseFnArgs(fnNameToken->value, fnArgsOpenBrace, true);
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
			<< LexerTokenIdentifiersNames[static_cast<int>(token->identifier)]
			<< "\nstartChrIndex: " << token->startChrIndex
			<< "\nchrIndex: " << token->chrIndex
			<< "\nlineNum: " << token->lineNum << "\nvalue: " << token->value
			<< "\n\n";

		switch (token->identifier) {
		case LexerTokenIdentifiers::Keyword:
			parseKeyword(token);
			break;
		default:
			break;
		}
	}

  public:
	Lexer *lexer;

	Parser(std::string filePath) {
		this->lexer = new Lexer(filePath);
		this->token = NULL;
	}

	void parse() {
		while (this->lexer->lex(true, true)) {
			this->parseNext(
				this->lexer->tokens[this->lexer->tokens.size() - 1]);
		}
	}
};