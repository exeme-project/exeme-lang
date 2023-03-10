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
	enum class Identifiers { Function };

	struct Function {
		const std::string fnName;

		Function(const std::string fnName) : fnName(fnName) {}
	};
};

/**
 * Represents a parser token.
 */
struct ParserToken {
	const ParserTokens::Identifiers IDENTIFIER;
	union {
		const ParserTokens::Function *Function;
	};

	ParserToken(const ParserTokens::Identifiers IDENTIFIER,
				ParserTokens::Function *Function)
		: IDENTIFIER(IDENTIFIER), Function(Function) {}
};

/**
 * The Parser class. Handles logic for parsing a stream of lexer tokens.
 */
class Parser {
  private:
	Lexer *lexer;

	/**
	 * Parses the current function.
	 */
	void parseFn(const std::string fnName) {
		auto token = new ParserToken(ParserTokens::Identifiers::Function,
									 new ParserTokens::Function(fnName));

		if (!this->lexer->lex(true, false)) {
			this->lexer->error("expected '" +
								   LexerTokenNames[static_cast<size_t>(
									   LexerTokens::OpenBrace)] +
								   "'",
							   negativeIndex);
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
							   negativeIndex);
		}

		this->lexer->clearTokens();

		while (this->lexer->lex(true, false)) {
		}

		this->lexer->error("unterminated function", negativeIndex);
	}

	/**
	 * Parses the current keyword 'fn'.
	 */
	void parseKeywordFn() {
		if (!this->lexer->lex(true, false)) {
			this->lexer->error("expected function name after 'fn' keyword",
							   negativeIndex);
		}

		auto fnName = this->lexer->getToken();

		if (fnName->identifier != LexerTokens::Variable) {
			this->lexer->error(
				"expected '" +
					LexerTokenNames[static_cast<size_t>(
						LexerTokens::Variable)] +
					"' after 'fn' keyword, got '" +
					LexerTokenNames[static_cast<size_t>(fnName->identifier)] +
					"'",
				negativeIndex);
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
							   negativeIndex);
		}

		auto import = this->lexer->getToken();

		if (import->identifier != LexerTokens::String) {
			this->lexer->error(
				"expected '" +
					LexerTokenNames[static_cast<size_t>(LexerTokens::String)] +
					"' after 'import' keyword, got '" +
					LexerTokenNames[static_cast<size_t>(import->identifier)] +
					"'",
				import->startChrIndex);
		}

		auto importPath = import->value;

		// TODO: Import module
	}

	/**
	 * Parses the current keyword 'using'.
	 */
	void parseKeywordUsing() {
		this->lexer->clearTokens();

		if (!this->lexer->lex(true, false)) {
			this->lexer->error("expected namespace / variable in namespace "
							   "after 'using' keyword",
							   negativeIndex);
		}

		while (this->lexer->lex(true, false)) {
			// TODO: Parse tokens
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
		case LexerTokens::Variable:
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
