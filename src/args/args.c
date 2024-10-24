/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#include "../includes.c"

#include "../errors.c"
#include "../utils/array.c"
#include "../utils/conversions.c"
#include "../utils/hashmap.c"
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
 * Represents a subcommand.
 */
struct Subcommand {
    char *name, *help;
    struct Array argumentsFormat;
};

/**
 * Represents a command type.
 */
enum CommandType {
    COMMAND_TYPE_ARGUMENT,
    COMMAND_TYPE_SUBCOMMAND,
};

/**
 * Represents a command.
 */
struct Command {
    enum CommandType type;
    union {
        struct Arg arg;
        struct Subcommand subcommand;
    } data;
};

/**
 * Represents arguments.
 */
struct ArgsFormat {
    struct Array argumentsFormat;
    struct Array *requiredArguments;
    struct Hashmap *subcommands;
};

#define arg_init(...)                                                                                                       \
    ((struct Command){.type = COMMAND_TYPE_ARGUMENT, .data.arg = (struct Arg){.position = -1, ##__VA_ARGS__}})
#define subcommand_init(...)                                                                                                \
    ((struct Command){.type = COMMAND_TYPE_SUBCOMMAND, .data.subcommand = (struct Subcommand){__VA_ARGS__}})
#define ARGSFORMAT_STRUCT_SIZE sizeof(struct ArgsFormat)

/* Forward declarations to silence warnings */
void argsFormat___check(struct ArgsFormat *self);

/**
 * Creates a new ArgsFormat struct.
 *
 * @param argumentsFormat The arguments config.
 *
 * @return The created Args struct.
 */
struct ArgsFormat *argsFormat_new(struct Array argumentsFormat) {
    struct ArgsFormat *self = malloc(ARGSFORMAT_STRUCT_SIZE);

    if (!self) {
        panic("failed to malloc Args struct");
    }

    self->argumentsFormat = argumentsFormat;

    argsFormat___check(self);

    return self;
}

bool argsFormat___check_parsedArgumentsNameMatch_(const void *element, const void *match) {
    char *elementName = NULL;

    switch (((struct Command *)element)->type) {
    case COMMAND_TYPE_ARGUMENT:
        elementName = ((struct Command *)element)->data.arg.name;
        break;
    case COMMAND_TYPE_SUBCOMMAND:
        elementName = ((struct Command *)element)->data.subcommand.name;
        break;
    default:
        panic("invalid command type while checking parsed arguments name match");
    }

    return strcmp(elementName, match) == 0;
}

bool argsFormat___check_parsedArgumentsShortFlagMatch_(const void *element, const void *match) {
    char *elementFlagShort = NULL;

    switch (((struct Command *)element)->type) {
    case COMMAND_TYPE_ARGUMENT:
        elementFlagShort = ((struct Command *)element)->data.arg.flagShort;
        break;
    case COMMAND_TYPE_SUBCOMMAND:
        return false;
    default:
        panic("invalid command type while checking parsed arguments short flag match");
    }

    return strcmp(elementFlagShort, match) == 0;
}

bool argsFormat___check_parsedArgumentsLongFlagMatch_(const void *element, const void *match) {
    char *elementFlagLong = NULL;

    switch (((struct Command *)element)->type) {
    case COMMAND_TYPE_ARGUMENT:
        elementFlagLong = ((struct Command *)element)->data.arg.flagLong;
        break;
    case COMMAND_TYPE_SUBCOMMAND:
        return false;
    default:
        panic("invalid command type while checking parsed arguments long flag match");
    }

    return strcmp(elementFlagLong, match) == 0;
}

/**
 * Checks the config for parsing arguments.
 *
 * @param self The current Args struct.
 */
void argsFormat___check(struct ArgsFormat *self) {
    self->requiredArguments = array_new();
    self->subcommands = hashmap_new(hashmap___hashDJB2, DEFAULT_INITIAL_TABLE_COUNT, DEFAULT_LOAD_FACTOR);

    struct Array *parsed_arguments = array_new();

    for (size_t index = 0; index < self->argumentsFormat.length; index++) {
        struct Command *command = (struct Command *)array_get(&self->argumentsFormat, index);

        switch (command->type) {
        case COMMAND_TYPE_ARGUMENT: {
            struct Arg *arg = &command->data.arg;

            if (!arg->name) {
                panic("argument must have a name");
            } else if (!arg->description) {
                panic("argument must have a description");
            }

            if (arg->position != -1) { /* Required argument */
                if (array_index_occupied(self->requiredArguments, arg->position)) {
                    panic("duplicate required argument position");
                } else if (arg->flagShort || arg->flagLong) {
                    panic("required argument cannot have a flag");
                } else if (arg->def) {
                    panic("required argument cannot have a default value");
                } else if ((size_t)arg->position != self->requiredArguments->length) {
                    panic("required argument position must be in order");
                }
            } else { /* Optional argument */
                if (!arg->flagShort || !arg->flagLong) {
                    panic("optional argument must have a short and long flag");
                } else if (!arg->def) {
                    panic("optional argument must have a non-NULL default value");
                } else if (strncmp(arg->flagShort, "-", 1) != 0) {
                    panic("short flag must start with '-'");
                } else if (strncmp(arg->flagLong, "--", 2) != 0) {
                    panic("long flag must start with '--'");
                } else if (array_contains(parsed_arguments, argsFormat___check_parsedArgumentsShortFlagMatch_,
                                          arg->flagShort)) {
                    panic("duplicate short flag");
                } else if (array_contains(parsed_arguments, argsFormat___check_parsedArgumentsLongFlagMatch_,
                                          arg->flagLong)) {
                    panic("duplicate long flag");
                }
            }

            if (array_contains(parsed_arguments, argsFormat___check_parsedArgumentsNameMatch_, arg->name)) {
                panic("duplicate argument / subcommand name");
            }

            array_append(parsed_arguments, command);

            break;
        }
        case COMMAND_TYPE_SUBCOMMAND: {
            struct Subcommand *subcommandRaw = &command->data.subcommand;

            if (!subcommandRaw->name) {
                panic("subcommand must have a name");
            } else if (!subcommandRaw->help) {
                panic("subcommand must have a help message");
            } else if (subcommandRaw->argumentsFormat.length == 0) {
                panic("subcommand must have arguments");
            }

            if (array_contains(parsed_arguments, argsFormat___check_parsedArgumentsNameMatch_, subcommandRaw->name)) {
                panic("duplicate subcommand / argument name");
            }

            struct ArgsFormat *subcommandFormat = argsFormat_new(subcommandRaw->argumentsFormat);
            array_append(parsed_arguments, command);

            hashmap_set(self->subcommands, subcommandRaw->name, subcommandFormat);

            break;
        }
        default:
            panic("invalid command type while parsing arguments format");
        }
    }

    array_free(&parsed_arguments);
}

/**
 * Frees an ArgsFormat struct.
 *
 * @param self The current ArgsFormat struct.
 */
void argsFormat_free(struct ArgsFormat **self) {
    if (self && *self) {
        free(*self);
        *self = NULL;
    } else {
        panic("ArgsFormat struct has already been freed");
    }
}

/**
 * Prints an argument parsing error and exits.
 *
 * @param self             The current ArgsFormat struct.
 * @param ERROR_MSG_NUMBER The error message number.
 * @param ERROR_MSG        The error message.
 * @param ARG              The index of the erroneous argument (0 is equivalent to NULL since 0 is the binary).
 */
__attribute__((noreturn)) void args_error(struct ArgsFormat *self, const enum ErrorIdentifiers ERROR_MSG_NUMBER,
                                          const char *ERROR_MSG, const size_t ARG) {
    printf("oops - %s", ERROR_MSG);

    exit(EXIT_FAILURE);
}

/**
 * Parses arguments.
 *
 * @param self The current ArgsFormat struct.
 * @param args The arguments to parse.
 * @param indexOffset The index to start checking arguments from.
 */
struct Hashmap *argsFormat_parse_(struct ArgsFormat *self, struct Array args, size_t indexOffset) {
    struct Hashmap *parsed_args = hashmap_new(hashmap___hashDJB2, DEFAULT_INITIAL_TABLE_COUNT, DEFAULT_LOAD_FACTOR);

    return parsed_args;
}

#define argsFormat_parse(self, argv) argsFormat_parse_(self, array_new_stack((void **)argv), 1)
