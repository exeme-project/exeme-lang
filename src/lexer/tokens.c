/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include "../includes.c"

#include "../utils/array.c"
#include "../utils/string.c"

/**
 * Used to identify different lexer tokens.
 */
enum LexerTokenIdentifiers {
    LEXERTOKENS_NONE,

    LEXERTOKENS_KEYWORD,
    LEXERTOKENS_IDENTIFIER,

    LEXERTOKENS_CHR,
    LEXERTOKENS_STRING,
    LEXERTOKENS_INTEGER,
    LEXERTOKENS_FLOAT,

    // Arithmetic operators
    LEXERTOKENS_MODULO,         // '%'
    LEXERTOKENS_MULTIPLICATION, // '*'
    LEXERTOKENS_EXPONENT,       // '**'
    LEXERTOKENS_DIVISION,       // '/'
    LEXERTOKENS_FLOOR_DIVISION, // '//'
    LEXERTOKENS_ADDITION,       // '+'
    LEXERTOKENS_SUBTRACTION,    // '-'

    // Comparison / Relational operators
    LEXERTOKENS_EQUAL_TO,              // '=='
    LEXERTOKENS_NOT_EQUAL_TO,          // '!='
    LEXERTOKENS_GREATER_THAN,          // '>'
    LEXERTOKENS_LESS_THAN,             // '<'
    LEXERTOKENS_GREATER_THAN_OR_EQUAL, // '>='
    LEXERTOKENS_LESS_THAN_OR_EQUAL,    // '<='

    // Logical operators
    LEXERTOKENS_LOGICAL_AND, // '&&'
    LEXERTOKENS_LOGICAL_OR,  // '||'
    LEXERTOKENS_LOGICAL_NOT, // '!'

    // Bitwise operators
    LEXERTOKENS_BITWISE_AND,         // '&'
    LEXERTOKENS_BITWISE_OR,          // '|'
    LEXERTOKENS_BITWISE_XOR,         // '^'
    LEXERTOKENS_BITWISE_NOT,         // '~'
    LEXERTOKENS_BITWISE_LEFT_SHIFT,  // '<<'
    LEXERTOKENS_BITWISE_RIGHT_SHIFT, // '>>'

    // Assignment operators
    LEXERTOKENS_ASSIGNMENT, // '='

    LEXERTOKENS_MODULO_ASSIGNMENT,         // '%='
    LEXERTOKENS_MULTIPLICATION_ASSIGNMENT, // '*='
    LEXERTOKENS_EXPONENT_ASSIGNMENT,       // '**='
    LEXERTOKENS_DIVISION_ASSIGNMENT,       // '/='
    LEXERTOKENS_FLOOR_DIVISION_ASSIGNMENT, // '//='
    LEXERTOKENS_ADDITION_ASSIGNMENT,       // '+='
    LEXERTOKENS_SUBTRACTION_ASSIGNMENT,    // '-='

    LEXERTOKENS_BITWISE_AND_ASSIGNMENT,         // '&='
    LEXERTOKENS_BITWISE_OR_ASSIGNMENT,          // '|='
    LEXERTOKENS_BITWISE_XOR_ASSIGNMENT,         // '^='
    LEXERTOKENS_BITWISE_NOT_ASSIGNMENT,         // '~='
    LEXERTOKENS_BITWISE_LEFT_SHIFT_ASSIGNMENT,  // '<<='
    LEXERTOKENS_BITWISE_RIGHT_SHIFT_ASSIGNMENT, // '>>='

    // Member / Pointer operators
    LEXERTOKENS_DOT,   // '.'
    LEXERTOKENS_ARROW, // '->'
    LEXERTOKENS_AT,    // '@'

    // Syntactic constructs
    LEXERTOKENS_OPEN_BRACE,         // '('
    LEXERTOKENS_OPEN_SQUARE_BRACE,  // '['
    LEXERTOKENS_OPEN_CURLY_BRACE,   // '{'
    LEXERTOKENS_CLOSE_BRACE,        // ')'
    LEXERTOKENS_CLOSE_SQUARE_BRACE, // ']'
    LEXERTOKENS_CLOSE_CURLY_BRACE,  // '}'
    LEXERTOKENS_COMMA,              // ','
    LEXERTOKENS_COLON,              // ':'
    LEXERTOKENS_SCOPE_RESOLUTION,   // '::'

    // Misc
    LEXERTOKENS_SINGLE_LINE_COMMENT,
    LEXERTOKENS_MULTI_LINE_COMMENT
};

/**
 * Contains the names of each of the lexer token identifiers.
 */
