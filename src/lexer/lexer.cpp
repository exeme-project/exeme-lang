// Part of the Exeme Language Project, under the MIT license. See '/LICENSE' for
// license information. SPDX-License-Identifier: MIT License.

#pragma once

#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

#include "../utils/console.cpp"
#include "../utils/globals.cpp"

static std::vector<std::string> KEYWORDS = {
	"break", "catch", "class", "continue", "elif",	 "else", "finally",
	"fn",	 "for",	  "if",	   "import",   "return", "try",	 "while"};
static auto KEYWORDS_BEGIN = KEYWORDS.begin();
static auto KEYWORDS_END = KEYWORDS.end();

/**
 * Used to identify different tokens.
 */
enum class LexerTokens {
	Keyword,
	Variable,

	// Data types
	Chr,
	String,
	Integer,
	Float,

	// Syntactic constructs
	OpenBrace,		  // '('
	OpenSqaureBrace,  // '['
	OpenCurlyBrace,	  // '{'
	CloseBrace,		  // ')'
	CloseSquareBrace, // ']'
	CloseCurlyBrace,  // '}'
	Comma,			  // ','
	Period,			  // '.'
	At,				  // '@'
	Hashtag,		  // '#'
	Arrow,			  // '->'

	// Operators (order is important)
	Exponent,	 // '**'
	Divide,		 // '/'
	FloorDivide, // '//'
	Multiply,	 // '*'
	Add,		 // '+'
	Subtract,	 // '-'
	Modulo,		 // '%'

	// Relational operators
	GreaterThan,	   // '>'
	GreaterThanEquals, // '>='
	LessThan,		   // '<'
	LessThanEquals,	   // '<='

	// Assignment operators
	Equals,			   // '='
	ExponentEquals,	   // '**='
	DivideEquals,	   // '/='
	FloorDivideEquals, // '//='
	MultiplyEquals,	   // '*='
	AddEquals,		   // '+='
	SubtractEquals,	   // '-='
	ModuloEquals,	   // '%='

	// Logical operators
	And, // '&&'
	Or,	 // '||'
	Not, // '!'

	// Comparison operators
	Equality,  // '=='
	NotEquals, // '!='

	// Bitwise operators
	AND,		// '&'
	OR,			// '|'
	XOR,		// '^'
	LEFTSHIFT,	// '<<'
	RIGHTSHIFT, // '>>'
};

/**
 * Contains the names of each of the lexer token identifiers.
 */
static std::vector<std::string> LexerTokenNames = {
	"keyword",
	"variable",
	"char",
	"string",
	"integer",
	"float",
	"open brace",
	"open square brace",
	"open curly brace",
	"close brace",
	"close sqaure brace",
	"close curly brace",
	"comma",
	"period",
	"at symbol",
	"hashtag",
	"arrow",
	"exponent symbol",
	"divide symbol",
	"floor divide symbol",
	"multiply symbol",
	"add symbol",
	"subtract symbol",
	"modulo symbol",
	"greater than symbol",
	"greater than equals symbol",
	"less than symbol",
	"less than equals symbol",
	"equals symbol",
	"exponent equals symbol",
	"divide equals symbol",
	"floor divide equals symbol",
	"multiply equals symbol",
	"add equals symbol",
	"subtract equals symbol",
	"modulo equals symbol",
	"and symbol",
	"or symbol",
	"not symbol",
	"equality comparison symbol",
	"not equals comparison symbol",
	"bitwise AND",
	"bitwise OR",
	"bitwise XOR",
	"bitwise left shift",
	"bitwise right shift",
	"end of line",
};

/**
 * Contains data relating to a lexer token.
 */
struct LexerToken {
	LexerTokens identifier;
	size_t startChrIndex, chrIndex, lineNum;
	std::string value;

