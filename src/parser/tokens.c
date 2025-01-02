/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include "../includes.c"

#include "../errors.c"
#include "../lexer/tokens.c"
#include "../utils/array.c"

/**
 * Represents an AST.
 */
struct AST {
	/**
	 * Used to identify different AST tokens.
	 */
	enum ASTTokenIdentifiers {
		ASTTOKENS_CHR,
		ASTTOKENS_STRING,
		ASTTOKENS_INTEGER,
		ASTTOKENS_FLOAT,

		ASTTOKENS_VARIABLE,
		ASTTOKENS_ASSIGNMENT,
		ASTTOKENS_MODULO_ASSIGNMENT,
		ASTTOKENS_MULTIPLICATION_ASSIGNMENT,
		ASTTOKENS_EXPONENT_ASSIGNMENT,
		ASTTOKENS_DIVISION_ASSIGNMENT,
		ASTTOKENS_FLOOR_DIVISION_ASSIGNMENT,
		ASTTOKENS_ADDITION_ASSIGNMENT,
		ASTTOKENS_SUBTRACTION_ASSIGNMENT,
		ASTTOKENS_BITWISE_AND_ASSIGNMENT,
		ASTTOKENS_BITWISE_OR_ASSIGNMENT,
		ASTTOKENS_BITWISE_XOR_ASSIGNMENT,
		ASTTOKENS_BITWISE_NOT_ASSIGNMENT,
		ASTTOKENS_BITWISE_LEFT_SHIFT_ASSIGNMENT,
		ASTTOKENS_BITWISE_RIGHT_SHIFT_ASSIGNMENT,

		ASTTOKENS_OPEN_BRACE,
		ASTTOKENS_CLOSE_BRACE,
		ASTTOKENS_COMMA,
		ASTTOKENS_COLON,

		ASTTOKENS_FUNCTION_DEFINITION,
	} IDENTIFIER;
	union {
		/* Represents a character in the AST. */
		struct AST_CHR {
			const struct LexerToken *_token;
			const struct String *VALUE;
		} *AST_CHR;

		/* Represents a string in the AST. */
		struct AST_STRING {
			const struct LexerToken *_token;
			const struct String *VALUE;
		} *AST_STRING;

		/* Represents an integer in the AST. */
		struct AST_INTEGER {
			const struct LexerToken *_token;
			const struct String *VALUE;
		} *AST_INTEGER;

		/* Represents a float in the AST. */
		struct AST_FLOAT {
			const struct LexerToken *_token;
			const struct String *VALUE;
		} *AST_FLOAT;

		/* Represents a variable in the AST.*/
		struct AST_VARIABLE {
			const bool POINTER;
			const struct LexerToken *_token;
			const struct String *NAME;
		} *AST_VARIABLE;

		/* Represents an assignment in the AST */
		struct AST_ASSIGNMENT {
			const struct LexerToken *_token;
			const struct AST_VARIABLE *IDENTIFIER;
			const struct AST *VALUE;
		} *AST_ASSIGNMENT;

		/* Represents a modulo assignment in the AST */
		struct AST_MODULO_ASSIGNMENT {
			const struct LexerToken *_token;
			const struct AST_VARIABLE *IDENTIFIER;
			const struct AST *VALUE;
		} *AST_MODULO_ASSIGNMENT;

		/* Represents a multiplication assignment in the AST */
		struct AST_MULTIPLICATION_ASSIGNMENT {
			const struct LexerToken *_token;
			const struct AST_VARIABLE *IDENTIFIER;
			const struct AST *VALUE;
		} *AST_MULTIPLICATION_ASSIGNMENT;

		/* Represents an exponent assignment in the AST */
		struct AST_EXPONENT_ASSIGNMENT {
			const struct LexerToken *_token;
			const struct AST_VARIABLE *IDENTIFIER;
			const struct AST *VALUE;
		} *AST_EXPONENT_ASSIGNMENT;

		/* Represents a division assignment in the AST */
		struct AST_DIVISION_ASSIGNMENT {
			const struct LexerToken *_token;
			const struct AST_VARIABLE *IDENTIFIER;
			const struct AST *VALUE;
		} *AST_DIVISION_ASSIGNMENT;

		/* Represents a floor division assignment in the AST */
		struct AST_FLOOR_DIVISION_ASSIGNMENT {
			const struct LexerToken *_token;
			const struct AST_VARIABLE *IDENTIFIER;
			const struct AST *VALUE;
		} *AST_FLOOR_DIVISION_ASSIGNMENT;

		/* Represents an addition assignment in the AST */
		struct AST_ADDITION_ASSIGNMENT {
			const struct LexerToken *_token;
			const struct AST_VARIABLE *IDENTIFIER;
			const struct AST *VALUE;
		} *AST_ADDITION_ASSIGNMENT;

		/* Represents a subtraction assignment in the AST */
		struct AST_SUBTRACTION_ASSIGNMENT {
			const struct LexerToken *_token;
			const struct AST_VARIABLE *IDENTIFIER;
			const struct AST *VALUE;
		} *AST_SUBTRACTION_ASSIGNMENT;

