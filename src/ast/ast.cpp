// Part of the Exeme Language Project, under the MIT license. See '/LICENSE' for
// license information. SPDX-License-Identifier: MIT License.

#pragma once

#include <string>
#include <type_traits>
#include <variant>
#include <vector>

#include "../lexer/lexer.cpp"
#include "../utils/hash.cpp"

struct AstTokens {
	enum class Identifiers { FN_DEF };

	struct FN_DEF {
		std::string fnName;
		LexerToken *returnType;

		FN_DEF(std::string fnName, LexerToken *returnType) {
			this->fnName = fnName;
			this->returnType = returnType;
		}
	};
};

struct AstToken {
	AstTokens::Identifiers identifier;
	union {
		AstTokens::FN_DEF *FN_DEF;
	};

	AstToken(AstTokens::Identifiers identifier, AstTokens::FN_DEF *FN_DEF) {
		this->identifier = identifier;
		this->FN_DEF = FN_DEF;
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

		auto token =
			new AstToken(AstTokens::Identifiers::FN_DEF,
						 new AstTokens::FN_DEF(
							 fnName, NULL)); // TODO: null variable, not 'NULL'.

		while (this->lexer->lex(true, true)) {
			auto funcParameter =
				this->lexer->tokens[this->lexer->tokens.size() - 1];

			if (funcParameter->identifier == LexerTokens::Comma) {
				if (!expectingComma) {
					this->lexer->error("unexpected '" +
										   LexerTokenNames[static_cast<size_t>(
											   LexerTokens::Comma)] +
										   "'",
									   -1);
				}

				expectingComma = false;
				continue;
			} else if (funcParameter->identifier == LexerTokens::CloseBrace) {
				if (this->lexer->lex(false, false)) {
					auto arrowToken =
						this->lexer->tokens[this->lexer->tokens.size() - 1];

					if (arrowToken->identifier == LexerTokens::OpenCurlyBrace) {
						break; // TODO: Un-get last token?
					} else if (arrowToken->identifier != LexerTokens::Arrow) {
						this->lexer->error(
							"expected '" +
								LexerTokenNames[static_cast<size_t>(
									LexerTokens::Arrow)] +
								"', got '" +
								LexerTokenNames[static_cast<size_t>(
									arrowToken->identifier)] +
								"'",
							arrowToken->startChrIndex);
					}

					if (!this->lexer->lex(false, false)) {
						this->lexer->error(
							"expected function return type to follow '" +
								LexerTokenNames[static_cast<size_t>(
									LexerTokens::Arrow)] +
								"'",
							arrowToken->chrIndex);
					}

					auto fnReturn =
						this->lexer->tokens[this->lexer->tokens.size() - 1];

					if (fnReturn->identifier != LexerTokens::Variable) {
						this->lexer->error(
							"expected '" +
								LexerTokenNames[static_cast<size_t>(
									LexerTokens::Variable)] +
								"', got '" +
								LexerTokenNames[static_cast<size_t>(
									fnReturn->identifier)] +
								"'",
							fnReturn->startChrIndex);
					}

					token->FN_DEF->returnType = fnReturn;
				}

				while (this->lexer->lex(true, true)) {
					auto lexerToken =
						this->lexer->tokens[this->lexer->tokens.size() - 1];

					if (lexerToken->identifier != LexerTokens::OpenCurlyBrace) {
						this->lexer->error(
							"expected '" +
								LexerTokenNames[static_cast<size_t>(
									LexerTokens::OpenCurlyBrace)] +
								"', got '" +
								LexerTokenNames[static_cast<size_t>(
									lexerToken->identifier)] +
								"'",
							lexerToken->startChrIndex);
					}

					break;
				}

				return token;
			}

			// TODO: parse variable declaration
			// TODO: check for default parameter value
			// TODO: set 'expectingComma' to true
		}

		this->lexer->error("unterminated function " + functionType,
						   openBrace->startChrIndex);
		return NULL;
	}

	void parseKeywordFn() {
		this->lexer->clearTokens();

		if (!this->lexer->lex(false, false)) {
			this->lexer->error("expected function name", -1);
		}

		auto fnNameToken = this->lexer->tokens[this->lexer->tokens.size() - 1];

		if (fnNameToken->identifier == LexerTokens::Keyword) {
			this->lexer->error(
				"functions are not allowed '" +
					LexerTokenNames[static_cast<size_t>(LexerTokens::Keyword)] +
					"s' as their names",
				fnNameToken->startChrIndex);
		} else if (fnNameToken->identifier != LexerTokens::Variable) {
			this->lexer->error("expected '" +
								   LexerTokenNames[static_cast<size_t>(
									   LexerTokens::Variable)] +
								   "' (as function name), got '" +
								   LexerTokenNames[static_cast<size_t>(
									   fnNameToken->identifier)] +
								   "'",
							   fnNameToken->startChrIndex);
		}

		if (!this->lexer->lex(false, false)) {
			this->lexer->error("expected " +
								   LexerTokenNames[static_cast<size_t>(
									   LexerTokens::OpenBrace)] +
								   "to start function arguments",
							   -1);
		}

		auto fnArgsOpenBrace =
			this->lexer->tokens[this->lexer->tokens.size() - 1];

		if (fnArgsOpenBrace->identifier != LexerTokens::OpenBrace) {
			this->lexer->error("expected '" +
								   LexerTokenNames[static_cast<size_t>(
									   LexerTokens::OpenBrace)] +
								   "', got '" +
								   LexerTokenNames[static_cast<size_t>(
									   fnNameToken->identifier)] +
								   "'",
							   fnArgsOpenBrace->startChrIndex);
		}

		this->token =
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
				  << LexerTokenNames[static_cast<size_t>(token->identifier)]
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