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
	enum class Identifiers { Variable, Fn_Def, Equals };

	struct Variable {
		std::string name;

		Variable(std::string name) { this->name = name; }
	};

	struct Fn_Def {
		std::string fnName;
		LexerToken *returnType;

		Fn_Def(std::string fnName, LexerToken *returnType) {
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

static std::vector<std::string> AstTokenNames = {
	"variable", "function definition", "assignment"};

struct AstToken {
	AstTokens::Identifiers identifier;
	union {
		AstTokens::Variable *Variable;
		AstTokens::Fn_Def *Fn_Def;
		AstTokens::Equals *Equals;
	};

	AstToken(AstTokens::Identifiers identifier, AstTokens::Variable *Variable) {
		this->identifier = identifier;
		this->Variable = Variable;
	}

	AstToken(AstTokens::Identifiers identifier, AstTokens::Fn_Def *Fn_Def) {
		this->identifier = identifier;
		this->Fn_Def = Fn_Def;
	}

	AstToken(AstTokens::Identifiers identifier, AstTokens::Equals *Equals) {
		this->identifier = identifier;
		this->Equals = Equals;
	}
};

class Ast {
  private:
	AstToken *convertToken(LexerToken *token) {
		switch (token->identifier) {
		case LexerTokens::Variable:
			return new AstToken(AstTokens::Identifiers::Variable,
								new AstTokens::Variable(token->value));
		default:
			std::cout << "NotImplemented: Cannot convert '"
					  << LexerTokenNames[static_cast<size_t>(token->identifier)]
					  << "' to an 'AstToken*' yet.";
			return nullptr;
		}
	}

	AstToken *parseFnDecl(const std::string fnName) {
		bool expectingComma = false;

		auto fnToken =
			new AstToken(AstTokens::Identifiers::Fn_Def,
						 new AstTokens::Fn_Def(
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

					fnToken->Fn_Def->returnType = token;

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

	void parseAssignment() {
		auto token = new AstToken(AstTokens::Identifiers::Equals,
								  new AstTokens::Equals(nullptr, nullptr));

		if (this->token == nullptr) {
			auto tokensSize = this->lexer->tokens.size();

			if (tokensSize < 2) { // 2 because of assignment operator token.
				this->lexer->error("missing first operand for '" +
									   LexerTokenNames[static_cast<size_t>(
										   LexerTokens::EqualTo)] +
									   "'",
								   0);
			}

			token->Equals->op1 =
				this->convertToken(this->lexer->tokens[tokensSize - 2]);
		} else {
			token->Equals->op1 = this->token;
			this->token = nullptr;
		}

		// TODO: Get the second operand of the assignment operator.

		this->token = token;
	}

	void parseToken(LexerToken *token) {
		switch (token->identifier) {
		case LexerTokens::Keyword:
			parseKeyword(token);
			break;
		case LexerTokens::Assignment:
			parseAssignment();
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