		/* Represents a bitwise	and assignment in the AST */
		struct AST_BITWISE_AND_ASSIGNMENT {
			const struct LexerToken *_token;
			const struct AST_VARIABLE *IDENTIFIER;
			const struct AST *VALUE;
		} *AST_BITWISE_AND_ASSIGNMENT;

		/* Represents a bitwise or assignment in the AST */
		struct AST_BITWISE_OR_ASSIGNMENT {
			const struct LexerToken *_token;
			const struct AST_VARIABLE *IDENTIFIER;
			const struct AST *VALUE;
		} *AST_BITWISE_OR_ASSIGNMENT;

		/* Represents a bitwise xor assignment in the AST */
		struct AST_BITWISE_XOR_ASSIGNMENT {
			const struct LexerToken *_token;
			const struct AST_VARIABLE *IDENTIFIER;
			const struct AST *VALUE;
		} *AST_BITWISE_XOR_ASSIGNMENT;

		/* Represents a bitwise not assignment in the AST */
		struct AST_BITWISE_NOT_ASSIGNMENT {
			const struct LexerToken *_token;
			const struct AST_VARIABLE *IDENTIFIER;
			const struct AST *VALUE;
		} *AST_BITWISE_NOT_ASSIGNMENT;

		/* Represents a bitwise left shift assignment in the AST */
		struct AST_BITWISE_LEFT_SHIFT_ASSIGNMENT {
			const struct LexerToken *_token;
			const struct AST_VARIABLE *IDENTIFIER;
			const struct AST *VALUE;
		} *AST_BITWISE_LEFT_SHIFT_ASSIGNMENT;

		/* Represents a bitwise right shift assignment in the AST */
		struct AST_BITWISE_RIGHT_SHIFT_ASSIGNMENT {
			const struct LexerToken *_token;
			const struct AST_VARIABLE *IDENTIFIER;
			const struct AST *VALUE;
		} *AST_BITWISE_RIGHT_SHIFT_ASSIGNMENT;

		/* Represents an open brace in the AST. */
		struct AST_OPEN_BRACE {
			const struct LexerToken *_token;
		} *AST_OPEN_BRACE;

		/* Represents a close brace in the AST. */
		struct AST_CLOSE_BRACE {
			const struct LexerToken *_token;
		} *AST_CLOSE_BRACE;

		/* Represents a comma in the AST. */
		struct AST_COMMA {
			const struct LexerToken *_token;
		} *AST_COMMA;

		/* Represents a colon in the AST. */
		struct AST_COLON {
			const struct LexerToken *_token;
		} *AST_COLON;

		/* Represents a function definition in the AST. */
		struct AST_FUNCTION_DEFINITION {
			const struct LexerToken *_token;
			const struct AST_VARIABLE *IDENTIFIER;
			const struct AST_OPEN_BRACE *OPEN_BRACE;
			const struct Array *ARGUMENTS;
			const struct Array *ARGUMENT_TYPES;
			const struct AST_CLOSE_BRACE *CLOSE_BRACE;
		} *AST_FUNCTION_DEFINITION;
	} data;
};

#define AST_STRUCT_SIZE sizeof(struct AST)
#define AST_CHR_STRUCT_SIZE sizeof(struct AST_CHR)
#define AST_STRING_STRUCT_SIZE sizeof(struct AST_STRING)
#define AST_INTEGER_STRUCT_SIZE sizeof(struct AST_INTEGER)
#define AST_FLOAT_STRUCT_SIZE sizeof(struct AST_FLOAT)
#define AST_VARIABLE_STRUCT_SIZE sizeof(struct AST_VARIABLE)
#define AST_ASSIGNMENT_STRUCT_SIZE sizeof(struct AST_ASSIGNMENT)
#define AST_MODULO_ASSIGNMENT_STRUCT_SIZE sizeof(struct AST_MODULO_ASSIGNMENT)
#define AST_MULTIPLICATION_ASSIGNMENT_STRUCT_SIZE sizeof(struct AST_MULTIPLICATION_ASSIGNMENT)
#define AST_EXPONENT_ASSIGNMENT_STRUCT_SIZE sizeof(struct AST_EXPONENT_ASSIGNMENT)
#define AST_DIVISION_ASSIGNMENT_STRUCT_SIZE sizeof(struct AST_DIVISION_ASSIGNMENT)
#define AST_FLOOR_DIVISION_ASSIGNMENT_STRUCT_SIZE sizeof(struct AST_FLOOR_DIVISION_ASSIGNMENT)
#define AST_ADDITION_ASSIGNMENT_STRUCT_SIZE sizeof(struct AST_ADDITION_ASSIGNMENT)
#define AST_SUBTRACTION_ASSIGNMENT_STRUCT_SIZE sizeof(struct AST_SUBTRACTION_ASSIGNMENT)
#define AST_BITWISE_AND_ASSIGNMENT_STRUCT_SIZE sizeof(struct AST_BITWISE_AND_ASSIGNMENT)
#define AST_BITWISE_OR_ASSIGNMENT_STRUCT_SIZE sizeof(struct AST_BITWISE_OR_ASSIGNMENT)
#define AST_BITWISE_XOR_ASSIGNMENT_STRUCT_SIZE sizeof(struct AST_BITWISE_XOR_ASSIGNMENT)
#define AST_BITWISE_NOT_ASSIGNMENT_STRUCT_SIZE sizeof(struct AST_BITWISE_NOT_ASSIGNMENT)
#define AST_BITWISE_LEFT_SHIFT_ASSIGNMENT_STRUCT_SIZE sizeof(struct AST_BITWISE_LEFT_SHIFT_ASSIGNMENT)
#define AST_BITWISE_RIGHT_SHIFT_ASSIGNMENT_STRUCT_SIZE sizeof(struct AST_BITWISE_RIGHT_SHIFT_ASSIGNMENT)
#define AST_OPEN_BRACE_STRUCT_SIZE sizeof(struct AST_OPEN_BRACE)
#define AST_CLOSE_BRACE_STRUCT_SIZE sizeof(struct AST_CLOSE_BRACE)
#define AST_COMMA_STRUCT_SIZE sizeof(struct AST_COMMA)
#define AST_COLON_STRUCT_SIZE sizeof(struct AST_COLON)
#define AST_FUNCTION_DEFINITION_STRUCT_SIZE sizeof(struct AST_FUNCTION_DEFINITION)

