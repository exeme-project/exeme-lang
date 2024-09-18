/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#include "../includes.c"

#include "../utils/array.c"
#include "../utils/map.c"
#include "../utils/panic.c"

/**
 * Represents an argument.
 */
struct Arg {
    char *name, *description, *def, *flagShort, *flagLong;
    enum VariableType type;
    int position; /* If != NULL then required, else not required */
};

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

__attribute__((noreturn)) void args_error(char *msg) {
    printf("oops - %s", msg);

    exit(EXIT_FAILURE);
}

/**
 * Parses arguments.
 *
 * @param self              The current Args struct.
 * @param ARGUMENTS_CONFIG  The arguments configuration.
 */
struct Map *args_parse(struct Args *self, const struct Array ARGUMENTS_CONFIG) {
    struct Map *parsed_args = map_new();
    int true_argc = self->argc - 1; /* Index 0 is the executable name */

    for (int index = 1; index < self->argc; index++) {
        char *arg = self->argv[index];

        if (arg[0] == '-') {
            /* Essentially checks if index == true_argc, and irrespective, increments index. A cool way to combine what
              should be two statements into one. Just dont't forget what it does :). */

            if (index++ == true_argc) {
                args_error("expected argument after flag");
            }

            char *arg_value = self->argv[index];
        } else {
        }
    }

    exit(EXIT_SUCCESS); // FIXME: REMOVE THIS

    return parsed_args;
}
