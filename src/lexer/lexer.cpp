#pragma once

#include <cstddef>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <variant>
#include <vector>

enum LexerTokenIdentifier { // The ordering of the enum items is important.
	Keyword,

	// Data types
	Chr,
	String,
	Integer,
	Float,
	Array,

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

	// Operators
	Exponent,	 // '**'
	Divide,		 // '/'
	FloorDivide, // '//'
	Multiply,	 // '*'
	Add,		 // '+'
	Subtract,	 // '-'
	Modulo,		 // '%'

	// Relational operators
	Greater, // '>'
	Less,	 // '<'

	// Assignment operators
	Equals,			   // '='
	Equality,		   // '=='
	NotEquals,		   // '!='
	GreaterEquals,	   // '>='
	LessEquals,		   // '<='
	PlusEquals,		   // '+='
	SubtractEquals,	   // '-='
	MultipleEquals,	   // '*='
	DivideEquals,	   // '/='
	FlootDivideEquals, // '//='
	ExponentEquals,	   // '**='
	ModuleEquals,	   // '%='

	// Logical operators
	And, // '&&'
	Or,	 // '||'
	Not, // '!'
};

struct LexerToken {
	LexerTokenIdentifier identifier;
	std::string filePath;
	size_t startChrIndex, chrIndex, lineNum;
	std::variant<char, std::string> value;

	LexerToken(LexerTokenIdentifier identifier, std::string filePath,
			   size_t startChrIndex, size_t chrIndex, size_t lineNum,
			   std::variant<char, std::string> value) {
		this->identifier = identifier;
		this->filePath = filePath;
		this->startChrIndex = startChrIndex;
		this->chrIndex = chrIndex;
		this->lineNum = lineNum;
		this->value = value;
	}
};

class Lexer {
  private:
	char chr, prevChr;
	size_t chrIndex, lineNum;
	std::string filePath;
	std::ifstream file;

	void error(const char *ERROR_MSG, size_t startChrIndex) {
		std::cout << ERROR_MSG << std::endl;
		exit(EXIT_FAILURE);
	}

	bool getChr(bool skipWhitespace) {
		if (this->chr == '\n') {
			return false;
		}

		while (true) {
			this->prevChr = this->chr;

			if (!this->file.get(this->chr)) {
				this->file.close();
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

	bool getLine(bool forceIncrement) {
		if (!forceIncrement && this->chr != '\n') {
			return true; // Not reached the end of the line yet
		}

		this->prevChr = this->chr;
		this->chr = '\0';

		this->chrIndex = -1;
		this->lineNum++;

		while (true) {
			if (!this->getChr(
					false)) { // Automatically increments 'this->chrIndex'
				return (this->file.fail()) ? true : false;
			}

			if (isspace(this->chr)) {
				continue;
			} else {
				this->file.unget();
				this->chr = this->prevChr;
				break;
			}
		}

		return true;
	}

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

	void lexChr() {
		bool includeChr = false;
		size_t startChrIndex = this->chrIndex;
		char chr;
		size_t chrLen = 0;

		while (this->getLine(false)) {
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
		}

		this->error("unterminated char literal", startChrIndex);
	}

	void lexString() {
		bool includeChr = false;
		size_t startChrIndex = this->chrIndex;
		std::string string = "";

		while (this->getLine(false)) {
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
		}

		this->error("unterminated string literal", startChrIndex);
	}

	void lexFloat(size_t startChrIndex, std::string integer) {
		while (this->getLine(false)) {
			while (this->getChr(false)) {
				if (isspace(this->chr)) {
					break;
				}

				if (this->chr == '.') {
					this->error("too many dots for float", startChrIndex);
				} else if (!isdigit(this->chr)) {
					this->error("invalid character for integer", startChrIndex);
				}

				integer += this->chr;
			}
		}

		this->tokens.emplace_back(new LexerToken(Float, this->filePath,
												 startChrIndex, this->chrIndex,
												 this->lineNum, integer));
	}

	void lexInteger() {
		bool includeChr = false;
		size_t startChrIndex = this->chrIndex;
		std::string integer(1, this->chr);

		while (this->getLine(false)) {
			while (this->getChr(false)) {
				if (isspace(this->chr)) {
					break;
				}

				integer += this->chr;

				if (this->chr == '.') {
					return lexFloat(startChrIndex, integer);
				} else if (!isdigit(this->chr)) {
					this->error("invalid character for integer", startChrIndex);
				}
			}
		}

		this->tokens.emplace_back(new LexerToken(Integer, this->filePath,
												 startChrIndex, this->chrIndex,
												 this->lineNum, integer));
	}

	void lexEquals() {
		bool equality = true;

		if (!this->getChr(false)) {
			equality = false;
		}

		if (this->chr != '=') {
			equality = false;
			this->file.unget();
		}

		if (equality) {
			this->tokens.emplace_back(
				new LexerToken(Equality, this->filePath, this->chrIndex - 1,
							   this->chrIndex, this->lineNum, ""));
		} else {
			this->tokens.emplace_back(
				new LexerToken(Equals, this->filePath, this->chrIndex,
							   this->chrIndex, this->lineNum, ""));
		}
	}

	void lexNext() {
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
		case ',':
			this->tokens.emplace_back(
				new LexerToken(Comma, this->filePath, this->chrIndex,
							   this->chrIndex, this->lineNum, ""));
			break;
		default:
			if (isdigit(this->chr)) {
				this->lexInteger();
			} else {
				std::cout << this->chr;
			}
		}
	}

  public:
	std::vector<LexerToken *> tokens;

	Lexer(std::string filePath) {
		this->file.open(this->filePath = filePath);

		if (this->file.fail()) {
			std::cout << "failed to open file '" << filePath << "'"
					  << std::endl;
			return;
		}

		this->chr = '\n';
		this->prevChr = '\0';

		this->chrIndex = 0;
		this->lineNum = 0;
	}

	bool lex() {
		if (!this->getChr(true)) {
			if (!this->getLine(true)) {
				return false;
			}

			if (!this->getChr(true)) {
				return false;
			}
		}

		this->lexNext();

		return true;
	}
};