/**
 * Contains the names of each of the AST token identifiers.
 */
static const struct Array ASTTOKEN_NAMES = array_new_stack(
	 "AST_CHR", "AST_STRING", "AST_INTEGER", "AST_FLOAT", "AST_VARIABLE", "AST_ASSIGNMENT", "AST_MODULO_ASSIGNMENT",
	 "AST_MULTIPLICATION_ASSIGNMENT", "AST_EXPONENT_ASSIGNMENT", "AST_DIVISION_ASSIGNMENT", "AST_FLOOR_DIVISION_ASSIGNMENT",
	 "AST_ADDITION_ASSIGNMENT", "AST_SUBTRACTION_ASSIGNMENT", "AST_BITWISE_AND_ASSIGNMENT", "AST_BITWISE_OR_ASSIGNMENT",
	 "AST_BITWISE_XOR_ASSIGNMENT", "AST_BITWISE_NOT_ASSIGNMENT", "AST_BITWISE_LEFT_SHIFT_ASSIGNMENT",
	 "AST_BITWISE_RIGHT_SHIFT_ASSIGNMENT", "AST_OPEN_BRACE", "AST_CLOSE_BRACE", "AST_COMMA", "AST_COLON",
	 "AST_FUNCTION_DEFINITION", );

/**
 * Gets the name of an AST token.
 *
 * @param IDENTIFIER The AST token's identifier.
 *
 * @return The name of the AST token.
 */
const char *astTokens_getName(const enum ASTTokenIdentifiers IDENTIFIER) {
	if ((size_t)IDENTIFIER + 1 > ASTTOKEN_NAMES.length) {
		panic("ASTTOKEN_NAMES get index out of bounds");
	}

	return ASTTOKEN_NAMES._values[IDENTIFIER];
}

/* Forward declarations to silence warnings */
void ast_free(struct AST **self);

/**
 * Frees an AST_CHR struct.
 *
 * @param self The current AST_CHR struct.
 */
void astChr_free(struct AST_CHR **self) {
	if (self && *self) {
		lexerToken_free((struct LexerToken **)&(*self)->_token);
		string_free((struct String **)&(*self)->VALUE);

		free(*self);
		*self = NULL;
	} else {
		panic("AST_CHR struct has already been freed");
	}
}

/**
 * Frees an AST_STRING struct.
 *
 * @param self The current AST_STRING struct.
 */
void astString_free(struct AST_STRING **self) {
	if (self && *self) {
		lexerToken_free((struct LexerToken **)&(*self)->_token);
		string_free((struct String **)&(*self)->VALUE);

		free(*self);
		*self = NULL;
	} else {
		panic("AST_STRING struct has already been freed");
	}
}

/**
 * Frees an AST_INTEGER struct.
 *
 * @param self The current AST_INTEGER struct.
 */
void astInteger_free(struct AST_INTEGER **self) {
	if (self && *self) {
		lexerToken_free((struct LexerToken **)&(*self)->_token);

		free(*self);
		*self = NULL;
	} else {
		panic("AST_INTEGER struct has already been freed");
	}
}

/**
 * Frees an AST_FLOAT struct.
 *
 * @param self The current AST_FLOAT struct.
 */
void astFloat_free(struct AST_FLOAT **self) {
	if (self && *self) {
		lexerToken_free((struct LexerToken **)&(*self)->_token);

		free(*self);
		*self = NULL;
	} else {
		panic("AST_FLOAT struct has already been freed");
	}
}

/**
 * Frees an AST_VARIABLE struct.
 *
 * @param self The current AST_VARIABLE struct.
 */
void astVariable_free(struct AST_VARIABLE **self) {
	if (self && *self) {
		lexerToken_free((struct LexerToken **)&(*self)->_token);
		string_free((struct String **)&(*self)->NAME);

		free(*self);
		*self = NULL;
	} else {
		panic("AST_VARIABLE struct has already been freed");
	}
}

/**
 * Frees an AST_ASSIGNMENT struct.
 *
 * @param self The current AST_ASSIGNMENT struct.
 */
