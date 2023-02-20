// Part of the Exeme Language Project, under the MIT license. See '/LICENSE' for
// license information. SPDX-License-Identifier: MIT License.

#pragma once

#include <string>
#include <type_traits>
#include <variant>
#include <vector>

#include "../lexer/lexer.cpp"
#include "../utils/hash.cpp"

enum class AstTokens { FN_DEF };

struct FN_DEF {
	std::string fnName;
	LexerToken *returnType;

	FN_DEF(std::string fnName, LexerToken *returnType) {
		this->fnName = fnName;
		this->returnType = returnType;
	}
};

struct AstToken {
	AstTokens identifier;
	union {
		FN_DEF *FN_DEF;
	};

	AstToken(AstTokens identifier, struct FN_DEF *value) {
		this->identifier = identifier;
		this->FN_DEF = value;
	}
};

class Ast {
  private:
	struct AstToken *token;

	AstToken *parseFnArgs(const std::string fnName, LexerToken *openBrace,
						  bool declaration) {
		bool expectingComma = false;
		const std::string functionType =
			(declaration ? "parameters" : "arguments");

		auto token = new AstToken(AstTokens::FN_DEF, new FN_DEF(fnName, NULL));

		while (true) {
			if (!this->lexer->lex(true, true)) {
				this->lexer->error("unterminated function " + functionType,
								   openBrace->startChrIndex);
			}

			auto funcParameter =
				this->lexer->tokens[this->lexer->tokens.size() - 1];

			if (funcParameter->identifier == LexerTokens::Comma) {
				if (!expectingComma) {
					this->lexer->error("unexpected comma", -1);
				}

				expectingComma = false;
				continue;
			} else if (funcParameter->identifier == LexerTokens::CloseBrace) {
				if (this->lexer->lex(false, false)) {
					auto arrowToken =
						this->lexer->tokens[this->lexer->tokens.size() - 1];

					if (arrowToken->identifier != LexerTokens::Arrow) {
						this->lexer->error("expected arrow, got '" +
											   LexerTokenNames[static_cast<int>(
												   arrowToken->identifier)] +
											   "'",
										   arrowToken->startChrIndex);
					}
				}

				return token;
			}

			// parse variable declaration
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

		if (fnNameToken->identifier == LexerTokens::Keyword) {
			this->lexer->error(
				"functions are not allowed keywords as their names",
				fnNameToken->startChrIndex);
		} else if (fnNameToken->identifier != LexerTokens::Variable) {
			this->lexer->error(
				"expected variable (as function name), got '" +
					LexerTokenNames[static_cast<int>(fnNameToken->identifier)] +
					"'",
				fnNameToken->startChrIndex);
		}

		if (!this->lexer->lex(false, false)) {
			this->lexer->error(
				"expected open brace to start function arguments", -1);
		}

		auto fnArgsOpenBrace =
			this->lexer->tokens[this->lexer->tokens.size() - 1];

		if (fnArgsOpenBrace->identifier != LexerTokens::OpenBrace) {
			this->lexer->error(
				"expected open brace, got '" +
					LexerTokenNames[static_cast<int>(fnNameToken->identifier)] +
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
		std::cout << "identifier: "
				  << LexerTokenNames[static_cast<int>(token->identifier)]
				  << "\nstartChrIndex: " << token->startChrIndex
				  << "\nchrIndex: " << token->chrIndex
				  << "\nlineNum: " << token->lineNum
				  << "\nvalue: " << token->value << "\n\n";

		switch (token->identifier) {
		case LexerTokens::Keyword:
			parseKeyword(token);
			break;
		default:
			break;
		}
	}

  public:
	Lexer *lexer;

	Ast(std::string filePath) {
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