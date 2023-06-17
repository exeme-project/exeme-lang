/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include "../includes.c"

#include "../lexer/lexer.c"
#include "../utils/array.c"
#include "../utils/errors.c"

/**
 * Represents an AST.
 */
struct AST {
    enum ASTIdentifiers {
        AST_VARIABLE,
        AST_ASSIGNMENT
    } IDENTIFIER;
    union {
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
    } data;
};

#define AST_STRUCT_SIZE sizeof(struct AST)
#define AST_VARIABLE_STRUCT_SIZE sizeof(struct AST_VARIABLE)
#define AST_ASSIGNMENT_STRUCT_SIZE sizeof(struct AST_ASSIGNMENT)

/* Forward declarations */
void ast_free(struct AST *self);

/**
 * Frees an AST Variable struct.
 *
 * @param self The current AST Variable struct.
 */
void ast_variable_free(struct AST_VARIABLE *self) {
    if (self) {
        lexerToken_free((struct LexerToken *) self->_token);
        string_free((struct String *) self->NAME);

        free(self);
        self = NULL;
    } else {
        panic("AST_VARIABLE struct has already been freed");
    }
}

/**
 * Frees an AST Assignment struct.
 *
 * @param self The current AST Assignment struct.
 */
void ast_assignment_free(struct AST_ASSIGNMENT *self) {
    if (self) {
        lexerToken_free((struct LexerToken *) self->_token);
        ast_free((struct AST *) self->VALUE);
        ast_variable_free((struct AST_VARIABLE *) self->IDENTIFIER);

        free(self);
        self = NULL;
    } else {
        panic("AST_ASSIGNMENT struct has already been freed");
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
struct AST *ast_new__(enum ASTIdentifiers IDENTIFIER, void *data) {
    struct AST *self = malloc(AST_STRUCT_SIZE);

    if (!self) {
        panic("failed to malloc AST struct");
    }

    self->IDENTIFIER = IDENTIFIER;

    switch (self->IDENTIFIER) {
        case AST_VARIABLE:
            self->data.AST_VARIABLE = malloc(AST_VARIABLE_STRUCT_SIZE);

            if (!self->data.AST_VARIABLE) {
                panic("failed to malloc AST_VARIABLE struct");
            }

            memcpy(self->data.AST_VARIABLE, data, AST_VARIABLE_STRUCT_SIZE);
            break;
        case AST_ASSIGNMENT:
            self->data.AST_ASSIGNMENT = malloc(AST_ASSIGNMENT_STRUCT_SIZE);

            if (!self->data.AST_ASSIGNMENT) {
                panic("failed to malloc AST_ASSIGNMENT struct");
            }

            memcpy(self->data.AST_ASSIGNMENT, data, AST_ASSIGNMENT_STRUCT_SIZE);
            break;
    }

    return self;
}

/* Vararg macro to reduce boilerplate */
#define ast_new(type, ...) ast_new__(type, &(struct type){__VA_ARGS__})

/**
 * Frees an AST struct.
 *
 * @param self The current AST struct.
 */
void ast_free(struct AST *self) {
    if (self) {
        switch (self->IDENTIFIER) {
            case AST_VARIABLE:
                ast_variable_free(self->data.AST_VARIABLE);
                break;
            case AST_ASSIGNMENT:
                ast_assignment_free(self->data.AST_ASSIGNMENT);
                break;
        }

        free(self);
        self = NULL;
    } else {
        panic("AST struct has already been freed");
    }
}