void astAssignment_free(struct AST_ASSIGNMENT **self) {
	if (self && *self) {
		lexerToken_free((struct LexerToken **)&(*self)->_token);
		ast_free((struct AST **)&(*self)->VALUE);
		astVariable_free((struct AST_VARIABLE **)&(*self)->IDENTIFIER);

		free(*self);
		*self = NULL;
	} else {
		panic("AST_ASSIGNMENT struct has already been freed");
	}
}

/**
 * Frees an AST_MODULO_ASSIGNMENT struct.
 *
 * @param self The current AST_MODULO_ASSIGNMENT struct.
 */
void astModuloAssignment_free(struct AST_MODULO_ASSIGNMENT **self) {
	if (self && *self) {
		lexerToken_free((struct LexerToken **)&(*self)->_token);
		ast_free((struct AST **)&(*self)->VALUE);
		astVariable_free((struct AST_VARIABLE **)&(*self)->IDENTIFIER);

		free(*self);
		*self = NULL;
	} else {
		panic("AST_MODULO_ASSIGNMENT struct has already been freed");
	}
}

/**
 * Frees an AST_MULTIPLICATION_ASSIGNMENT struct.
 *
 * @param self The current AST_MULTIPLICATION_ASSIGNMENT struct.
 */
void astMultiplicationAssignment_free(struct AST_MULTIPLICATION_ASSIGNMENT **self) {
	if (self && *self) {
		lexerToken_free((struct LexerToken **)&(*self)->_token);
		ast_free((struct AST **)&(*self)->VALUE);
		astVariable_free((struct AST_VARIABLE **)&(*self)->IDENTIFIER);

		free(*self);
		*self = NULL;
	} else {
		panic("AST_MULTIPLICATION_ASSIGNMENT struct has already been freed");
	}
}

/**
 * Frees an AST_EXPONENT_ASSIGNMENT struct.
 *
 * @param self The current AST_EXPONENT_ASSIGNMENT struct.
 */
void astExponentAssignment_free(struct AST_EXPONENT_ASSIGNMENT **self) {
	if (self && *self) {
		lexerToken_free((struct LexerToken **)&(*self)->_token);
		ast_free((struct AST **)&(*self)->VALUE);
		astVariable_free((struct AST_VARIABLE **)&(*self)->IDENTIFIER);

		free(*self);
		*self = NULL;
	} else {
		panic("AST_EXPONENT_ASSIGNMENT struct has already been freed");
	}
}

/**
 * Frees an AST_DIVISION_ASSIGNMENT struct.
 *
 * @param self The current AST_DIVISION_ASSIGNMENT struct.
 */
void astDivisionAssignment_free(struct AST_DIVISION_ASSIGNMENT **self) {
	if (self && *self) {
		lexerToken_free((struct LexerToken **)&(*self)->_token);
		ast_free((struct AST **)&(*self)->VALUE);
		astVariable_free((struct AST_VARIABLE **)&(*self)->IDENTIFIER);

		free(*self);
		*self = NULL;
	} else {
		panic("AST_DIVISION_ASSIGNMENT struct has already been freed");
	}
}

/**
 * Frees an AST_FLOOR_DIVISION_ASSIGNMENT struct.
 *
 * @param self The current AST_FLOOR_DIVISION_ASSIGNMENT struct.
 */
void astFloorDivisionAssignment_free(struct AST_FLOOR_DIVISION_ASSIGNMENT **self) {
	if (self && *self) {
		lexerToken_free((struct LexerToken **)&(*self)->_token);
		ast_free((struct AST **)&(*self)->VALUE);
		astVariable_free((struct AST_VARIABLE **)&(*self)->IDENTIFIER);

		free(*self);
		*self = NULL;
	} else {
		panic("AST_FLOOR_DIVISION_ASSIGNMENT struct has already been freed");
	}
}

/**
 * Frees an AST_ADDITION_ASSIGNMENT struct.
 *
 * @param self The current AST_ADDITION_ASSIGNMENT struct.
 */
void astAdditionAssignment_free(struct AST_ADDITION_ASSIGNMENT **self) {
	if (self && *self) {
		lexerToken_free((struct LexerToken **)&(*self)->_token);
		ast_free((struct AST **)&(*self)->VALUE);
		astVariable_free((struct AST_VARIABLE **)&(*self)->IDENTIFIER);

		free(*self);
		*self = NULL;
	} else {
		panic("AST_ADDITION_ASSIGNMENT struct has already been freed");
	}
}

/**
 * Frees an AST_SUBTRACTION_ASSIGNMENT struct.
 *
 * @param self The current AST_SUBTRACTION_ASSIGNMENT struct.
 */
void astSubtractionAssignment_free(struct AST_SUBTRACTION_ASSIGNMENT **self) {
	if (self && *self) {
		lexerToken_free((struct LexerToken **)&(*self)->_token);
		ast_free((struct AST **)&(*self)->VALUE);
		astVariable_free((struct AST_VARIABLE **)&(*self)->IDENTIFIER);

		free(*self);
		*self = NULL;
	} else {
		panic("AST_SUBTRACTION_ASSIGNMENT struct has already been freed");
	}
}

