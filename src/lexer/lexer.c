// Part of the Exeme Language Project, under the MIT license. See '/LICENSE' for
// license information. SPDX-License-Identifier: MIT License.

#pragma once

#include "../includes.c"

/**
 * Used to identify different lexer tokens.
 */
enum LexerTokens {
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