	/**
	 * Construct a new Lexer Token object.
	 *
	 * @param identifier    - Token identifier.
	 * @param startChrIndex - Start char index of token.
	 * @param chrIndex      - End char index of the token.
	 * @param lineNum       - Line num of the token.
	 * @param value         - Value of the token.
	 */
	LexerToken(LexerTokens identifier, size_t startChrIndex, size_t chrIndex,
			   size_t lineNum, std::string value) {
		this->identifier = identifier;
		this->startChrIndex = startChrIndex;
		this->chrIndex = chrIndex;
		this->lineNum = lineNum;
		this->value = value;
	}
};

/**
 * The Lexer class. Handles logic for lexing a file.
 */
class Lexer {
  private:
	bool closed;
	char chr, prevChr;
	std::string filePath;
	std::ifstream file;

	/**
	 * Get the next character.
	 *
	 * @param skipWhitespace - Whether to skip whitespace
	 * characters.
	 *
	 * @return bool - Whether the next character was got successfully.
	 */
	bool getChr(bool skipWhitespace) {
		if (this->closed) {
			return false;
		} else if (this->chr == '\n') {
			return false;
		}

		while (true) {
			this->prevChr = this->chr;

			if (!this->file.get(this->chr)) { // End of file (hopefully)
				this->file.close();
				this->closed = true;
				return false;
			}

			this->chrIndex++;

			if (this->chr == '\n') {
				return false;
			}

			if (skipWhitespace) { // Keep going till we encounter non whitespace
				if (!isspace(this->chr)) { // Not whitespace
					break;
				}
			} else {
				break;
			}
		}

		return true;
	}

	/**
	 * Un-get the current character.
	 *
	 * @return bool - Whether the current character was un-got successfully.
	 */
	bool unGetChr() {
		if (this->closed) {
			return false;
		}

		this->chrIndex--;
		this->chr = this->prevChr;
		this->prevChr = '\0';

		this->file.unget();

		if (this->file.fail()) {
			this->file.close();
			std::cout << "failed to unget char in " << this->filePath;
			exit(EXIT_FAILURE);
		}

		return true;
	}

	/**
	 * Get the next line.
	 *
	 * @param getNext - Get the next line even if the EOL has not been
	 * reached.
	 *
	 * @return bool - Whether the next line was got successfully.
	 */
	bool getLine(bool getNext) {
		if (this->closed) {
			return false;
		}

		if (!getNext &&
			this->chr != '\n') { // Not reached the end of the line yet
			return true;
		}

		this->prevChr = this->chr;
		this->chr = '\0';

		this->chrIndex = static_cast<size_t>(negativeIndex);
		this->lineNum++;

		if (!this->getChr(
				true)) { // Read chars till non-whitespace is encountered
			return !this->file.fail();
		}

		this->unGetChr();

		return true;
	}

	/**
	 * Returns the escaped version of the current character.
	 *
	 * @return char - The escaped version of the current character.
	 */
	char escapeChr() {
		switch (this->chr) {
		case 'b':
			return '\b';
		case 'f':
			return '\f';
		case 'n':
			return '\n';
		case 't':
			return '\t';
		case 'v':
			return '\v';
		case '\'':
			return '\'';
		case '"':
			return '"';
		case '\\':
			return '\\';
		}

		this->error("invalid escape sequence", this->chrIndex - 1);
	}

	/**
	 * Checks for repetition of the current character.
	 *
	 * @param ifNotRepeated - Identifier of token if
	 * repetition was not found.
	 * @param ifRepeated    - Identifier of token if
	 * repetition was found.
	 *
	 * @return LexerToken* - The created token.
	 */
	LexerToken *checkForRepetition(LexerTokens ifNotRepeated,
								   LexerTokens ifRepeated) {
		bool repeated = true;
		char chr = this->chr;

		if (this->getChr(false)) {
			if (this->chr != chr) {
				repeated = false;
				this->unGetChr();
			}
		} else {
			repeated = false;
		}

		if (repeated) {
			return new LexerToken(ifRepeated, this->chrIndex - 1,
								  this->chrIndex, this->lineNum,
								  std::string(2, this->chr));
		} else {
			return new LexerToken(ifNotRepeated, this->chrIndex, this->chrIndex,
								  this->lineNum, std::string(1, this->chr));
		}
	}

