// Part of the Exeme Language Project, under the MIT license. See '/LICENSE' for
// license information. SPDX-License-Identifier: MIT License.

#pragma once

#include <variant>

#include "../lexer/lexer.cpp"
#include "../utils/globals.cpp"
#include "../utils/hash.cpp"

/**
 * Contains the identifiers of parser tokens and their corresponding parser
 * token.
 */
struct ParserTokens {
	/**
	 * Used to identify different parser tokens.
	 */
	enum class Identifiers { Variable, Function };

	struct Variable {
		std::string identifier;
	};

	struct Function {
		const std::string name;
		const std::vector<Variable> args;

		Function(const std::string name, std::vector<Variable> args)
			: name(name), args(args) {}
	};
}; 

/**
 * Represents a parser token.
 */
struct ParserToken {
	const ParserTokens::Identifiers IDENTIFIER;
	union {
		const ParserTokens::Variable *Variable;
		const ParserTokens::Function *Function;
	};

	ParserToken(ParserTokens::Function *Function)
		: IDENTIFIER(ParserTokens::Identifiers::Function), Function(Function) {}
};

/**
 * The Parser class. Handles logic for parsing a stream of lexer tokens.
 */
class Parser {
  private:
	Lexer *lexer;

	/*
	 * Parses a function's parameters.
	 */
	void parseFnParameters(ParserToken *token) {
		bool expectingComma = false;

		while (this->lexer->lex(true, true)) {
			auto fnParamName = this->lexer->getToken();

			if (expectingComma) {
				if (fnParamName->identifier != LexerTokens::Comma) {
					this->lexer->error("expected '" +
										   LexerTokenNames[static_cast<size_t>(
											   LexerTokens::Comma)] +
										   "' after function parameter",
									   fnParamName);
				}

				expectingComma = false;
				continue;
			}

			if (fnParamName->identifier != LexerTokens::Identifier) {
				this->lexer->error("expected '" +
									   LexerTokenNames[static_cast<size_t>(
										   LexerTokens::Identifier)] +
									   "' as function parameter name, got '" +
									   LexerTokenNames[static_cast<size_t>(
										   fnParamName->identifier)] +
									   "'",
								   fnParamName);
			}

			if (!this->lexer->lex(true, true)) {
				this->lexer->error(
					"expected '" +
						LexerTokenNames[static_cast<size_t>(
							LexerTokens::Colon)] +
						"' to separate function parameter name and type",
					nullptr);
			}

			auto colon = this->lexer->getToken();
		}

		this->lexer->error("unterminated function", nullptr);
	}

	/**
	 * Parses the current function.
	 */
	void parseFn(const std::string fnName) {
		auto token = new ParserToken(new ParserTokens::Function(
			fnName, std::vector<ParserTokens::Variable>{}));

		if (!this->lexer->lex(true, false)) {
			this->lexer->error("expected '" +
								   LexerTokenNames[static_cast<size_t>(
									   LexerTokens::OpenBrace)] +
								   "'",
							   nullptr);
		}

		auto openBrace = this->lexer->getToken();

		if (openBrace->identifier != LexerTokens::OpenBrace) {
			this->lexer->error("expected '" +
								   LexerTokenNames[static_cast<size_t>(
									   LexerTokens::OpenBrace)] +
								   "' after function name, got '" +
								   LexerTokenNames[static_cast<size_t>(
									   openBrace->identifier)] +
								   "'",
							   openBrace);
		}

		this->lexer->clearTokens();

		this->parseFnParameters(token);

		this->lexer->error("unterminated function", nullptr);
	}

	/**
	 * Parses the current keyword 'fn'.
	 */
	void parseKeywordFn() {
		if (!this->lexer->lex(true, false)) {
			this->lexer->error("expected function name after 'fn' keyword",
							   nullptr);
		}

		auto fnName = this->lexer->getToken();

		if (fnName->identifier != LexerTokens::Identifier) {
			this->lexer->error(
				"expected '" +
					LexerTokenNames[static_cast<size_t>(
						LexerTokens::Identifier)] +
					"' after 'fn' keyword, got '" +
					LexerTokenNames[static_cast<size_t>(fnName->identifier)] +
					"'",
				fnName);
		}

		this->parseFn(fnName->value);

		// TODO: Add function definition to parser token.
	}

	/**
	 * Parses the current keyword 'import'.
	 */
	void parseKeywordImport() {
		if (!this->lexer->lex(true, false)) {
			this->lexer->error("expected import after 'import' keyword",
							   nullptr);
		}

		auto import = this->lexer->getToken();

		if (import->identifier != LexerTokens::String) {
			this->lexer->error(
				"expected '" +
					LexerTokenNames[static_cast<size_t>(LexerTokens::String)] +
					"' after 'import' keyword, got '" +
					LexerTokenNames[static_cast<size_t>(import->identifier)] +
					"'",
				import);
		}

		auto importPath = import->value;

		// TODO: Import module.
	}

	/**
	 * Parses the current keyword 'using'.
	 */
	void parseKeywordUsing() {
		this->lexer->clearTokens();

		if (!this->lexer->lex(true, false)) {
			this->lexer->error("expected namespace / identifier in namespace "
							   "after 'using' keyword",
							   nullptr);
		}

		while (this->lexer->lex(true, false)) {
			// TODO: Parse tokens.
		}
	}

	/**
	 * Calls the correct function for parsing the current keyword.
	 *
	 * @param token The current lexer token.
	 */
	void parseKeyword(const LexerToken *token) {
		switch (hash(token->value)) {
		case hash("fn"):
			this->parseKeywordFn();
			break;
		case hash("import"):
			this->parseKeywordImport();
			break;
		case hash("using"):
			this->parseKeywordUsing();
			break;
		}
	}

	/**
	 * Calls the correct function for parsing the current lexer token.
	 *
	 * @param token The current lexer token.
	 */
	void parseNext(const LexerToken *token) {
		std::cout << "identifier: "
				  << LexerTokenNames[static_cast<size_t>(token->identifier)]
				  << "\nstartChrIndex: " << token->startChrIndex
				  << "\nchrIndex: " << token->chrIndex
				  << "\nlineNum: " << token->lineNum
				  << "\nvalue: " << token->value << "\n\n";

		switch (token->identifier) {
		case LexerTokens::Chr:
		case LexerTokens::Comment:
		case LexerTokens::Float:
		case LexerTokens::String:
		case LexerTokens::Identifier:
			break;
		case LexerTokens::Keyword:
			this->parseKeyword(token);
			break;
		}
	}

  public:
	/**
	 * Construct a new Parser object
	 *
	 * @param filePath - The path to the file to parse.
	 */
	Parser(std::string filePath) { this->lexer = new Lexer(filePath); }

	/**
	 * Handles getting the next lexer token and parsing it.
	 *
	 * @return bool - Whether parsing succeeded.
	 */
	bool parse() {
		if (!this->lexer->lex(true, true)) {
			return false;
		}

		this->parseNext(this->lexer->getToken());

		return true;
	}
};
