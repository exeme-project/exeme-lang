// Part of the Exeme Language Project, under the MIT license. See '/LICENSE' for
// license information. SPDX-License-Identifier: MIT License.

#pragma once

#include <string>
#include <type_traits>
#include <variant>
#include <vector>

#include "../lexer/lexer.cpp"
#include "../utils/globals.cpp"
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

static std::vector<std::string> AstTokenNames = {"function definition"};

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
	AstToken *parseFnDecl(const std::string fnName) {
		bool expectingComma = false;

		auto fnToken =
			new AstToken(AstTokens::Identifiers::FN_DEF,
						 new AstTokens::FN_DEF(
							 fnName,
							 nullptr)); // TODO: null variable, not 'nullptr'.

		while (this->lexer->lex(false, true)) {
			auto token = this->lexer->tokens[this->lexer->tokens.size() - 1];

			if (token->identifier == LexerTokens::CloseBrace) {
				if (!this->lexer->lex(false, false)) {
					this->lexer->error("expected '" +
										   LexerTokenNames[static_cast<size_t>(
											   LexerTokens::OpenCurlyBrace)] +
										   "' (as start of function body)",
									   negative_index);
				}

				auto token =
					this->lexer->tokens[this->lexer->tokens.size() - 1];

				if (token->identifier == LexerTokens::Arrow) {
					if (!this->lexer->lex(false, false)) {
						this->lexer->error("expected function return type",
										   negative_index);
					}

					auto token =
						this->lexer->tokens[this->lexer->tokens.size() - 1];

					if (token->identifier != LexerTokens::Variable) {
						this->lexer->error(
							"expected '" +
								LexerTokenNames[static_cast<size_t>(
									LexerTokens::Variable)] +
								"' (as function return type), got '" +
								LexerTokenNames[static_cast<size_t>(
									token->identifier)] +
								"'",
							token->startChrIndex);
					}

					fnToken->FN_DEF->returnType = token;

					if (!this->lexer->lex(false, false)) {
						this->lexer->error(
							"expected '" +
								LexerTokenNames[static_cast<size_t>(
									LexerTokens::OpenCurlyBrace)] +
								"' (as start of function body)",
							negative_index);
					}

					token = this->lexer->tokens[this->lexer->tokens.size() - 1];

					if (token->identifier != LexerTokens::OpenCurlyBrace) {
						this->lexer->error(
							"expected '" +
								LexerTokenNames[static_cast<size_t>(
									LexerTokens::OpenCurlyBrace)] +
								"' (as start of function body), got '" +
								LexerTokenNames[static_cast<size_t>(
									token->identifier)] +
								"'",
							token->startChrIndex);
					}
				} else if (token->identifier != LexerTokens::OpenCurlyBrace) {
					this->lexer->error(
						"expected '" +
							LexerTokenNames[static_cast<size_t>(
								LexerTokens::OpenCurlyBrace)] +
							"' (as start of function body), got '" +
							LexerTokenNames[static_cast<size_t>(
								token->identifier)] +
							"'",
						token->startChrIndex);
				}

				return fnToken;
			} else if (token->identifier == LexerTokens::Comma) {
				if (!expectingComma) {
					this->lexer->error("unexpected '" +
										   LexerTokenNames[static_cast<size_t>(
											   LexerTokens::Comma)] +
										   "'",
									   negative_index);
				}

				expectingComma = false;
				continue;
			} else if (token->identifier != LexerTokens::Variable) {
				this->lexer->error("expected '" +
									   LexerTokenNames[static_cast<size_t>(
										   LexerTokens::Variable)] +
									   "' (as function argument), got '" +
									   LexerTokenNames[static_cast<size_t>(
										   token->identifier)] +
									   "'",
								   token->startChrIndex);
			}

			// TODO: Parser parameter / argument.
		}

		this->lexer->error("unterminated function declaration", negative_index);
	}

	void parseKeywordFn() {
		this->lexer->clearTokens();

		if (!this->lexer->lex(false, false)) {
			this->lexer->error("expected function name", negative_index);
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
							   negative_index);
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

		this->token = this->parseFnDecl(fnNameToken->value);
	}

	void parseKeyword(LexerToken *token) {
		switch (hash(token->value)) {
		case hash("fn"):
			parseKeywordFn();
		}
	}

  public:
	Lexer *lexer;
	struct AstToken *token;

	Ast(std::string filePath) {
		this->lexer = new Lexer(filePath);
		this->token = nullptr;
	}

	bool parse() {
		if (!this->lexer->tokens.empty()) {
			this->lexer->tokens.clear();
		}

		if (!this->lexer->lex(true, true)) {
			return false;
		}

		auto token = this->lexer->tokens[this->lexer->tokens.size() - 1];

		switch (token->identifier) {
		case LexerTokens::Keyword:
			parseKeyword(token);
			break;
		default:
			break;
		}

		return true;
	}
};