	/**
	 * Checks for a trailing character.
	 *
	 * @param token - The current token.
	 *
	 * @return bool - Whether the trailing character was found.
	 */
	bool checkForTrailingChr(LexerToken *token, char chr) {
		bool found = true;

		if (this->getChr(false)) {
			if (this->chr != chr) {
				this->unGetChr();
				found = false;
			}
		} else {
			found = false;
		}

		if (found) {
			token->chrIndex++;
			token->value += std::string(1, chr);
		}

		return found;
	}

	/**
	 * Checks for an unexpected continuation of the current token.
	 *
	 * @param token - The current token.
	 */
	void checkForContinuation(std::string token) {
		if (this->getChr(false)) {
			if (!isspace(this->chr)) {
				this->error("unexpected continuation of token '" + token + "'",
							negativeIndex);
			}

			this->unGetChr();
		}
	}

	/**
	 * Lex a '=' token.
	 */
	void lexEquals() {
		LexerToken *token = this->checkForRepetition(LexerTokens::Equals,
													 LexerTokens::Equality);

		this->tokens.emplace_back(token);

		this->checkForContinuation(token->value);
	}

	/**
	 * Lex a character.
	 */
	void lexChr() {
		bool includeChr = false;
		size_t startChrIndex = this->chrIndex;
		std::string chr;
		size_t chrLen = 0;

		while (this->getChr(false)) {
			if (chrLen > 1) {
				this->error("multi-character char", startChrIndex);
			}

			if (this->chr == '\'') {
				this->tokens.emplace_back(
					new LexerToken(LexerTokens::Chr, startChrIndex,
								   this->chrIndex, this->lineNum, chr));
				return;
			}

			chrLen++;

			if (includeChr) {
				chr += this->escapeChr();
				includeChr = false;
			} else if (this->chr != '\\') {
				chr += this->chr;
			} else {
				chrLen--;
				includeChr = true;
			}
		}

		this->unGetChr();
		this->error("unterminated char", startChrIndex);
	}

	/**
	 * Lex a string.
	 */
	void lexString() {
		bool includeChr = false;
		size_t startChrIndex = this->chrIndex;
		std::string string = "";

		while (this->getChr(false)) {
			if (this->chr == '"') {
				this->tokens.emplace_back(
					new LexerToken(LexerTokens::String, startChrIndex,
								   this->chrIndex, this->lineNum, string));
				return;
			}

			if (includeChr) {
				string += this->escapeChr();
				includeChr = false;
			} else if (this->chr == '\\') {
				includeChr = true;
			} else {
				string += this->chr;
			}
		}

		this->unGetChr();
		this->error("unterminated string", startChrIndex);
	}

	/**
	 * Lex a float.
	 *
	 * @param startChrIndex - The index of the first character of
	 * the float.
	 * @param integer       - The float's value (float is a C++
	 * keyword).
	 */
	void lexFloat(size_t startChrIndex, std::string integer) {
		while (this->getChr(false)) {
			if (isspace(this->chr) ||
				(this->chr != '.' && !isalpha(this->chr))) {
				this->unGetChr();
				break;
			}

			if (this->chr == '.') {
				this->error("too many decimal points for float", negativeIndex);
			} else if (!isdigit(this->chr)) {
				this->error("invalid character for integer", negativeIndex);
			}

			integer += this->chr;
		}

		this->tokens.emplace_back(new LexerToken(LexerTokens::Float,
												 startChrIndex, this->chrIndex,
												 this->lineNum, integer));
	}

	/**
	 * Lex an integer. Hands control over to 'lexFloat()' if needed.
	 */
	void lexInteger() {
		size_t startChrIndex = this->chrIndex;
		std::string integer(1, this->chr);

		while (this->getChr(false)) {
			if (isspace(this->chr) ||
				(this->chr != '.' && !isalpha(this->chr))) {
				this->unGetChr();
				break;
			}

			integer += this->chr;

			if (this->chr == '.') {
				return lexFloat(startChrIndex, integer);
			} else if (!isdigit(this->chr)) {
				this->error("invalid character for integer", negativeIndex);
			}
		}

		this->tokens.emplace_back(new LexerToken(LexerTokens::Integer,
												 startChrIndex, this->chrIndex,
												 this->lineNum, integer));
	}

