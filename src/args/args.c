/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#include "../includes.c"

#include "../errors.c"
#include "../utils/array.c"
#include "../utils/conversions.c"
#include "../utils/map.c"
#include "../utils/panic.c"
#include "../utils/string.c"

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
    struct Array argumentsFormat, *requiredArguments;
};

#define arg_init(...) ((struct Arg){.position = -1, ##__VA_ARGS__})
#define ARGS_STRUCT_SIZE sizeof(struct Args)

/* Forward declarations to silence warnings */
void args_check(struct Args *self);

/**
 * Creates a new Args struct.
 *
 * @param argc The number of arguments.
 * @param argv The arguments.
 * @param argumentsFormat The arguments config.
 *
 * @return The created Args struct.
 */
struct Args *args_new(int argc, char **argv, struct Array argumentsFormat) {
    struct Args *self = malloc(ARGS_STRUCT_SIZE);

    if (!self) {
        panic("failed to malloc Args struct");
    }

    self->argv = argv;
    self->argc = argc;

    self->argumentsFormat = argumentsFormat;

    args_check(self);

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

/**
 * Prints an argument parsing error and exits.
 *
 * @param self             The current Parser struct.
 * @param ERROR_MSG_NUMBER The error message number.
 * @param ERROR_MSG        The error message.
 * @param ARG              The erroneous argument.
 */
__attribute__((noreturn)) void args_error(struct Args *self, const enum ErrorIdentifiers ERROR_MSG_NUMBER,
                                          const char *ERROR_MSG, const char *ARG) {
    printf("oops - %s", ERROR_MSG);

    exit(EXIT_FAILURE);
}

bool args_check_matchArgumentNames_(const void *element, const void *match) {
    return strcmp(((struct Arg *)element)->name, ((struct Arg *)match)->name) == 0;
}

/**
 * Checks the config for parsing arguments.
 *
 * @param self The current Args struct.
 */
void args_check(struct Args *self) {
    if (self->requiredArguments) {
        array_free(&self->requiredArguments);
    }

    self->requiredArguments = array_new();
    struct Array *checkedArguments = array_new();

    for (size_t index = 0; index < self->argumentsFormat.length; index++) {
        struct Arg *arg = (struct Arg *)self->argumentsFormat._values[index];

        if (!arg->name || strlen(arg->name) == 0) {
            panic("argument must have a name");
        } else if (!arg->description || strlen(arg->description) == 0) {
            panic("argument must have a description");
        }

        if (arg->position != -1) { /* Required argument */
            if (array_index_occupied(self->requiredArguments, arg->position)) {
                panic("duplicate required argument position");
            } else if ((arg->flagShort && strlen(arg->flagShort) > 0) ||
                       (arg->flagLong && strlen(arg->flagLong) > 0)) { /* Short-circuiting to the rescue */
                panic("required argument cannot have flags");
            } else if (arg->def) {
                panic("required argument cannot have a default value");
            } else if ((size_t)arg->position != self->requiredArguments->length) {
                panic("required argument position must be in order");
            }

            if (array_contains(checkedArguments, args_check_matchArgumentNames_, arg)) {
                panic("duplicate argument name");
            }

            array_append(self->requiredArguments, arg);
            array_append(checkedArguments, arg);
        } else { /* Optional argument */
            if ((!arg->flagShort || strlen(arg->flagShort) == 0) ||
                (!arg->flagLong || strlen(arg->flagLong) == 0)) { /* Again, short-circuiting! */
                panic("optional argument must have a short and long flag");
            } else if (!arg->def) {
                panic("optional argument must have a non-NULL default value");
            }

            if (strncmp(arg->flagShort, "-", 1) != 0) {
                panic("short flag must start with '-'");
            } else if (strncmp(arg->flagLong, "--", 2) != 0) {
                panic("long flag must start with '--'");
            }

            /* Bit more complex than using the array_contains function I also wrote, but in this case its better this way */
            for (size_t j = 0; j < checkedArguments->length; j++) {
                struct Arg *argument = (struct Arg *)checkedArguments->_values[j];

                if (strcmp(arg->name, argument->name) == 0) {
                    panic("duplicate argument name");
                } else if (argument->flagShort &&
                           strcmp(arg->flagShort, argument->flagShort) ==
                               0) { /* Again, short-circuiting, as we only compare the strings if it is a string */
                    panic("duplicate optional argument short flag");
                } else if (argument->flagLong && strcmp(arg->flagLong, argument->flagLong) == 0) {
                    panic("duplicate optional argument long flag");
                }
            }

            array_append(checkedArguments, arg);
        }
    }

    array_free(&checkedArguments);
}

/**
 * Parses arguments.
 *
 * @param self              The current Args struct.
 */
struct Map *args_parse(struct Args *self) {
    struct Map *parsed_args = map_new();
    size_t true_argc = self->argc - 1; /* Index 0 is the executable name */

    for (size_t index = 1; index < self->argc; index++) {
        char *arg = self->argv[index];

        if (arg[0] == '-') {
            if (index - 1 < self->requiredArguments->length) {
                struct Arg *required_arg = (struct Arg *)self->requiredArguments->_values[index - 1];

                args_error(self, A0001,
                           stringConcatenate("expected required argument '", required_arg->name, "' at position ",
                                             ulToString(required_arg->position)),
                           arg);
            }

            if (index++ == true_argc) { /* Essentially checks if index == true_argc, and irrespective, increments index. A
  cool way to combine what should be two statements into one. Just dont't forget what it does :). */
                args_error(self, A0002, "expected argument after flag", arg);
            }

            char *arg_value = self->argv[index];

        } else {
        }
    }

    return parsed_args;
}
