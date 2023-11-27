/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include "../includes.c"

#include "../parser/parser.c"
#include "./tokens.c"

struct Compiler {
    struct Parser *parser;
};

#define COMPILER_STRUCT_SIZE sizeof(struct Compiler)

/**
 * Creates a new Compiler struct.
 *
 * @param FILE_PATH The path to the file to compile.
 *
 * @return The created Compiler struct.
 */
struct Compiler *compiler_new(const char *FILE_PATH) {
    struct Compiler *compiler = malloc(COMPILER_STRUCT_SIZE);

    compiler->parser = parser_new(FILE_PATH);

    return compiler;
}

/**
 * Frees the Compiler struct.
 *
 * @param self The current Compiler struct.
 */
void compiler_free(struct Compiler **self) {
    if (self && *self) {
        parser_free(&(*self)->parser);

        free(*self);
        *self = NULL;
    } else {
        panic("Compiler struct has already been freed");
    }
}

/**
 * Compiles the current assignment.
 *
 * @param self The current Compiler struct.
 */
void compiler_compileAssignment(struct Compiler *self) { printf("compiling assignment\n"); }

/**
 * Compiles the current modulo assignment.
 *
 * @param self The current Compiler struct.
 */
void compiler_compileModuloAssignment(struct Compiler *self) { printf("compiling modulo assignment\n"); }

/**
 * Compiles the current multiplication assignment.
 *
 * @param self The current Compiler struct.
 */
void compiler_compileMultiplicationAssignment(struct Compiler *self) { printf("compiling multiplication assignment\n"); }

/**
 * Compiles the current exponent assignment.
 *
 * @param self The current Compiler struct.
 */
void compiler_compileExponentAssignment(struct Compiler *self) { printf("compiling exponent assignment\n"); }

/**
 * Compiles the current division assignment.
 *
 * @param self The current Compiler struct.
 */
void compiler_compileDivisionAssignment(struct Compiler *self) { printf("compiling division assignment\n"); }

/**
 * Compiles the current floor division assignment.
 *
 * @param self The current Compiler struct.
 */
void compiler_compileFloorDivisionAssignment(struct Compiler *self) { printf("compiling floor division assignment\n"); }

/**
 * Compiles the current addition assignment.
 *
 * @param self The current Compiler struct.
 */
void compiler_compileAdditionAssignment(struct Compiler *self) { printf("compiling addition assignment\n"); }

/**
 * Compiles the current subtraction assignment.
 *
 * @param self The current Compiler struct.
 */
void compiler_compileSubtractionAssignment(struct Compiler *self) { printf("compiling subtraction assignment\n"); }

/**
 * Compiles the current bitwise and assignment.
 *
 * @param self The current Compiler struct.
 */
void compiler_compileBitwiseAndAssignment(struct Compiler *self) { printf("compiling bitwise and assignment\n"); }

/**
 * Compiles the current bitwise or assignment.
 *
 * @param self The current Compiler struct.
 */
void compiler_compileBitwiseOrAssignment(struct Compiler *self) { printf("compiling bitwise or assignment\n"); }

/**
 * Compiles the current bitwise xor assignment.
 *
 * @param self The current Compiler struct.
 */
void compiler_compileBitwiseXorAssignment(struct Compiler *self) { printf("compiling bitwise xor assignment\n"); }

/**
 * Compiles the current bitwise not assignment.
 *
 * @param self The current Compiler struct.
 */
void compiler_compileBitwiseNotAssignment(struct Compiler *self) { printf("compiling bitwise not assignment\n"); }

/**
 * Compiles the current bitwise left shift assignment.
 *
 * @param self The current Compiler struct.
 */
void compiler_compileBitwiseLeftShiftAssignment(struct Compiler *self) {
    printf("compiling bitwise left shift assignment\n");
}

/**
 * Compiles the current bitwise right shift assignment.
 *
 * @param self The current Compiler struct.
 */
void compiler_compileBitwiseRightShiftAssignment(struct Compiler *self) {
    printf("compiling bitwise right shift assignment\n");
}

/**
 * Calls the correct function for compiling the current parser token.
 *
 * @param self The current Compiler struct.
 */
void compiler_compileNext(struct Compiler *self) {
    switch (self->parser->AST->IDENTIFIER) {
    case ASTTOKENS_ASSIGNMENT:
        compiler_compileAssignment(self);
        break;
    case ASTTOKENS_MODULO_ASSIGNMENT:
        compiler_compileModuloAssignment(self);
        break;
    case ASTTOKENS_MULTIPLICATION_ASSIGNMENT:
        compiler_compileMultiplicationAssignment(self);
        break;
    case ASTTOKENS_EXPONENT_ASSIGNMENT:
        compiler_compileExponentAssignment(self);
        break;
    case ASTTOKENS_DIVISION_ASSIGNMENT:
        compiler_compileDivisionAssignment(self);
        break;
    case ASTTOKENS_FLOOR_DIVISION_ASSIGNMENT:
        compiler_compileFloorDivisionAssignment(self);
        break;
    case ASTTOKENS_ADDITION_ASSIGNMENT:
        compiler_compileAdditionAssignment(self);
        break;
    case ASTTOKENS_SUBTRACTION_ASSIGNMENT:
        compiler_compileSubtractionAssignment(self);
        break;
    case ASTTOKENS_BITWISE_AND_ASSIGNMENT:
        compiler_compileBitwiseAndAssignment(self);
        break;
    case ASTTOKENS_BITWISE_OR_ASSIGNMENT:
        compiler_compileBitwiseOrAssignment(self);
        break;
    case ASTTOKENS_BITWISE_XOR_ASSIGNMENT:
        compiler_compileBitwiseXorAssignment(self);
        break;
    case ASTTOKENS_BITWISE_NOT_ASSIGNMENT:
        compiler_compileBitwiseNotAssignment(self);
        break;
    case ASTTOKENS_BITWISE_LEFT_SHIFT_ASSIGNMENT:
        compiler_compileBitwiseLeftShiftAssignment(self);
        break;
    case ASTTOKENS_BITWISE_RIGHT_SHIFT_ASSIGNMENT:
        compiler_compileBitwiseRightShiftAssignment(self);
        break;
    default:
        printf("unsupported parser token for compiler: %s\n",
               astTokens_getName(self->parser->AST->IDENTIFIER)); // TODO: fix
        break;
    }
}

/**
 * Gets the next parser token and compiles it.
 *
 * @param self The current Compiler struct.
 *
 * @return Whether compiling succeeded.
 */
bool compiler_compile(struct Compiler *self) {
    if (!parser_parse(self->parser, true, true)) {
        return false;
    }

    compiler_compileNext(self);

    return true;
}