	/**
	 * Lex a keyword or variable.
	 */
	void lexKeywordOrVariable() {
		size_t startChrIndex = this->chrIndex;
		std::string string(1, this->chr);

		while (this->getChr(false)) {
			if (!isalnum(this->chr)) {
				this->unGetChr();
				break;
			}

			string += this->chr;
		}

		this->tokens.emplace_back(new LexerToken(
			std::find(KEYWORDS_BEGIN, KEYWORDS_END, string) == KEYWORDS_END
				? LexerTokens::Variable
				: LexerTokens::Keyword,
			startChrIndex,
			this->chr == '\n' ? this->chrIndex - 1 : this->chrIndex,
			this->lineNum, string));
	}

	/**
	 * Lex '*'.
	 */
	void lexMultiply() {
		LexerToken *token = this->checkForRepetition(LexerTokens::Multiply,
													 LexerTokens::Exponent);

		if (this->checkForTrailingChr(token, '=')) {
			token->identifier = token->identifier == LexerTokens::Multiply
									? LexerTokens::MultiplyEquals
									: LexerTokens::ExponentEquals;
		}

		this->tokens.emplace_back(token);
		this->checkForContinuation(token->value);
	}

	/**
	 * Lex '/'.
	 */
	void lexDivide() {
		LexerToken *token = this->checkForRepetition(LexerTokens::Divide,
													 LexerTokens::FloorDivide);

		if (this->checkForTrailingChr(token, '=')) {
			token->identifier = token->identifier == LexerTokens::Divide
									? LexerTokens::DivideEquals
									: LexerTokens::FloorDivideEquals;
		}

		this->tokens.emplace_back(token);
		this->checkForContinuation(token->value);
	}

	/**
	 * Lex '+'.
	 */
	void lexAdd() {
		LexerToken *token =
			new LexerToken(LexerTokens::Add, this->chrIndex, this->chrIndex,
						   this->lineNum, std::string(1, this->chr));

		if (this->checkForTrailingChr(token, '=')) {
			token->identifier = LexerTokens::AddEquals;
		}

		this->tokens.emplace_back(token);
		this->checkForContinuation(token->value);
	}

	/**
	 * Lex '-'.
	 */
	void lexSubtract() {
		LexerToken *token = new LexerToken(
			LexerTokens::Subtract, this->chrIndex, this->chrIndex,
			this->lineNum, std::string(1, this->chr));

		if (this->checkForTrailingChr(token, '=')) {
			token->identifier = LexerTokens::SubtractEquals;
		} else if (this->checkForTrailingChr(token, '>')) {
			token->identifier = LexerTokens::Arrow;
		}

		this->tokens.emplace_back(token);
		this->checkForContinuation(token->value);
	}

	/**
	 * Lex '%'.
	 */
	void lexModulo() {
		LexerToken *token =
			new LexerToken(LexerTokens::Modulo, this->chrIndex, this->chrIndex,
						   this->lineNum, std::string(1, this->chr));

		if (this->checkForTrailingChr(token, '=')) {
			token->identifier = LexerTokens::ModuloEquals;
		}

		this->tokens.emplace_back(token);
		this->checkForContinuation(token->value);
	}

	/**
	 * Lex '>'.
	 */
	void lexGreaterThan() {
		LexerToken *token = new LexerToken(
			LexerTokens::GreaterThan, this->chrIndex, this->chrIndex,
			this->lineNum, std::string(1, this->chr));

		if (this->checkForTrailingChr(token, '=')) {
			token->identifier = LexerTokens::GreaterThanEquals;
		} else if (this->checkForTrailingChr(token, '>')) {
			token->identifier = LexerTokens::RIGHTSHIFT;
		}

		this->tokens.emplace_back(token);
		this->checkForContinuation(token->value);
	}

