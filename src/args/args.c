/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#include "../includes.c"

#include "../utils/array.c"
#include "../utils/conversions.c"
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

__attribute__((noreturn)) void args_error(char *msg) {
    printf("oops - %s", msg);

    exit(EXIT_FAILURE);
}

bool args_check_matchRequiredArgumentNames_(const void *element, const void *match) {
    return strcmp(((struct Arg *)element)->name, ((struct Arg *)match)->name) == 0;
}

void args_check(struct Args *self) {
    if (self->requiredArguments) {
        array_free(&self->requiredArguments);
    }

    struct Array *optionalArguments = array_new();
    self->requiredArguments = array_new();

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
            }

            if (array_contains(self->requiredArguments, args_check_matchRequiredArgumentNames_, arg)) {
                panic("duplicate required argument name");
            }

            array_insert(self->requiredArguments, arg->position, arg);
        } else { /* Optional argument */
            if ((!arg->flagShort || strlen(arg->flagShort) == 0) ||
                (!arg->flagLong || strlen(arg->flagLong) == 0)) { /* Again, short-circuiting! */
                panic("optional argument must have a short and long flag");
            } else if (!arg->def && arg->type != VARIABLE_TYPE_NULL) { /* Why do NULL variables exist in arguments? No use
                                                                          case pops to mind. But it's fun to implement :). */
                panic("optional argument must have a non-NULL default value, if type is not NULL");
            }

            if (strncmp(arg->flagShort, "-", 1) != 0) {
                panic("short flag must start with '-'");
            } else if (strncmp(arg->flagLong, "--", 2) != 0) {
                panic("long flag must start with '--'");
            }

            /* Bit more complex than using the array_contains function I also wrote, but in this case its better this way */
            for (size_t j = 0; j < optionalArguments->length; j++) {
                struct Arg *optionalArg = (struct Arg *)optionalArguments->_values[j];

                if (strcmp(arg->name, optionalArg->name) == 0) {
                    panic("duplicate optional argument name");
                } else if (strcmp(arg->flagShort, optionalArg->flagShort) == 0) {
                    panic("duplicate optional argument short flag");
                } else if (strcmp(arg->flagLong, optionalArg->flagLong) == 0) {
                    panic("duplicate optional argument long flag");
                }
            }

            array_insert(optionalArguments, optionalArguments->length, arg);
        }
    }

    array_free(&optionalArguments);
}

/**
 * Parses arguments.
 *
 * @param self              The current Args struct.
 */
struct Map *args_parse(struct Args *self) {
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
