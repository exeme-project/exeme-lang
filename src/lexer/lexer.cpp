// Part of the Exeme language project, under the MIT license. See '/LICENSE' for
// license information. SPDX-License-Identifier: MIT License.

#pragma once

#include "../utils/console.cpp"
#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

std::vector<std::string> KEYWORDS = {
	"break", "catch", "class", "continue", "elif",	 "else", "finally",
	"fn",	 "for",	  "if",	   "import",   "return", "try",	 "while"};
auto KEYWORDS_BEGIN = KEYWORDS.begin();
auto KEYWORDS_END = KEYWORDS.end();

/**
 * @brief Used to identify different tokens.
 *
 */
enum LexerTokenIdentifier {
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

	// Operators (order is important)
	Exponent,	 // '**'
	Divide,		 // '/'
	FloorDivide, // '//'
	Multiply,	 // '*'
	Add,		 // '+'
	Subtract,	 // '-'
	Modulo,		 // '%'

	// Relational operators
	GreaterThan, // '>'
	LessThan,	 // '<'

	// Assignment operators
	Equals,			   // '='
	Equality,		   // '=='
	NotEquals,		   // '!='
	GreaterThanEquals, // '>='
	LessThanEquals,	   // '<='
	AddEquals,		   // '+='
	SubtractEquals,	   // '-='
	MultiplyEquals,	   // '*='
	DivideEquals,	   // '/='
	FloorDivideEquals, // '//='
	ExponentEquals,	   // '**='
	ModuleEquals,	   // '%='

	// Logical operators
	And, // '&&'
	Or,	 // '||'
	Not, // '!'
};

/**
 * @brief The LexerToken struct. Contains data relating to a lexer token.
 *
 */
struct LexerToken {
	LexerTokenIdentifier identifier;
	std::string filePath;
	size_t startChrIndex, chrIndex, lineNum;
	std::string value;

	/**
	 * @brief Construct a new Lexer Token object.
	 *
	 * @param identifier Token identifier.
	 * @param filePath File path token originates from.
	 * @param startChrIndex Start char index of token.
	 * @param chrIndex End char index of the token.
	 * @param lineNum Line num of the token.
	 * @param value Value of the token.
	 */
	LexerToken(LexerTokenIdentifier identifier, std::string filePath,
			   size_t startChrIndex, size_t chrIndex, size_t lineNum,
			   std::string value) {
		this->identifier = identifier;
		this->filePath = filePath;
		this->startChrIndex = startChrIndex;
		this->chrIndex = chrIndex;
		this->lineNum = lineNum;
		this->value = value;
	}
};

/**
 * @brief The Lexer class. Handles logic for lexing a file.
 *
 */
class Lexer {
  private:
	bool closed;
	char chr, prevChr;
	size_t chrIndex, lineNum;
	std::string filePath;
	std::ifstream file;

