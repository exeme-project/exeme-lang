// Part of the Exeme Language Project, under the MIT license. See '/LICENSE' for
// license information. SPDX-License-Identifier: MIT License.

#pragma once

#include "../lexer/lexer.cpp"
#include "../utils/globals.cpp"
#include "../utils/hash.cpp"

/**
 * The Parser class. Handles logic for parsing a stream of lexer tokens.
 */
class Parser {
  private:
	Lexer *lexer;

	/**
	 * Parses the current import.
	 */
	void parseKeywordImport() {
		if (!this->lexer->lex(false, false)) {
			this->lexer->error("expected import after 'import' keyword",
							   negativeIndex);
		}

		auto import = lexer->tokens[lexer->tokens.size() - 1];

		if (import->identifier != LexerTokens::String) {
			this->lexer->error(
				"expected '" +
					LexerTokenNames[static_cast<size_t>(LexerTokens::String)] +
					"' after 'import' keyword, got '" +
					LexerTokenNames[static_cast<size_t>(import->identifier)] +
					"'",
				import->startChrIndex);
		}
	}

	/**
	 * Calls the correct function for parsing the current keyword.
	 *
	 * @param token The current lexer token.
	 */
	void parseKeyword(LexerToken *token) {
		switch (hash(token->value)) {
		case hash("import"):
			this->parseKeywordImport();
			break;
		}
	}

	/**
	 * Calls the correct function for parsing the current lexer token.
	 *
	 * @param token The current lexer token.
	 */
	void parseNext(LexerToken *token) {
		std::cout << "identifier: "
				  << LexerTokenNames[static_cast<size_t>(token->identifier)]
				  << "\nstartChrIndex: " << token->startChrIndex
				  << "\nchrIndex: " << token->chrIndex
				  << "\nlineNum: " << token->lineNum
				  << "\nvalue: " << token->value << "\n\n";

		switch (token->identifier) {
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

		this->parseNext(lexer->tokens[lexer->tokens.size() - 1]);

		return true;
	}
};