	/**
	 * Lex '<'.
	 */
	void lexLessThan() {
		LexerToken *token = new LexerToken(
			LexerTokens::LessThan, this->chrIndex, this->chrIndex,
			this->lineNum, std::string(1, this->chr));

		if (this->checkForTrailingChr(token, '=')) {
			token->identifier = LexerTokens::LessThanEquals;
		} else if (this->checkForTrailingChr(token, '>')) {
			token->identifier = LexerTokens::LEFTSHIFT;
		}

		this->tokens.emplace_back(token);
		this->checkForContinuation(token->value);
	}

	/**
	 * Lex '&'.
	 */
	void lexAnd() {
		LexerToken *token =
			new LexerToken(LexerTokens::AND, this->chrIndex, this->chrIndex,
						   this->lineNum, std::string(1, this->chr));

		if (this->checkForTrailingChr(token, '&')) {
			token->identifier = LexerTokens::And;
		}

		this->tokens.emplace_back(token);
		this->checkForContinuation(token->value);
	}

	/**
	 * Lex '|'.
	 */
	void lexOr() {
		LexerToken *token =
			new LexerToken(LexerTokens::Or, this->chrIndex, this->chrIndex,
						   this->lineNum, std::string(1, this->chr));

		if (this->checkForTrailingChr(token, '|')) {
			token->identifier = LexerTokens::OR;
		}

		this->tokens.emplace_back(token);
		this->checkForContinuation(token->value);
	}

	/**
	 * Lex '!'.
	 */
	void lexNot() {
		LexerToken *token =
			new LexerToken(LexerTokens::Not, this->chrIndex, this->chrIndex,
						   this->lineNum, std::string(1, this->chr));

		if (this->checkForTrailingChr(token, '=')) {
			token->identifier = LexerTokens::NotEquals;
		}

		this->tokens.emplace_back(token);
		this->checkForContinuation(token->value);
	}

	/**
	 * Calls the correct function for lexing the current token.
	 *
	 * @return bool - Whether lexing succeeded.
	 */
	bool lexNext() {
		switch (this->chr) {
		case '\'':
			this->lexChr();
			break;
		case '"':
			this->lexString();
			break;
		case '=':
			this->lexEquals();
			break;
		case '(':
			this->tokens.emplace_back(
				new LexerToken(LexerTokens::OpenBrace, this->chrIndex,
							   this->chrIndex, this->lineNum, ""));
			break;
		case '[':
			this->tokens.emplace_back(
				new LexerToken(LexerTokens::OpenSqaureBrace, this->chrIndex,
							   this->chrIndex, this->lineNum, ""));
			break;
		case '{':
			this->tokens.emplace_back(
				new LexerToken(LexerTokens::OpenCurlyBrace, this->chrIndex,
							   this->chrIndex, this->lineNum, ""));
			break;
		case ')':
			this->tokens.emplace_back(
				new LexerToken(LexerTokens::CloseBrace, this->chrIndex,
							   this->chrIndex, this->lineNum, ""));
			break;
		case ']':
			this->tokens.emplace_back(
				new LexerToken(LexerTokens::CloseSquareBrace, this->chrIndex,
							   this->chrIndex, this->lineNum, ""));
			break;
		case '}':
			this->tokens.emplace_back(
				new LexerToken(LexerTokens::CloseCurlyBrace, this->chrIndex,
							   this->chrIndex, this->lineNum, ""));
			break;
		case ',':
			this->tokens.emplace_back(
				new LexerToken(LexerTokens::Comma, this->chrIndex,
							   this->chrIndex, this->lineNum, ""));
			break;
		case '.':
			this->tokens.emplace_back(
				new LexerToken(LexerTokens::Period, this->chrIndex,
							   this->chrIndex, this->lineNum, ""));
			break;
		case '@':
			this->tokens.emplace_back(
				new LexerToken(LexerTokens::At, this->chrIndex, this->chrIndex,
							   this->lineNum, ""));
			break;
		case '#':
			while (this->getChr(false)) { // Skip to the end of the line
			}

			return false;
		case '*':
			this->lexMultiply();
			break;
		case '/':
			this->lexDivide();
			break;
		case '+':
			this->lexAdd();
			break;
		case '-':
			this->lexSubtract();
			break;
		case '%':
			this->lexModulo();
			break;
		case '>':
			this->lexGreaterThan();
			break;
		case '<':
			this->lexLessThan();
			break;
		case '&':
			this->lexAnd();
			break;
		case '|':
			this->lexOr();
			break;
		case '!':
			this->lexNot();
			break;
		default:
			if (isalpha(this->chr)) {
				this->lexKeywordOrVariable();
			} else if (isdigit(this->chr)) {
				this->lexInteger();
			} else {
				this->error("unknown character", negativeIndex);
			}

			break;
		}

		return true;
	}