	/**
	 * @brief Get the next character.
	 * @see unGetChr()

	 * @param skipWhitespace Whether or not to skip whitespace characters.
	 * @return true Succeeded to get the next character.
	 * @return false Failed to get the next character.
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
	 * @brief Un-get the current character.
	 * @see getChr()

	 * @return true Succeeded to un-get the current character.
	 * @return false Failed to un-get the current character.
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
	 * @brief Get the next line.
	 *
	 * @param getNext Get the next line even if the EOL has not been reached.
	 * @return true Succeeded to get the next line.
	 * @return false Failed to get the next line.
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

		this->chrIndex = -1;
		this->lineNum++;

		if (!this->getChr(
				true)) { // Read chars till non-whitespace is encountered
			return !this->file.fail();
		}

		this->unGetChr();

		return true;
	}

	/**
	 * @brief Returns the escaped version of the current character.
	 *
	 * @return char
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
		return '\0';
	}

	/**
	 * @brief Checks for repetition of the current character.
	 *
	 * @param ifNotRepeated Identifier of token if repetition was not found.
	 * @param ifRepeated Identifier of token if repetition was found.
	 * @return LexerToken* The created token.
	 */
	LexerToken *checkForRepetition(LexerTokenIdentifier ifNotRepeated,
								   LexerTokenIdentifier ifRepeated) {
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
			return new LexerToken(ifRepeated, this->filePath,
								  this->chrIndex - 1, this->chrIndex,
								  this->lineNum, std::string(2, this->chr));
		} else {
			return new LexerToken(ifNotRepeated, this->filePath, this->chrIndex,
								  this->chrIndex, this->lineNum,
								  std::string(1, this->chr));
		}
	}

	/**
	 * @brief Checks for a trailing equals.
	 *
	 * @param token The current token.
	 * @return true Trailing equals was found.
	 * @return false Trailing equals wasn't found.
	 */
	bool checkForEquals(LexerToken *token) {
		bool equals = true;

		if (this->getChr(false)) {
			if (this->chr != '=') {
				this->unGetChr();
				equals = false;
			}
		} else {
			equals = false;
		}

		if (equals) {
			token->chrIndex++;
			token->value += "=";
		}

		return equals;
	}

	/**
	 * @brief Checks for an unexpected continuation of the current token.
	 *
	 * @param token
	 */
	void checkForContinuation(std::string token) {
		if (this->getChr(false)) {
			if (!isspace(this->chr)) {
				this->error("unexpected continuation of token '" + token + "'",
							-1);
			}

			this->unGetChr();
		}
	}

	/**
	 * @brief Lex a '=' token.
	 *
	 */
	void lexEquals() {
		LexerToken *token = this->checkForRepetition(Equals, Equality);

		this->tokens.emplace_back(token);

		this->checkForContinuation(token->value);
	}

	/**
	 * @brief Lex a character.
	 *
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
					new LexerToken(Chr, this->filePath, startChrIndex,
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
	 * @brief Lex a string.
	 *
	 */
	void lexString() {
		bool includeChr = false;
		size_t startChrIndex = this->chrIndex;
		std::string string = "";

		while (this->getChr(false)) {
			if (this->chr == '"') {
				this->tokens.emplace_back(
					new LexerToken(String, this->filePath, startChrIndex,
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
	 * @brief Lex a float.
	 * @see lexInteger()
	 *
	 * @param startChrIndex The index of the first character of the float.
	 * @param integer The float's value (float is a C++ keyword).
	 */
	void lexFloat(size_t startChrIndex, std::string integer) {
		while (this->getChr(false)) {
			if (isspace(this->chr) ||
				(this->chr != '.' && !isalpha(this->chr))) {
				this->unGetChr();
				break;
			}

			if (this->chr == '.') {
				this->error("too many decimal points for float", -1);
			} else if (!isdigit(this->chr)) {
				this->error("invalid character for integer", -1);
			}

			integer += this->chr;
		}

		this->tokens.emplace_back(new LexerToken(Float, this->filePath,
												 startChrIndex, this->chrIndex,
												 this->lineNum, integer));
	}

	/**
	 * @brief Lex an integer. Hands control over to 'lexFloat()' if needed.
	 * @see lexFloat()
	 *
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
				this->error("invalid character for integer", -1);
			}
		}

		this->tokens.emplace_back(new LexerToken(Integer, this->filePath,
												 startChrIndex, this->chrIndex,
												 this->lineNum, integer));
	}

	/**
	 * @brief Lex a keyword or variable.
	 *
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
			(std::find(KEYWORDS_BEGIN, KEYWORDS_END, string) == KEYWORDS_END)
				? Variable
				: Keyword,
			this->filePath, startChrIndex,
			(this->chr == '\n') ? this->chrIndex - 1 : this->chrIndex,
			this->lineNum, string));
	}

	/**
	 * @brief Lex '*'.
	 *
	 */
	void lexMultiply() {
		LexerToken *token = this->checkForRepetition(Multiply, Exponent);

		if (this->checkForEquals(token)) {
			token->identifier = (token->identifier == Multiply)
									? MultiplyEquals
									: ExponentEquals;
		}

		this->tokens.emplace_back(token);
		this->checkForContinuation(token->value);
	}

	/**
	 * @brief Lex '/'.
	 *
	 */
	void lexDivide() {
		LexerToken *token = this->checkForRepetition(Divide, FloorDivide);

		if (this->checkForEquals(token)) {
			token->identifier = (token->identifier == Divide)
									? DivideEquals
									: FloorDivideEquals;
		}

		this->tokens.emplace_back(token);
		this->checkForContinuation(token->value);
	}

	/**
	 * @brief Lex '+'.
	 *
	 */
	void lexAdd() {
		LexerToken *token =
			new LexerToken(Add, this->filePath, this->chrIndex, this->chrIndex,
						   this->lineNum, std::string(1, this->chr));

		if (this->checkForEquals(token)) {
			token->identifier = AddEquals;
		}

		this->tokens.emplace_back(token);
		this->checkForContinuation(token->value);
	}

	/**
	 * @brief Lex '-'.
	 *
	 */
	void lexSubtract() {
		LexerToken *token = new LexerToken(
			Subtract, this->filePath, this->chrIndex, this->chrIndex,
			this->lineNum, std::string(1, this->chr));

		if (this->checkForEquals(token)) {
			token->identifier = SubtractEquals;
		}

		this->tokens.emplace_back(token);
		this->checkForContinuation(token->value);
	}

	/**
	 * @brief Lex '%'.
	 *
	 */
	void lexModulo() {
		LexerToken *token = new LexerToken(
			Modulo, this->filePath, this->chrIndex, this->chrIndex,
			this->lineNum, std::string(1, this->chr));

		if (this->checkForEquals(token)) {
			token->identifier = ModuleEquals;
		}

		this->tokens.emplace_back(token);
		this->checkForContinuation(token->value);
	}

	/**
	 * @brief Lex '>='.
	 *
	 */
	void lexGreaterThan() {
		LexerToken *token = new LexerToken(
			GreaterThan, this->filePath, this->chrIndex, this->chrIndex,
			this->lineNum, std::string(1, this->chr));

		if (this->checkForEquals(token)) {
			token->identifier = GreaterThanEquals;
		}

		this->tokens.emplace_back(token);
		this->checkForContinuation(token->value);
	}

	/**
	 * @brief Lex '<='.
	 *
	 */
	void lexLessThan() {
		LexerToken *token = new LexerToken(
			LessThan, this->filePath, this->chrIndex, this->chrIndex,
			this->lineNum, std::string(1, this->chr));

		if (this->checkForEquals(token)) {
			token->identifier = LessThanEquals;
		}

		this->tokens.emplace_back(token);
		this->checkForContinuation(token->value);
	}

	/**
	 * @brief Lex '&&'.
	 *
	 */
	void lexAnd() {
		LexerToken *token = new LexerToken(And, this->filePath, this->chrIndex,
										   this->chrIndex + 1, this->lineNum,
										   std::string(2, this->chr));

		if (this->getChr(false)) {
			if (this->chr != '&') {
				this->error(
					"'&" + std::string(1, this->chr) +
						"' is not a valid token prefix - did you mean '&&'",
					this->chrIndex - 1);
			}
		} else {
			this->error("'&' is not a valid token - did you mean '&&'", -1);
		}

		this->tokens.emplace_back(token);
	}

	/**
	 * @brief Lex '||'.
	 *
	 */
	void lexOr() {
		LexerToken *token = new LexerToken(Or, this->filePath, this->chrIndex,
										   this->chrIndex + 1, this->lineNum,
										   std::string(2, this->chr));

		if (this->getChr(false)) {
			if (this->chr != '|') {
				this->error(
					"'|" + std::string(1, this->chr) +
						"' is not a valid token prefix - did you mean '||'",
					this->chrIndex - 1);
			}
		} else {
			this->error("'|' is not a valid token - did you mean '||'", -1);
		}

		this->tokens.emplace_back(token);
	}

	/**
	 * @brief Lex '!'.
	 *
	 */
	void lexNot() {
		LexerToken *token =
			new LexerToken(Not, this->filePath, this->chrIndex, this->chrIndex,
						   this->lineNum, std::string(1, this->chr));

		if (this->checkForEquals(token)) {
			token->identifier = NotEquals;
		}

		this->tokens.emplace_back(token);
		this->checkForContinuation(token->value);
	}

	/**
	 * @brief Calls the correct function for lexing the current token.
	 *
	 * @return true Lexing succeeded.
	 * @return false Lexing failed.
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
				new LexerToken(OpenBrace, this->filePath, this->chrIndex,
							   this->chrIndex, this->lineNum, ""));
			break;
		case '[':
			this->tokens.emplace_back(
				new LexerToken(OpenSqaureBrace, this->filePath, this->chrIndex,
							   this->chrIndex, this->lineNum, ""));
			break;
		case '{':
			this->tokens.emplace_back(
				new LexerToken(OpenCurlyBrace, this->filePath, this->chrIndex,
							   this->chrIndex, this->lineNum, ""));
			break;
		case ')':
			this->tokens.emplace_back(
				new LexerToken(CloseBrace, this->filePath, this->chrIndex,
							   this->chrIndex, this->lineNum, ""));
			break;
		case ']':
			this->tokens.emplace_back(
				new LexerToken(CloseSquareBrace, this->filePath, this->chrIndex,
							   this->chrIndex, this->lineNum, ""));
			break;
		case '}':
			this->tokens.emplace_back(
				new LexerToken(CloseCurlyBrace, this->filePath, this->chrIndex,
							   this->chrIndex, this->lineNum, ""));
			break;
		case ',':
			this->tokens.emplace_back(
				new LexerToken(Comma, this->filePath, this->chrIndex,
							   this->chrIndex, this->lineNum, ""));
			break;
		case '.':
			this->tokens.emplace_back(
				new LexerToken(Period, this->filePath, this->chrIndex,
							   this->chrIndex, this->lineNum, ""));
			break;
		case '@':
			this->tokens.emplace_back(
				new LexerToken(At, this->filePath, this->chrIndex,
							   this->chrIndex, this->lineNum, ""));
			break;
		case '#':
			while (this->getChr(false)) { // Skip to the end of the line
				;
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
				this->error("unknown character", -1);
			}

			break;
		}

		return true;
	}

  public:
	std::vector<LexerToken *> tokens;

	/**
	 * @brief Construct a new Lexer object
	 *
	 * @param filePath The path to the file to lex.
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
	 * @brief Prints an error and exits.
	 *
	 * @param ERROR_MSG The error message.
	 * @param startChrIndex The start char index of erroneous token.
	 */
	void error(std::string ERROR_MSG, size_t startChrIndex) {
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

		if (startChrIndex == (size_t)-1) {
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
	 * @brief Handles getting the next token and lexing it.
	 * @see lexNext()
	 *
	 * @param mustGetToken A token must be parsed.
	 * @param nextLine Whether the token can be on the next line.
	 * @return true Lexing succeeded.
	 * @return false Lexing failed.
	 */
	bool lex(bool mustGetToken, bool nextLine) {
		if (mustGetToken) {
			while (!this->getChr(true)) {
				if (!this->getLine(nextLine)) {
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