static const struct Array LEXERTOKEN_NAMES = array_new_stack(
    "",

    "keyword", "identifier",

    "char", "string", "integer", "float",

    // Arithmetic operators
    "modulo operator", "multiplication operator", "exponent operator", "division operator", "floor division operator",
    "addition operator", "subtraction operator",

    // Comparison / Relational operators
    "equal to operator", "not equal to operator", "greater then operator", "less than operator",
    "greater than or equal operator", "less than or equal operator",

    // Logical operators
    "logical and operator", "logical or operator", "logical not operator",

    // Bitwise operators
    "bitwise AND operator", "bitwise OR operator", "bitwise XOR operator", "bitwise NOT operator", "bitwise left shift",
    "bitwise right shift",

    // Assignment operators
    "assignment operator",

    "modulo assignment operator", "multiplication assignment operator", "exponent assignment operator",
    "division assignment operator", "floor division assignment operator", "addition assignment operator",
    "subtraction assignment operator",

    "bitwise AND assignment operator", "bitwise OR assignment operator", "bitwise XOR assignment operator",
    "bitwise NOT assignment operator", "bitwise left shift assignment operator", "bitwise right shift assignment operator",

    // Member / Pointer operators
    "dot operator", "arrow operator", "at operator",

    // Syntactic constructs
    "open brace", "open square brace", "open curly brace", "close brace", "close square brace", "close curly brace", "comma",
    "colon", "scope resolution operator",

    // Misc
    "single line comment", "multi line comment");

/**
 * Gets the name of a lexer token.
 *
 * @param IDENTIFIER The lexer token's identifier.
 *
 * @return The name of the lexer token.
 */
const char *lexerTokens_getName(const enum LexerTokenIdentifiers IDENTIFIER) {
    if ((size_t)IDENTIFIER + 1 > LEXERTOKEN_NAMES.length) {
        panic("LEXERTOKEN_NAMES get index out of bounds");
    }

    return LEXERTOKEN_NAMES._values[IDENTIFIER];
}

/**
 * Used to identify the precedence of different tokens. Comparison can
 * be done with 'strcmp(a, b) < 0' ('true' if 'a' precedes over 'b',
 * else 'false').
 */
const struct Array LEXER_TOKEN_PRECEDENCES = array_new_stack("a",

                                                             "a", "a",

                                                             "a", "a", "a", "a",

                                                             // Arithmetic operators
                                                             "d", "c", "d", "d", "d", "e", "e",

                                                             // Comparison / Relational operators
                                                             "g", "g", "f", "f", "f", "f",

                                                             // Logical operators
                                                             "h", "h", "h",

                                                             // Bitwise operators
                                                             "e", "e", "e", "e", "i", "i",

                                                             // Assignment operators
                                                             "i",

                                                             "i", "i", "i", "i", "i", "i", "i",

                                                             "i", "i", "i", "i", "i", "i",

                                                             // Member / Pointer operators
                                                             "b", "b", "c",

                                                             // Syntactic constructs
                                                             "b", "b", "b", "b", "b", "b", "j", "b", "a",

                                                             // Misc
                                                             "a", "a");

/**
 * Represents a lexer token.
 */
struct LexerToken {
    enum LexerTokenIdentifiers identifier;
    size_t startChrIndex, endChrIndex, lineIndex;
    const struct String *value;
};

#define LEXERTOKEN_STRUCT_SIZE sizeof(struct LexerToken)

/**
 * Creates a new LexerToken struct.
 *
 * @param identifier       Token identifier.
 * @param value            Value of the token.
 * @param startChrIndex    Start char index of the token.
 * @param endChrIndex      End char index of the token.
 * @param lineIndex        Line index of the token.
 *
 * @return The created LexerToken struct.
 */
const struct LexerToken *lexerToken_new(enum LexerTokenIdentifiers identifier, struct String *value, size_t startChrIndex,
                                        size_t endChrIndex, size_t lineIndex) {
    struct LexerToken *self = malloc(LEXERTOKEN_STRUCT_SIZE);

    if (!self) {
        panic("failed to malloc LexerToken struct");
    }

    self->identifier = identifier;
    self->value = value;
    self->startChrIndex = startChrIndex;
    self->endChrIndex = endChrIndex;
    self->lineIndex = lineIndex;

    return self;
}

/**
 * Frees an LexerToken struct.
 *
 * @param self The current LexerToken struct.
 */
void lexerToken_free(struct LexerToken **self) {
    if (self && *self) {
        string_free((struct String **)&(*self)->value);

        free(*self);
        *self = NULL;
    } else {
        panic("LexerToken struct has already been freed");
    }
}