/**
 * Frees an AST_BITWISE_AND_ASSIGNMENT struct.
 *
 * @param self The current AST_BITWISE_AND_ASSIGNMENT struct.
 */
void astBitwiseAndAssignment_free(struct AST_BITWISE_AND_ASSIGNMENT **self) {
	if (self && *self) {
		lexerToken_free((struct LexerToken **)&(*self)->_token);
		ast_free((struct AST **)&(*self)->VALUE);
		astVariable_free((struct AST_VARIABLE **)&(*self)->IDENTIFIER);

		free(*self);
		*self = NULL;
	} else {
		panic("AST_BITWISE_AND_ASSIGNMENT struct has already been freed");
	}
}

/**
 * Frees an AST_BITWISE_OR_ASSIGNMENT struct.
 *
 * @param self The current AST_BITWISE_OR_ASSIGNMENT struct.
 */
void astBitwiseOrAssignment_free(struct AST_BITWISE_OR_ASSIGNMENT **self) {
	if (self && *self) {
		lexerToken_free((struct LexerToken **)&(*self)->_token);
		ast_free((struct AST **)&(*self)->VALUE);
		astVariable_free((struct AST_VARIABLE **)&(*self)->IDENTIFIER);

		free(*self);
		*self = NULL;
	} else {
		panic("AST_BITWISE_OR_ASSIGNMENT struct has already been freed");
	}
}

/**
 * Frees an AST_BITWISE_XOR_ASSIGNMENT struct.
 *
 * @param self The current AST_BITWISE_XOR_ASSIGNMENT struct.
 */
void astBitwiseXorAssignment_free(struct AST_BITWISE_XOR_ASSIGNMENT **self) {
	if (self && *self) {
		lexerToken_free((struct LexerToken **)&(*self)->_token);
		ast_free((struct AST **)&(*self)->VALUE);
		astVariable_free((struct AST_VARIABLE **)&(*self)->IDENTIFIER);

		free(*self);
		*self = NULL;
	} else {
		panic("AST_BITWISE_XOR_ASSIGNMENT struct has already been freed");
	}
}

/**
 * Frees an AST_BITWISE_NOT_ASSIGNMENT struct.
 *
 * @param self The current AST_BITWISE_NOT_ASSIGNMENT struct.
 */
void astBitwiseNotAssignment_free(struct AST_BITWISE_NOT_ASSIGNMENT **self) {
	if (self && *self) {
		lexerToken_free((struct LexerToken **)&(*self)->_token);
		ast_free((struct AST **)&(*self)->VALUE);
		astVariable_free((struct AST_VARIABLE **)&(*self)->IDENTIFIER);

		free(*self);
		*self = NULL;
	} else {
		panic("AST_BITWISE_NOT_ASSIGNMENT struct has already been freed");
	}
}

/**
 * Frees an AST_BITWISE_LEFT_SHIFT_ASSIGNMENT struct.
 *
 * @param self The current AST_BITWISE_LEFT_SHIFT_ASSIGNMENT struct.
 */
void astBitwiseLeftShiftAssignment_free(struct AST_BITWISE_LEFT_SHIFT_ASSIGNMENT **self) {
	if (self && *self) {
		lexerToken_free((struct LexerToken **)&(*self)->_token);
		ast_free((struct AST **)&(*self)->VALUE);
		astVariable_free((struct AST_VARIABLE **)&(*self)->IDENTIFIER);

		free(*self);
		*self = NULL;
	} else {
		panic("AST_BITWISE_LEFT_SHIFT_ASSIGNMENT struct has already been freed");
	}
}

/**
 * Frees an AST_BITWISE_RIGHT_SHIFT_ASSIGNMENT struct.
 *
 * @param self The current AST_BITWISE_RIGHT_SHIFT_ASSIGNMENT struct.
 */
void astBitwiseRightShiftAssignment_free(struct AST_BITWISE_RIGHT_SHIFT_ASSIGNMENT **self) {
	if (self && *self) {
		lexerToken_free((struct LexerToken **)&(*self)->_token);
		ast_free((struct AST **)&(*self)->VALUE);
		astVariable_free((struct AST_VARIABLE **)&(*self)->IDENTIFIER);

		free(*self);
		*self = NULL;
	} else {
		panic("AST_BITWISE_RIGHT_SHIFT_ASSIGNMENT struct has already been freed");
	}
}

/**
 * Frees an AST_OPEN_BRACE struct.
 *
 * @param self The current AST_OPEN_BRACE struct.
 */
void astOpenBrace_free(struct AST_OPEN_BRACE **self) {
	if (self && *self) {
		lexerToken_free((struct LexerToken **)&(*self)->_token);

		free(*self);
		*self = NULL;
	} else {
		panic("AST_OPEN_BRACE struct has already been freed");
	}
}

/**
 * Frees an AST_CLOSE_BRACE struct.
 *
 * @param self The current AST_CLOSE_BRACE struct.
 */
void astCloseBrace_free(struct AST_CLOSE_BRACE **self) {
	if (self && *self) {
		lexerToken_free((struct LexerToken **)&(*self)->_token);

		free(*self);
		*self = NULL;
	} else {
		panic("AST_CLOSE_BRACE struct has already been freed");
	}
}