  public:
	size_t chrIndex, lineNum;
	std::vector<LexerToken *> tokens;

	/**
	 * Construct a new Lexer object
	 *
	 * @param filePath - The path to the file to lex.
	 */
	Lexer(std::string filePath) {
		this->file.open(this->filePath = filePath);

		if (this->file.fail()) {
			std::cout << "failed to open file '" << filePath << "'\n";
			return;
		}

		this->closed = false;

		this->chr = '\n';
		this->prevChr = '\0';

		this->chrIndex = 0;
		this->lineNum = 0;
	}

	/**
	 * Prints an error and exits.
	 *
	 * @param ERROR_MSG     - The error message.
	 * @param startChrIndex - The start char index of erroneous
	 * token.
	 */
	[[noreturn]] void error(std::string ERROR_MSG, size_t startChrIndex) {
		std::cout << Foreground::BRIGHT_BLUE << Style::BOLD << "--> "
				  << Style::RESET << this->filePath << "\n";

		size_t lineNum = 0;
		std::string line;

		std::ifstream file;
		file.open(this->filePath);

		while (std::getline(file, line)) {
			lineNum++;

			if (lineNum == this->lineNum) {
				break;
			}
		}

		size_t length = std::to_string(this->lineNum).length() + 3;

		std::cout << this->lineNum << " | " << line << "\n";

		if (startChrIndex == static_cast<size_t>(negativeIndex) ||
			startChrIndex == this->chrIndex) {
			std::cout << std::string(length + this->chrIndex, ' ') << "^ "
					  << Foreground::BRIGHT_RED << Style::BOLD
					  << "error: " << Style::RESET << ERROR_MSG << "\n";
		} else {
			std::cout << std::string(length + startChrIndex, ' ')
					  << std::string(this->chrIndex - startChrIndex + 1, '^')
					  << " " << Foreground::BRIGHT_RED << Style::BOLD
					  << "error: " << Style::RESET << ERROR_MSG << "\n";
		}

		exit(EXIT_FAILURE);
	}

	/**
	 * Clears the tokens list.
	 */
	void clearTokens() {
		for (size_t i = 0; i < this->tokens.size(); i++) {
			delete this->tokens[0];
			this->tokens.erase(this->tokens.begin());
		}
	}

	/**
	 * Handles getting the next token and lexing it.
	 *
	 * @param mustGetToken - Whether a token must be parsed.
	 * @param nextLine     - Whether the token can be on the next line.
	 *
	 * @return bool - Whether lexing succeeded.
	 */
	bool lex(bool mustGetToken, bool nextLine) {
		if (mustGetToken) {
			while (!this->getChr(true)) {
				if (!nextLine || !this->getLine(nextLine)) {
					return false;
				}

				if (!this->getChr(true)) {
					continue;
				}

				if (this->lexNext()) {
					return true;
				}
			}
		} else {
			if (!this->getChr(true)) {
				if (!this->getLine(nextLine)) {
					return false;
				}

				if (!this->getChr(true)) {
					return false;
				}
			}
		}

		return this->lexNext();
	}
};
