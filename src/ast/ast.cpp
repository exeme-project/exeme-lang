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

struct AstToken;

struct AstTokens {
	enum class Identifiers { FN_DEF, Equals };

	struct FN_DEF {
		std::string fnName;
		LexerToken *returnType;

		FN_DEF(std::string fnName, LexerToken *returnType) {
			this->fnName = fnName;
			this->returnType = returnType;
		}
	};

	struct Equals {
		AstToken *op1;
		AstToken *op2;

		Equals(AstToken *op1, AstToken *op2) {
			this->op1 = op1;
			this->op2 = op2;
		}
	};
};

static std::vector<std::string> AstTokenNames = {"function definition"};

struct AstToken {
	AstTokens::Identifiers identifier;
	union {
		AstTokens::FN_DEF *FN_DEF;
		AstTokens::Equals *Equals;
	};

	AstToken(AstTokens::Identifiers identifier, AstTokens::FN_DEF *FN_DEF) {
		this->identifier = identifier;
		this->FN_DEF = FN_DEF;
	}

	AstToken(AstTokens::Identifiers identifier, AstTokens::Equals *Equals) {
		this->identifier = identifier;
		this->Equals = Equals;
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
									   negativeIndex);
				}

				auto token =
					this->lexer->tokens[this->lexer->tokens.size() - 1];

				if (token->identifier == LexerTokens::Arrow) {
					if (!this->lexer->lex(false, false)) {
						this->lexer->error("expected function return type",
										   negativeIndex);
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
							negativeIndex);
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

				this->lexer->clearTokens();
				return fnToken;
			} else if (token->identifier == LexerTokens::Comma) {
				if (!expectingComma) {
					this->lexer->error("unexpected '" +
										   LexerTokenNames[static_cast<size_t>(
											   LexerTokens::Comma)] +
										   "'",
									   negativeIndex);
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

		this->lexer->error("unterminated function declaration", negativeIndex);
	}

	void parseKeywordFn() {
		this->lexer->clearTokens();

		if (!this->lexer->lex(false, false)) {
			this->lexer->error("expected function name", negativeIndex);
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
							   negativeIndex);
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

	void parseEquals() {
		auto tokensSize = this->lexer->tokens.size();

		if (tokensSize < 2) {
			this->lexer->error(
				"missing first operand for '" +
					LexerTokenNames[static_cast<size_t>(LexerTokens::Equals)] +
					"'",
				0);
		}

		auto token = new AstToken(AstTokens::Identifiers::Equals,
								  new AstTokens::Equals(nullptr, nullptr));

		if (this->token == nullptr) {
			// TODO: Convert 'this->token' to an 'AstToken*' type. Implement a
			// function for this that calls the token's respective converter.
		} else {
			token->Equals->op1 = this->token;
			this->token = nullptr;
		}

		// TODO: Get the second operand of the assignment operator.
	}

	void parseToken(LexerToken *token) {
		switch (token->identifier) {
		case LexerTokens::Keyword:
			parseKeyword(token);
			break;
		case LexerTokens::Equals:
			parseEquals();
			break;
		default:
			break;
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
		if (!this->lexer->lex(false, false)) { // New line
			if (TESTING == 0) {
				if (!this->lexer->tokens.empty()) {
					auto token =
						this->lexer->tokens[this->lexer->tokens.size() - 1];

					this->lexer->lineNum = token->lineNum;
					this->lexer->chrIndex = token->chrIndex;
					this->lexer->error("token has no effect",
									   token->startChrIndex);
				}
			}

			if (!this->lexer->lex(true, true)) {
				return false;
			}
		}

		this->parseToken(this->lexer->tokens[this->lexer->tokens.size() - 1]);

		return true;
	}
};