/**
 * Frees an AST_COMMA struct.
 *
 * @param self The current AST_COMMA struct.
 */
void astComma_free(struct AST_COMMA **self) {
	if (self && *self) {
		lexerToken_free((struct LexerToken **)&(*self)->_token);

		free(*self);
		*self = NULL;
	} else {
		panic("AST_COMMA struct has already been freed");
	}
}

/**
 * Frees an AST_COLON struct.
 *
 * @param self The current AST_COLON struct.
 */
void astColon_free(struct AST_COLON **self) {
	if (self && *self) {
		lexerToken_free((struct LexerToken **)&(*self)->_token);

		free(*self);
		*self = NULL;
	} else {
		panic("AST_COLON struct has already been freed");
	}
}

/**
 * Frees an AST_FUNCTION_DEFINITION struct.
 *
 * @param self The current AST_FUNCTION_DEFINITION struct.
 */
void astFunctionDefinition_free(struct AST_FUNCTION_DEFINITION **self) {
	if (self && *self) {
		lexerToken_free((struct LexerToken **)&(*self)->_token);
		astVariable_free((struct AST_VARIABLE **)&(*self)->IDENTIFIER);
		astOpenBrace_free((struct AST_OPEN_BRACE **)&(*self)->OPEN_BRACE);
		array_free((struct Array **)&(*self)->ARGUMENTS);
		array_free((struct Array **)&(*self)->ARGUMENT_TYPES);
		astCloseBrace_free((struct AST_CLOSE_BRACE **)&(*self)->CLOSE_BRACE);

		free(*self);
		*self = NULL;
	} else {
		panic("AST_FUNCTION_DEFINITION struct has already been freed");
	}
}

/**
 * WARNING: DO NOT USE - USE THE MACRO INSTEAD.
 *
 * Creates a new AST struct.
 *
 * @param IDENTIFIER The identifier of the AST.
 * @param data       The data of the AST.
 *
 * @return The created AST struct.
 */
struct AST *ast_new__(enum ASTTokenIdentifiers IDENTIFIER, void *data) {
	struct AST *self = malloc(AST_STRUCT_SIZE);

	if (!self) {
		panic("failed to malloc AST struct");
	}

	self->IDENTIFIER = IDENTIFIER;

