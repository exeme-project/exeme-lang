// Part of the Exeme Language Project, under the MIT license. See '/LICENSE' for
// license information. SPDX-License-Identifier: MIT License.

#pragma once

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <vector>

#include "../utils/console.cpp"
#include "../utils/globals.cpp"

static std::vector<std::string> KEYWORDS = {
	"break", "catch",  "class",	  "continue", "elif",
	"else",	 "except", "finally", "fn",		  "for",
	"if",	 "import", "return",  "try",	  "while"};
static auto KEYWORDS_BEGIN = KEYWORDS.begin();
static auto KEYWORDS_END = KEYWORDS.end();

/**
 * Used to identify different lexer tokens.
 */
enum class LexerTokens {
	None,

	Keyword,
	Identifier,

	Chr,
	String,
	Integer,
	Float,

	// Arithmetic operators
	Modulo,			// '%'
	Exponent,		// '**'
	Division,		// '/'
	FloorDivision,	// '//'
	Multiplication, // '*'
	Addition,		// '+'
	Subtraction,	// '-'

	// Comparison / Relational operators
	EqualTo,			// '=='
	NotEqualTo,			// '!='
	GreaterThan,		// '>'
	LessThan,			// '<'
	GreaterThanOrEqual, // '>='
	LessThanOrEqual,	// '<='

	// Logical operators
	LogicalAnd, // '&&'
	LogicalOr,	// '||'
	LogicalNot, // '!'

	// Bitwise operators
	BitwiseAND,		   // '&'
	BitwiseOR,		   // '|'
	BitwiseXOR,		   // '^'
	BitwiseNOT,		   // '~'
	BitwiseLeftShift,  // '<<'
	BitwiseRightShift, // '>>'

	// Assignment operators
	Assignment, // '='

	ModuloAssignment,		  // '%='
	ExponentAssignment,		  // '**='
	DivisionAssignment,		  // '/='
	FloorDivisionAssignment,  // '//='
	MultiplicationAssignment, // '*='
	AdditionAssignment,		  // '+='
	SubtractionAssignment,	  // '-='

	BitwiseANDAssignment,		 // '&='
	BitwiseORAssignment,		 // '|='
	BitwiseXORAssignment,		 // '^='
	BitwiseNOTAssignment,		 // '~='
	BitwiseLeftShiftAssignment,	 // '<<='
	BitwiseRightShiftAssignment, // '>>='

	// Member / Pointer operators
	Dot,   // '.'
	Arrow, // '->'
	At,	   // '@'

	// Syntactic constructs
	OpenBrace,		  // '('
	OpenSqaureBrace,  // '['
	OpenCurlyBrace,	  // '{'
	CloseBrace,		  // ')'
	CloseSquareBrace, // ']'
	CloseCurlyBrace,  // '}'

	// Other operators
	Comma,			 // ','
	Colon,			 // ':'
	ScopeResolution, // '::'

	// Misc
	Comment,
};

/**
 * Used to identify the precedence of different tokens.
 */
