/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#include "../includes.c"

#include "../utils/panic.c"
#include "./config.c"

/**
 * Represents arguments.
 */
struct Args {
    char **argv;
    int argc;
};

#define ARGS_STRUCT_SIZE sizeof(struct Args)

/**
 * Creates a new Args struct.
 *
 * @param argc The number of arguments.
 * @param argv The arguments.
 *
 * @return The created Args struct.
 */
struct Args *args_new(int argc, char **argv) {
    struct Args *self = malloc(ARGS_STRUCT_SIZE);

    if (!self) {
        panic("failed to malloc Args struct");
    }

    self->argv = argv;
    self->argc = argc;

    return self;
}

/**
 * Frees an Args struct.
 *
 * @param self The current Args struct.
 */
void args_free(struct Args **self) {
    if (self && *self) {
        free(*self);
        *self = NULL;
    } else {
        panic("Args struct has already been freed");
    }
}

void args_parse(struct Args *self) {}