	switch (self->IDENTIFIER) {
	case ASTTOKENS_CHR:
		self->data.AST_CHR = malloc(AST_CHR_STRUCT_SIZE);
		memcpy(self->data.AST_CHR, data, AST_CHR_STRUCT_SIZE);
		break;
	case ASTTOKENS_STRING:
		self->data.AST_STRING = malloc(AST_STRING_STRUCT_SIZE);
		memcpy(self->data.AST_STRING, data, AST_STRING_STRUCT_SIZE);
		break;
	case ASTTOKENS_INTEGER:
		self->data.AST_INTEGER = malloc(AST_INTEGER_STRUCT_SIZE);
		memcpy(self->data.AST_INTEGER, data, AST_INTEGER_STRUCT_SIZE);
		break;
	case ASTTOKENS_FLOAT:
		self->data.AST_FLOAT = malloc(AST_FLOAT_STRUCT_SIZE);
		memcpy(self->data.AST_FLOAT, data, AST_FLOAT_STRUCT_SIZE);
		break;
	case ASTTOKENS_VARIABLE:
		self->data.AST_VARIABLE = malloc(AST_VARIABLE_STRUCT_SIZE);
		memcpy(self->data.AST_VARIABLE, data, AST_VARIABLE_STRUCT_SIZE);
		break;
	case ASTTOKENS_ASSIGNMENT:
		self->data.AST_ASSIGNMENT = malloc(AST_ASSIGNMENT_STRUCT_SIZE);
		memcpy(self->data.AST_ASSIGNMENT, data, AST_ASSIGNMENT_STRUCT_SIZE);
		break;
	case ASTTOKENS_MODULO_ASSIGNMENT:
		self->data.AST_MODULO_ASSIGNMENT = malloc(AST_MODULO_ASSIGNMENT_STRUCT_SIZE);
		memcpy(self->data.AST_MODULO_ASSIGNMENT, data, AST_MODULO_ASSIGNMENT_STRUCT_SIZE);
		break;
	case ASTTOKENS_MULTIPLICATION_ASSIGNMENT:
		self->data.AST_MULTIPLICATION_ASSIGNMENT = malloc(AST_MULTIPLICATION_ASSIGNMENT_STRUCT_SIZE);
		memcpy(self->data.AST_MULTIPLICATION_ASSIGNMENT, data, AST_MULTIPLICATION_ASSIGNMENT_STRUCT_SIZE);
		break;
	case ASTTOKENS_EXPONENT_ASSIGNMENT:
		self->data.AST_EXPONENT_ASSIGNMENT = malloc(AST_EXPONENT_ASSIGNMENT_STRUCT_SIZE);
		memcpy(self->data.AST_EXPONENT_ASSIGNMENT, data, AST_EXPONENT_ASSIGNMENT_STRUCT_SIZE);
		break;
	case ASTTOKENS_DIVISION_ASSIGNMENT:
		self->data.AST_DIVISION_ASSIGNMENT = malloc(AST_DIVISION_ASSIGNMENT_STRUCT_SIZE);
		memcpy(self->data.AST_DIVISION_ASSIGNMENT, data, AST_DIVISION_ASSIGNMENT_STRUCT_SIZE);
		break;
	case ASTTOKENS_FLOOR_DIVISION_ASSIGNMENT:
		self->data.AST_FLOOR_DIVISION_ASSIGNMENT = malloc(AST_FLOOR_DIVISION_ASSIGNMENT_STRUCT_SIZE);
		memcpy(self->data.AST_FLOOR_DIVISION_ASSIGNMENT, data, AST_FLOOR_DIVISION_ASSIGNMENT_STRUCT_SIZE);
		break;
	case ASTTOKENS_ADDITION_ASSIGNMENT:
		self->data.AST_ADDITION_ASSIGNMENT = malloc(AST_ADDITION_ASSIGNMENT_STRUCT_SIZE);
		memcpy(self->data.AST_ADDITION_ASSIGNMENT, data, AST_ADDITION_ASSIGNMENT_STRUCT_SIZE);
		break;
	case ASTTOKENS_SUBTRACTION_ASSIGNMENT:
		self->data.AST_SUBTRACTION_ASSIGNMENT = malloc(AST_SUBTRACTION_ASSIGNMENT_STRUCT_SIZE);
		memcpy(self->data.AST_SUBTRACTION_ASSIGNMENT, data, AST_SUBTRACTION_ASSIGNMENT_STRUCT_SIZE);
		break;
	case ASTTOKENS_BITWISE_AND_ASSIGNMENT:
		self->data.AST_BITWISE_AND_ASSIGNMENT = malloc(AST_BITWISE_AND_ASSIGNMENT_STRUCT_SIZE);
		memcpy(self->data.AST_BITWISE_AND_ASSIGNMENT, data, AST_BITWISE_AND_ASSIGNMENT_STRUCT_SIZE);
		break;
	case ASTTOKENS_BITWISE_OR_ASSIGNMENT:
		self->data.AST_BITWISE_OR_ASSIGNMENT = malloc(AST_BITWISE_OR_ASSIGNMENT_STRUCT_SIZE);
		memcpy(self->data.AST_BITWISE_OR_ASSIGNMENT, data, AST_BITWISE_OR_ASSIGNMENT_STRUCT_SIZE);
		break;
	case ASTTOKENS_BITWISE_XOR_ASSIGNMENT:
		self->data.AST_BITWISE_XOR_ASSIGNMENT = malloc(AST_BITWISE_XOR_ASSIGNMENT_STRUCT_SIZE);
		memcpy(self->data.AST_BITWISE_XOR_ASSIGNMENT, data, AST_BITWISE_XOR_ASSIGNMENT_STRUCT_SIZE);
		break;
	case ASTTOKENS_BITWISE_NOT_ASSIGNMENT:
		self->data.AST_BITWISE_NOT_ASSIGNMENT = malloc(AST_BITWISE_NOT_ASSIGNMENT_STRUCT_SIZE);
		memcpy(self->data.AST_BITWISE_NOT_ASSIGNMENT, data, AST_BITWISE_NOT_ASSIGNMENT_STRUCT_SIZE);
		break;
	case ASTTOKENS_BITWISE_LEFT_SHIFT_ASSIGNMENT:
		self->data.AST_BITWISE_LEFT_SHIFT_ASSIGNMENT = malloc(AST_BITWISE_LEFT_SHIFT_ASSIGNMENT_STRUCT_SIZE);
		memcpy(self->data.AST_BITWISE_LEFT_SHIFT_ASSIGNMENT, data, AST_BITWISE_LEFT_SHIFT_ASSIGNMENT_STRUCT_SIZE);
		break;
	case ASTTOKENS_BITWISE_RIGHT_SHIFT_ASSIGNMENT:
		self->data.AST_BITWISE_RIGHT_SHIFT_ASSIGNMENT = malloc(AST_BITWISE_RIGHT_SHIFT_ASSIGNMENT_STRUCT_SIZE);
		memcpy(self->data.AST_BITWISE_RIGHT_SHIFT_ASSIGNMENT, data, AST_BITWISE_RIGHT_SHIFT_ASSIGNMENT_STRUCT_SIZE);
		break;
	case ASTTOKENS_OPEN_BRACE:
		self->data.AST_OPEN_BRACE = malloc(AST_OPEN_BRACE_STRUCT_SIZE);
		memcpy(self->data.AST_OPEN_BRACE, data, AST_OPEN_BRACE_STRUCT_SIZE);
		break;
	case ASTTOKENS_CLOSE_BRACE:
		self->data.AST_CLOSE_BRACE = malloc(AST_CLOSE_BRACE_STRUCT_SIZE);
		memcpy(self->data.AST_CLOSE_BRACE, data, AST_CLOSE_BRACE_STRUCT_SIZE);
		break;
	case ASTTOKENS_COMMA:
		self->data.AST_COMMA = malloc(AST_COMMA_STRUCT_SIZE);
		memcpy(self->data.AST_COMMA, data, AST_COMMA_STRUCT_SIZE);
		break;
	case ASTTOKENS_COLON:
		self->data.AST_COLON = malloc(AST_COLON_STRUCT_SIZE);
		memcpy(self->data.AST_COLON, data, AST_COLON_STRUCT_SIZE);
		break;
	case ASTTOKENS_FUNCTION_DEFINITION:
		self->data.AST_FUNCTION_DEFINITION = malloc(AST_FUNCTION_DEFINITION_STRUCT_SIZE);
		memcpy(self->data.AST_FUNCTION_DEFINITION, data, AST_FUNCTION_DEFINITION_STRUCT_SIZE);
		break;
	}