std::unordered_map<LexerTokens, size_t> LexerTokenPrecedences = {
	{LexerTokens::ScopeResolution, 1},

	{LexerTokens::OpenBrace, 2},
	{LexerTokens::OpenSqaureBrace, 2},
	{LexerTokens::OpenCurlyBrace, 2},
	{LexerTokens::CloseBrace, 2},
	{LexerTokens::CloseSquareBrace, 2},
	{LexerTokens::CloseCurlyBrace, 2},
	{LexerTokens::Dot, 2},
	{LexerTokens::Arrow, 2},
	{LexerTokens::Colon, 2},

	{LexerTokens::At, 3},
	{LexerTokens::Exponent, 3},

	{LexerTokens::Multiplication, 4},
	{LexerTokens::Division, 4},
	{LexerTokens::FloorDivision, 4},
	{LexerTokens::Modulo, 4},

	{LexerTokens::Addition, 5},
	{LexerTokens::Subtraction, 5},
	{LexerTokens::BitwiseAND, 5},
	{LexerTokens::BitwiseOR, 5},
	{LexerTokens::BitwiseXOR, 5},
	{LexerTokens::BitwiseNOT, 5},
	{LexerTokens::BitwiseLeftShift, 9},
	{LexerTokens::BitwiseRightShift, 9},

	{LexerTokens::GreaterThan, 6},
	{LexerTokens::LessThan, 6},
	{LexerTokens::GreaterThanOrEqual, 6},
	{LexerTokens::LessThanOrEqual, 6},

	{LexerTokens::EqualTo, 7},
	{LexerTokens::NotEqualTo, 7},

	{LexerTokens::LogicalAnd, 8},
	{LexerTokens::LogicalOr, 8},
	{LexerTokens::LogicalNot, 8},

	{LexerTokens::Assignment, 9},
	{LexerTokens::ModuloAssignment, 9},
	{LexerTokens::ExponentAssignment, 9},
	{LexerTokens::DivisionAssignment, 9},
	{LexerTokens::FloorDivisionAssignment, 9},
	{LexerTokens::MultiplicationAssignment, 9},
	{LexerTokens::AdditionAssignment, 9},
	{LexerTokens::SubtractionAssignment, 9},
	{LexerTokens::BitwiseANDAssignment, 9},
	{LexerTokens::BitwiseORAssignment, 9},
	{LexerTokens::BitwiseXORAssignment, 9},
	{LexerTokens::BitwiseNOTAssignment, 9},
	{LexerTokens::BitwiseLeftShiftAssignment, 9},
	{LexerTokens::BitwiseRightShiftAssignment, 9},

	{LexerTokens::Comma, 10},
}; // 54 - 7 (None to Float) = 47

/**
 * Contains the names of each of the lexer token identifiers.
 */
std::vector<std::string> LexerTokenNames = {
	"",
	"keyword",
	"identifier",
	"char",
	"string",
	"integer",
	"float",
	"modulo operator",
	"exponent operator",
	"division operator",
	"floor division operator",
	"multiplication operator",
	"addition operator",
	"subtraction operator",
	"equal to operator",
	"not equal to operator",
	"greater then operator",
	"less than operator",
	"greater than or equal operator",
	"less than or equal operator",
	"logical and operator",
	"logical or operator",
	"logical not operator",
	"bitwise AND operator",
	"bitwise OR operator",
	"bitwise XOR operator",
	"bitwise NOT operator",
	"bitwise left shift",
	"bitwise right shift",
	"assignment operator",
	"modulo assignment operator",
	"exponent assignment operator",
	"divison assignment operator",
	"floor division assignment operator",
	"multiplication assignment operator",
	"addition assignment operator",
	"subtraction assignment operator",
	"bitwise AND assignment operator",
	"bitwise OR assignment operator",
	"bitwise XOR assignment operator",
	"bitwise NOT assignment operator",
	"bitwise left shift assignment operator",
	"bitwise right shift assignment operator",
	"dot operator",
	"arrow operator",
	"address of operator",
	"open brace",
	"open square brace",
	"open curly brace",
	"close brace",
	"close sqaure brace",
	"close curly brace",
	"comma",
	"colon",
	"scope resolution operator",
	"comment",
};

/**
 * Represents a lexer token.
 */
struct LexerToken {
	LexerTokens identifier;
	const size_t startChrIndex, chrIndex, lineNum;
	const std::string value;

	/**
	 * Construct a new Lexer Token object.
	 *
	 * @param identifier    - Token identifier.
	 * @param startChrIndex - Start char index of token.
	 * @param chrIndex      - End char index of the token.
	 * @param lineNum       - Line num of the token.
	 * @param value         - Value of the token.
	 */
	LexerToken(LexerTokens identifier, const size_t startChrIndex,
			   const size_t chrIndex, const size_t lineNum,
			   const std::string value)
		: identifier(identifier), startChrIndex(startChrIndex),
		  chrIndex(chrIndex), lineNum(lineNum), value(value) {}
};

/**
 * The Lexer class. Handles logic for lexing a file.
 */