	return self;
}

/* Vararg macro to reduce boilerplate */
#define ast_new(identifier, type, ...) ast_new__(identifier, &(struct type){__VA_ARGS__})

/**
 * Frees an AST struct.
 *
 * @param self The current AST struct.
 */
void ast_free(struct AST **self) {
	if (self && *self) {
		switch ((*self)->IDENTIFIER) {
		case ASTTOKENS_CHR:
			astChr_free(&(*self)->data.AST_CHR);
			break;
		case ASTTOKENS_STRING:
			astString_free(&(*self)->data.AST_STRING);
			break;
		case ASTTOKENS_INTEGER:
			astInteger_free(&(*self)->data.AST_INTEGER);
			break;
		case ASTTOKENS_FLOAT:
			astFloat_free(&(*self)->data.AST_FLOAT);
			break;
		case ASTTOKENS_VARIABLE:
			astVariable_free(&(*self)->data.AST_VARIABLE);
			break;
		case ASTTOKENS_ASSIGNMENT:
			astAssignment_free(&(*self)->data.AST_ASSIGNMENT);
			break;
		case ASTTOKENS_MODULO_ASSIGNMENT:
			astModuloAssignment_free(&(*self)->data.AST_MODULO_ASSIGNMENT);
			break;
		case ASTTOKENS_MULTIPLICATION_ASSIGNMENT:
			astMultiplicationAssignment_free(&(*self)->data.AST_MULTIPLICATION_ASSIGNMENT);
			break;
		case ASTTOKENS_EXPONENT_ASSIGNMENT:
			astExponentAssignment_free(&(*self)->data.AST_EXPONENT_ASSIGNMENT);
			break;
		case ASTTOKENS_DIVISION_ASSIGNMENT:
			astDivisionAssignment_free(&(*self)->data.AST_DIVISION_ASSIGNMENT);
			break;
		case ASTTOKENS_FLOOR_DIVISION_ASSIGNMENT:
			astFloorDivisionAssignment_free(&(*self)->data.AST_FLOOR_DIVISION_ASSIGNMENT);
			break;
		case ASTTOKENS_ADDITION_ASSIGNMENT:
			astAdditionAssignment_free(&(*self)->data.AST_ADDITION_ASSIGNMENT);
			break;
		case ASTTOKENS_SUBTRACTION_ASSIGNMENT:
			astSubtractionAssignment_free(&(*self)->data.AST_SUBTRACTION_ASSIGNMENT);
			break;
		case ASTTOKENS_BITWISE_AND_ASSIGNMENT:
			astBitwiseAndAssignment_free(&(*self)->data.AST_BITWISE_AND_ASSIGNMENT);
			break;
		case ASTTOKENS_BITWISE_OR_ASSIGNMENT:
			astBitwiseOrAssignment_free(&(*self)->data.AST_BITWISE_OR_ASSIGNMENT);
			break;
		case ASTTOKENS_BITWISE_XOR_ASSIGNMENT:
			astBitwiseXorAssignment_free(&(*self)->data.AST_BITWISE_XOR_ASSIGNMENT);
			break;
		case ASTTOKENS_BITWISE_NOT_ASSIGNMENT:
			astBitwiseNotAssignment_free(&(*self)->data.AST_BITWISE_NOT_ASSIGNMENT);
			break;
		case ASTTOKENS_BITWISE_LEFT_SHIFT_ASSIGNMENT:
			astBitwiseLeftShiftAssignment_free(&(*self)->data.AST_BITWISE_LEFT_SHIFT_ASSIGNMENT);
			break;
		case ASTTOKENS_BITWISE_RIGHT_SHIFT_ASSIGNMENT:
			astBitwiseRightShiftAssignment_free(&(*self)->data.AST_BITWISE_RIGHT_SHIFT_ASSIGNMENT);
			break;
		case ASTTOKENS_OPEN_BRACE:
			astOpenBrace_free(&(*self)->data.AST_OPEN_BRACE);
			break;
		case ASTTOKENS_CLOSE_BRACE:
			astCloseBrace_free(&(*self)->data.AST_CLOSE_BRACE);
			break;
		case ASTTOKENS_COMMA:
			astComma_free(&(*self)->data.AST_COMMA);
			break;
		case ASTTOKENS_COLON:
			astColon_free(&(*self)->data.AST_COLON);
			break;
		case ASTTOKENS_FUNCTION_DEFINITION:
			astFunctionDefinition_free(&(*self)->data.AST_FUNCTION_DEFINITION);
			break;
		}

		free(*self);
		*self = NULL;
	} else {
		panic("AST struct has already been freed");
	}
}