class Lexer {
  private:
	bool closed;
	char chr, prevChr;
	size_t unGetTokens, chrIndex, lineNum;
	std::ifstream file;
	std::string filePath;
	std::vector<const LexerToken *> tokens;

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

			if (skipWhitespace) { // Keep going till we encounter a character
								  // that is not whitespace
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
	 * @param getNextLine - Get the next line even if the EOL has not been
	 * reached.
	 *
	 * @return bool - Whether the next line was got successfully.
	 */
	bool getLine(bool getNextLine) {
		if (this->closed) {
			return false;
		}

		if (!getNextLine &&
			this->chr != '\n') { // Not reached the end of the line yet
			return true;
		}

		this->prevChr = this->chr;
		this->chr = '\0';

		this->chrIndex = static_cast<size_t>(negativeIndex);
		this->lineNum++;

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
		case 'r':
			return '\r';
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

		this->error("invalid escape sequence",
					new const LexerToken(LexerTokens::None, this->chrIndex - 1,
										 this->chrIndex, this->lineNum, ""));
	}

	/**
	 * Checks for a trailing character.
	 *
	 * @return bool - Whether the trailing character was found.
	 */
	bool checkForTrailingChr(char chr) {
		bool found = true;

		if (this->getChr(false)) {
			if (this->chr != chr) {
				found = false;
			}
		} else {
			found = false;
		}

		if (!found) {
			this->unGetChr(); // Only if a match is not found.
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
			if (!isspace(this->chr) && !isalnum(this->chr)) {
				this->error("unexpected continuation of token '" + token + "'",
							nullptr);
			}

			this->unGetChr();
		}
	}

	/**
	 * Lex a '=' token.
	 */
	void lexEquals() {
		const LexerToken *token = nullptr;

		if (this->checkForTrailingChr(this->chr)) {
			token =
				new const LexerToken(LexerTokens::EqualTo, this->chrIndex - 1,
									 this->chrIndex, this->lineNum, "==");
		} else {
			token =
				new const LexerToken(LexerTokens::Assignment, this->chrIndex,
									 this->chrIndex, this->lineNum, "=");
		}

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
				this->error("multi-character char",
							new const LexerToken(LexerTokens::None,
												 startChrIndex, this->chrIndex,
												 this->lineNum, ""));
			}

			if (this->chr == '\'') {
				this->tokens.emplace_back(
					new const LexerToken(LexerTokens::Chr, startChrIndex,
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
		this->error("unterminated char",
					new const LexerToken(LexerTokens::None, startChrIndex,
										 this->chrIndex, this->lineNum, ""));
	}

	/**
	 * Lex a string.
	 */
	void lexString() {
		bool includeChr = false;
		size_t startChrIndex = this->chrIndex, startLineNum = this->lineNum;
		std::string string = "";

		while (this->getLine(false)) {
			while (this->getChr(false)) {
				if (this->chr == '"') {
					this->tokens.emplace_back(new const LexerToken(
						LexerTokens::String, startChrIndex, this->chrIndex,
						this->lineNum, string));
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

			string += "\n";
		}

		this->error("unterminated string",
					new const LexerToken(
						LexerTokens::None,
						this->lineNum == startLineNum ? startChrIndex : 0,
						this->chrIndex, this->lineNum, ""));
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
				this->error("too many decimal points for float", nullptr);
			} else if (!isdigit(this->chr)) {
				this->error("invalid character for integer", nullptr);
			}

			integer += this->chr;
		}

		this->tokens.emplace_back(
			new const LexerToken(LexerTokens::Float, startChrIndex,
								 this->chrIndex, this->lineNum, integer));
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
				this->error("invalid character for integer", nullptr);
			}
		}

		this->tokens.emplace_back(
			new const LexerToken(LexerTokens::Integer, startChrIndex,
								 this->chrIndex, this->lineNum, integer));
	}

	/**
	 * Lex a keyword or identifier.
	 */
	void lexKeywordOrIdentifier() {
		size_t startChrIndex = this->chrIndex;
		std::string string(1, this->chr);

		while (this->getChr(false)) {
			if (!isalnum(this->chr)) {
				this->unGetChr();
				break;
			}

			string += this->chr;
		}

		this->tokens.emplace_back(new const LexerToken(
			std::find(KEYWORDS_BEGIN, KEYWORDS_END, string) == KEYWORDS_END
				? LexerTokens::Identifier
				: LexerTokens::Keyword,
			startChrIndex,
			this->chr == '\n' ? this->chrIndex - 1 : this->chrIndex,
			this->lineNum, string));
	}

	/*
	 * Lex '#='.
	 */
	void lexMultiLineComment() {
		bool end;
		size_t startChrIndex = this->chrIndex - 1, startLineNum = this->lineNum;

		while (this->getLine(false)) {
			end = false;

			while (this->getChr(true)) {
				if (end) {
					if (this->chr == '#') {
						return;
					}

					end = false;
				} else if (this->chr == '=') {
					end = true;
				}
			}
		}

		this->error("unterminated multi-line comment",
					new const LexerToken(
						LexerTokens::None,
						this->lineNum == startLineNum ? startChrIndex : 0,
						this->chrIndex, this->lineNum, ""));
	}

	/*
	 * Lex '#'.
	 */
	void lexSingleLineComment() {
		if (this->getChr(false)) {
			if (this->chr == '=') {
				this->lexMultiLineComment();
			} else {
				while (this->getChr(true)) {
				}
			}
		}

		this->tokens.emplace_back(
			new const LexerToken(LexerTokens::Comment, this->chrIndex,
								 this->chrIndex, this->lineNum, ""));
	}

	/**
	 * Lex '%'.
	 */
	void lexModulo() {
		const LexerToken *token = nullptr;

		if (this->checkForTrailingChr('=')) {
			token =
				new LexerToken(LexerTokens::ModuloAssignment, this->chrIndex,
							   this->chrIndex, this->lineNum, "%=");
		} else {
			token = new LexerToken(LexerTokens::Modulo, this->chrIndex,
								   this->chrIndex, this->lineNum, "%");
		}

		this->tokens.emplace_back(token);
		this->checkForContinuation(token->value);
	}

	/**
	 * Lex '*'.
	 */
	void lexMultiplication() {
		const LexerToken *token = nullptr;

		if (this->checkForTrailingChr(this->chr)) {
			if (this->checkForTrailingChr('=')) {
				token = new const LexerToken(LexerTokens::ExponentAssignment,
											 this->chrIndex - 2, this->chrIndex,
											 this->lineNum, "**=");
			} else {
				token = new const LexerToken(LexerTokens::Exponent,
											 this->chrIndex - 1, this->chrIndex,
											 this->lineNum, "**");
			}
		} else {
			if (this->checkForTrailingChr('=')) {
				token = new const LexerToken(
					LexerTokens::MultiplicationAssignment, this->chrIndex - 1,
					this->chrIndex, this->lineNum, "*=");
			} else {
				token = new const LexerToken(LexerTokens::Multiplication,
											 this->chrIndex, this->chrIndex,
											 this->lineNum, "*");
			}
		}

		this->tokens.emplace_back(token);
		this->checkForContinuation(token->value);
	}

	/**
	 * Lex '/'.
	 */
	void lexDivision() {
		const LexerToken *token = nullptr;

		if (this->checkForTrailingChr(this->chr)) {
			if (this->checkForTrailingChr('=')) {
				token = new const LexerToken(
					LexerTokens::FloorDivisionAssignment, this->chrIndex - 2,
					this->chrIndex, this->lineNum, "//=");
			} else {
				token = new const LexerToken(LexerTokens::FloorDivision,
											 this->chrIndex - 1, this->chrIndex,
											 this->lineNum, "//");
			}
		} else {
			if (this->checkForTrailingChr('=')) {
				token = new const LexerToken(LexerTokens::DivisionAssignment,
											 this->chrIndex - 1, this->chrIndex,
											 this->lineNum, "/=");
			} else {
				token =
					new const LexerToken(LexerTokens::Division, this->chrIndex,
										 this->chrIndex, this->lineNum, "/");
			}
		}

		this->tokens.emplace_back(token);
		this->checkForContinuation(token->value);
	}

	/**
	 * Lex '+'.
	 */
	void lexAddition() {
		const LexerToken *token = nullptr;

		if (this->checkForTrailingChr('=')) {
			token =
				new LexerToken(LexerTokens::AdditionAssignment, this->chrIndex,
							   this->chrIndex, this->lineNum, "+=");
		} else {
			token = new LexerToken(LexerTokens::Addition, this->chrIndex,
								   this->chrIndex, this->lineNum, "+");
		}

		this->tokens.emplace_back(token);
		this->checkForContinuation(token->value);
	}

	/**
	 * Lex '-'.
	 */
	void lexSubtraction() {
		const LexerToken *token = nullptr;

		if (this->checkForTrailingChr('=')) {
			token = new LexerToken(LexerTokens::SubtractionAssignment,
								   this->chrIndex, this->chrIndex,
								   this->lineNum, "-=");
		} else {
			if (this->checkForTrailingChr('>')) {
				token = new LexerToken(LexerTokens::Arrow, this->chrIndex,
									   this->chrIndex, this->lineNum, "->");
			} else {
				token = new LexerToken(LexerTokens::Subtraction, this->chrIndex,
									   this->chrIndex, this->lineNum, "-");
			}
		}

		this->tokens.emplace_back(token);
		this->checkForContinuation(token->value);
	}

	/**
	 * Lex '>'.
	 */
	void lexGreaterThan() {
		const LexerToken *token = nullptr;

		if (this->checkForTrailingChr(this->chr)) {
			if (this->checkForTrailingChr('=')) {
				token = new const LexerToken(
					LexerTokens::BitwiseRightShiftAssignment,
					this->chrIndex - 2, this->chrIndex, this->lineNum, ">>=");
			} else {
				token = new const LexerToken(LexerTokens::BitwiseRightShift,
											 this->chrIndex - 1, this->chrIndex,
											 this->lineNum, ">>");
			}
		} else {
			if (this->checkForTrailingChr('=')) {
				token = new const LexerToken(LexerTokens::GreaterThanOrEqual,
											 this->chrIndex - 1, this->chrIndex,
											 this->lineNum, ">=");
			} else {
				token = new const LexerToken(LexerTokens::GreaterThan,
											 this->chrIndex, this->chrIndex,
											 this->lineNum, ">");
			}
		}

		this->tokens.emplace_back(token);
		this->checkForContinuation(token->value);
	}

	/**
	 * Lex '<'.
	 */
	void lexLessThan() {
		const LexerToken *token = nullptr;

		if (this->checkForTrailingChr(this->chr)) {
			if (this->checkForTrailingChr('=')) {
				token = new const LexerToken(
					LexerTokens::BitwiseLeftShiftAssignment, this->chrIndex - 2,
					this->chrIndex, this->lineNum, "<<=");
			} else {
				token = new const LexerToken(LexerTokens::BitwiseLeftShift,
											 this->chrIndex - 1, this->chrIndex,
											 this->lineNum, "<<");
			}
		} else {
			if (this->checkForTrailingChr('=')) {
				token = new const LexerToken(LexerTokens::LessThanOrEqual,
											 this->chrIndex - 1, this->chrIndex,
											 this->lineNum, "<=");
			} else {
				token =
					new const LexerToken(LexerTokens::LessThan, this->chrIndex,
										 this->chrIndex, this->lineNum, "<");
			}
		}

		this->tokens.emplace_back(token);
		this->checkForContinuation(token->value);
	}

	/**
	 * Lex '!'.
	 */
	void lexLogicalNot() {
		const LexerToken *token = nullptr;

		if (this->checkForTrailingChr('=')) {
			token = new LexerToken(LexerTokens::NotEqualTo, this->chrIndex,
								   this->chrIndex, this->lineNum, "!=");
		} else {
			token = new LexerToken(LexerTokens::LogicalNot, this->chrIndex,
								   this->chrIndex, this->lineNum, "!");
		}

		this->tokens.emplace_back(token);
		this->checkForContinuation(token->value);
	}

	/**
	 * Lex '&'.
	 */
	void lexBitwiseAND() {
		const LexerToken *token = nullptr;

		if (this->checkForTrailingChr(this->chr)) {
			token = new const LexerToken(LexerTokens::LogicalAnd,
										 this->chrIndex - 1, this->chrIndex,
										 this->lineNum, "&&");
		} else {
			if (this->checkForTrailingChr('=')) {
				token = new const LexerToken(LexerTokens::BitwiseANDAssignment,
											 this->chrIndex - 1, this->chrIndex,
											 this->lineNum, "&=");
			} else {
				token = new const LexerToken(LexerTokens::BitwiseAND,
											 this->chrIndex, this->chrIndex,
											 this->lineNum, "&");
			}
		}

		this->tokens.emplace_back(token);
		this->checkForContinuation(token->value);
	}

	/**
	 * Lex '|'.
	 */
	void lexBitwiseOR() {
		const LexerToken *token = nullptr;

		if (this->checkForTrailingChr(this->chr)) {
			token =
				new const LexerToken(LexerTokens::LogicalOr, this->chrIndex - 1,
									 this->chrIndex, this->lineNum, "||");
		} else {
			if (this->checkForTrailingChr('=')) {
				token = new const LexerToken(LexerTokens::BitwiseORAssignment,
											 this->chrIndex - 1, this->chrIndex,
											 this->lineNum, "|=");
			} else {
				token =
					new const LexerToken(LexerTokens::BitwiseOR, this->chrIndex,
										 this->chrIndex, this->lineNum, "|");
			}
		}

		this->tokens.emplace_back(token);
		this->checkForContinuation(token->value);
	}

	/**
	 * Lex '^'.
	 */
	void lexBitwiseXOR() {
		const LexerToken *token = nullptr;

		if (this->checkForTrailingChr('=')) {
			token = new LexerToken(LexerTokens::BitwiseXORAssignment,
								   this->chrIndex, this->chrIndex,
								   this->lineNum, "^=");
		} else {
			token = new LexerToken(LexerTokens::BitwiseXOR, this->chrIndex,
								   this->chrIndex, this->lineNum, "^");
		}

		this->tokens.emplace_back(token);
		this->checkForContinuation(token->value);
	}

	/**
	 * Lex '~'.
	 */
	void lexBitwiseNOT() {
		const LexerToken *token = nullptr;

		if (this->checkForTrailingChr('=')) {
			token = new LexerToken(LexerTokens::BitwiseNOTAssignment,
								   this->chrIndex, this->chrIndex,
								   this->lineNum, "~=");
		} else {
			token = new LexerToken(LexerTokens::BitwiseNOT, this->chrIndex,
								   this->chrIndex, this->lineNum, "~");
		}

		this->tokens.emplace_back(token);
		this->checkForContinuation(token->value);
	}

	/**
	 * Lex ':'.
	 */
	void lexColon() {
		const LexerToken *token = nullptr;

		if (this->checkForTrailingChr(':')) {
			token = new LexerToken(LexerTokens::ScopeResolution, this->chrIndex,
								   this->chrIndex, this->lineNum, "::");
		} else {
			token = new LexerToken(LexerTokens::Colon, this->chrIndex,
								   this->chrIndex, this->lineNum, ":");
		}

		this->tokens.emplace_back(token);
		this->checkForContinuation(token->value);
	}

	/**
	 * Calls the correct function for lexing the current character.
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
				new const LexerToken(LexerTokens::OpenBrace, this->chrIndex,
									 this->chrIndex, this->lineNum, ""));
			break;
		case '[':
			this->tokens.emplace_back(new const LexerToken(
				LexerTokens::OpenSqaureBrace, this->chrIndex, this->chrIndex,
				this->lineNum, ""));
			break;
		case '{':
			this->tokens.emplace_back(new const LexerToken(
				LexerTokens::OpenCurlyBrace, this->chrIndex, this->chrIndex,
				this->lineNum, ""));
			break;
		case ')':
			this->tokens.emplace_back(
				new const LexerToken(LexerTokens::CloseBrace, this->chrIndex,
									 this->chrIndex, this->lineNum, ""));
			break;
		case ']':
			this->tokens.emplace_back(new const LexerToken(
				LexerTokens::CloseSquareBrace, this->chrIndex, this->chrIndex,
				this->lineNum, ""));
			break;
		case '}':
			this->tokens.emplace_back(new const LexerToken(
				LexerTokens::CloseCurlyBrace, this->chrIndex, this->chrIndex,
				this->lineNum, ""));
			break;
		case '.':
			this->tokens.emplace_back(
				new const LexerToken(LexerTokens::Dot, this->chrIndex,
									 this->chrIndex, this->lineNum, ""));
			break;
		case '@':
			this->tokens.emplace_back(
				new const LexerToken(LexerTokens::At, this->chrIndex,
									 this->chrIndex, this->lineNum, ""));
			break;
		case '#':
			this->lexSingleLineComment();
			break;
		case '%':
			this->lexModulo();
			break;
		case '*':
			this->lexMultiplication();
			break;
		case '/':
			this->lexDivision();
			break;
		case '+':
			this->lexAddition();
			break;
		case '-':
			this->lexSubtraction();
			break;
		case '>':
			this->lexGreaterThan();
			break;
		case '<':
			this->lexLessThan();
			break;
		case '!':
			this->lexLogicalNot();
			break;
		case '&':
			this->lexBitwiseAND();
			break;
		case '|':
			this->lexBitwiseOR();
			break;
		case '^':
			this->lexBitwiseXOR();
			break;
		case '~':
			this->lexBitwiseNOT();
			break;
		case ',':
			this->tokens.emplace_back(
				new const LexerToken(LexerTokens::Comma, this->chrIndex,
									 this->chrIndex, this->lineNum, ""));
			break;
		case ':':
			this->lexColon();
			break;
		default:
			if (isalpha(this->chr) || this->chr == '_') {
				this->lexKeywordOrIdentifier();
			} else if (isdigit(this->chr)) {
				this->lexInteger();
			} else {
				this->error("unknown character", nullptr);
			}

			break;
		}

		return true;
	}

  public:
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
		this->unGetTokens = 0;
	}

	/**
	 * Prints an error and exits.
	 *
	 * @param ERROR_MSG     - The error message.
	 * @param startChrIndex - The start char index of erroneous
	 * token.
	 */
	[[noreturn]] void error(std::string ERROR_MSG, const LexerToken *token) {
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

		if (token == nullptr) {
			std::cout << std::string(length + this->chrIndex, ' ') << "^ "
					  << Foreground::BRIGHT_RED << Style::BOLD
					  << "error: " << Style::RESET << ERROR_MSG << "\n";
		} else {
			this->lineNum = token->lineNum;
			this->chrIndex = token->chrIndex;

			std::cout << std::string(length + token->startChrIndex, ' ')
					  << std::string(this->chrIndex - token->startChrIndex + 1,
									 '^')
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
	 * Handles getting the next character and lexing it.
	 *
	 * @param mustLexChar - Whether a character must be lexed. Respects
	 * nextLine.
	 * @param nextLine    - Whether the character can be on the next line.
	 *
	 * @return bool - Whether lexing succeeded.
	 */
	bool lex(bool mustLexChar, bool nextLine) {
		if (mustLexChar) {
			while (true) {
				while (!this->getChr(true)) {
					if (!nextLine || !this->getLine(nextLine)) {
						return false;
					}
				}

				if (this->lexNext()) {
					return true;
				}
			}
		} else {
			while (!this->getChr(true)) {
				if (!this->getLine(nextLine)) {
					return false;
				}
			}
		}

		return this->lexNext();
	}

	/**
	 * Un-lexes the last token. Actually just increments an internal counter
	 * used for tracking un-lexes.
	 */
	void unLex() { this->unGetTokens++; }

	/**
	 * Gets the last token. Respects un-lexes.
	 */
	const LexerToken *getToken() {
		if (this->unGetTokens > 0) {
			return this
				->tokens[this->tokens.size() - 1 - (this->unGetTokens--)];
		}

		return this->tokens[this->tokens.size() - 1];
	}
};
