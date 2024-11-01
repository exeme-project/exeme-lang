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
    struct Hashmap *defaultValues, *subcommands;
};

#define arg_init(...)                                                                                                       \
    ((struct Command){.type = COMMAND_TYPE_ARGUMENT, .data.arg = (struct Arg){.position = -1, ##__VA_ARGS__}})
#define subcommand_init(...)                                                                                                \
    ((struct Command){.type = COMMAND_TYPE_SUBCOMMAND, .data.subcommand = (struct Subcommand){__VA_ARGS__}})
#define ARGSFORMAT_STRUCT_SIZE sizeof(struct ArgsFormat)

/* Forward declarations to silence warnings */
void argsFormat___check(struct ArgsFormat *self);
void argsFormat_free(struct ArgsFormat **self);

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
    self->defaultValues = hashmap_new(hashmap___hashDJB2, DEFAULT_INITIAL_TABLE_COUNT, DEFAULT_LOAD_FACTOR);
    self->subcommands = hashmap_new(hashmap___hashDJB2, DEFAULT_INITIAL_TABLE_COUNT, DEFAULT_LOAD_FACTOR);

    struct Array *parsed_arguments = array_new();

    for (size_t index = 0; index < self->argumentsFormat.length; index++) {
        struct Command *command = (struct Command *)self->argumentsFormat._values[index];

        switch (command->type) {
        case COMMAND_TYPE_ARGUMENT: {
            struct Arg *arg = &command->data.arg;

            if (array_contains(parsed_arguments, argsFormat___check_parsedArgumentsNameMatch_, arg->name)) {
                panic("duplicate argument / subcommand name");
            }

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
                } else if ((size_t)arg->position != self->requiredArguments->length + 1) {
                    panic("required argument position must be in order");
                }

                array_append(self->requiredArguments, arg);
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

                hashmap_set(self->defaultValues, arg->name, arg->def);
            }

            array_append(parsed_arguments, command);

            break;
        }
        case COMMAND_TYPE_SUBCOMMAND: {
            struct Subcommand *subcommandRaw = &command->data.subcommand;

            if (array_contains(parsed_arguments, argsFormat___check_parsedArgumentsNameMatch_, subcommandRaw->name)) {
                panic("duplicate subcommand / argument name");
            }

            if (!subcommandRaw->name) {
                panic("subcommand must have a name");
            } else if (!subcommandRaw->help) {
                panic("subcommand must have a help message");
            } else if (subcommandRaw->argumentsFormat.length == 0) {
                panic("subcommand must have arguments");
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

char *argsFormat___error_convertToString_(const void *element) { return (char *)element; }

/**
 * Prints an argument parsing error and exits.
 *
 * @param args             The arguments.
 * @param ERROR_MSG_NUMBER The error message number.
 * @param ERROR_MSG        The error message.
 * @param ARG_INDEX        The index of the erroneous argument (0 is equivalent to NULL since 0 is the binary, so the
 * end).
 */
__attribute__((noreturn)) void args_error(struct Array args, const enum ErrorIdentifiers ERROR_MSG_NUMBER,
                                          const char *ERROR_MSG, const size_t ARG_INDEX) {
    const char *ARGUMENT_INDEX_STRING = ARG_INDEX == 0 ? "-1" : ulToString(ARG_INDEX);
    const size_t ARGUMENT_INDEX_STRING_LENGTH = strlen(ARGUMENT_INDEX_STRING);
    const char *LINE = array_join(&args, " ", argsFormat___error_convertToString_);

    printf("-%s> %s\n%s | %s\n%s", repeatChr('-', ARGUMENT_INDEX_STRING_LENGTH), "~/cli", ARGUMENT_INDEX_STRING, LINE,
           repeatChr(' ', ARGUMENT_INDEX_STRING_LENGTH + 3));

    size_t argumentStartIndex = 0;

    for (size_t index = 0; index < ((ARG_INDEX == 0) ? args.length : ARG_INDEX); index++) {
        argumentStartIndex += strlen((char *)args._values[index]) + 1;
    }

    // argumentStartIndex--; // Remove the last space. NB: Commented out to remove `-1` from the bottom branch below

    if (ARG_INDEX == 0) {
        printf("%s^ ", repeatChr(' ', argumentStartIndex - 2));
    } else {
        printf("%s%s ", repeatChr(' ', argumentStartIndex), repeatChr('^', strlen((char *)args._values[ARG_INDEX])));
    }

    printf("%serror[%s]:%s %s\n", F_BRIGHT_RED, error_get(ERROR_MSG_NUMBER), S_RESET, ERROR_MSG);

    exit(EXIT_FAILURE);
}

bool argsFormat_parse_optionalArgumentFlagMatch_(const void *element, const void *match) {
    struct Command *command = (struct Command *)element;

    switch (command->type) {
    case COMMAND_TYPE_ARGUMENT:
        return strcmp(command->data.arg.flagShort, (char *)match) == 0 ||
               strcmp(command->data.arg.flagLong, (char *)match) == 0;
    case COMMAND_TYPE_SUBCOMMAND:
        return false;
    default:
        panic("invalid command type while parsing optional argument flag match");
    }
}
bool argsFormat_parse_requiredArgumentsExist_(const void *element, const void *_) { return element != NULL; }

/**
 * Parses arguments.
 *
 * @param self The current ArgsFormat struct.
 * @param args The arguments to parse.
 * @param indexOffset The index to start checking arguments from.
 */
struct Hashmap *argsFormat_parse_(struct ArgsFormat *self, struct Array args, size_t indexOffset) {
    struct Hashmap *parsed_args = hashmap_new(hashmap___hashDJB2, DEFAULT_INITIAL_TABLE_COUNT, DEFAULT_LOAD_FACTOR);
    hashmap_combine(parsed_args, self->defaultValues); // Set up the default values

    for (size_t index = indexOffset; index < args.length; index++) {
        char *arg_raw = (char *)args._values[index];
        size_t real_index = index - indexOffset; // The real index of the argument, used with some internal data structures

        if (strncmp(arg_raw, "-", 1) == 0) { // Long / short flag
            bool longFlag = strncmp(arg_raw, "--", 2) == 0;
            int argumentFormatIndex =
                array_find(&self->argumentsFormat, argsFormat_parse_optionalArgumentFlagMatch_, arg_raw);

            if (argumentFormatIndex == -1) {
                args_error(args, A0001, stringConcatenate("unknown ", longFlag ? "long" : "short", " flag"), index);
            } else if (index++ == args.length - 1) {
                args_error(args, A0002, stringConcatenate("missing value for ", longFlag ? "long" : "short", " flag"),
                           real_index);
            }

            struct Arg *arg = &((struct Command *)self->argumentsFormat._values[argumentFormatIndex])->data.arg;
            void *arg_converted = convertToType((char *)args._values[index], arg->type);

            if (!arg_converted) {
                args_error(args, A0003, stringConcatenate("failed to convert argument to ", variableType_get(arg->type)),
                           index);
            }

            hashmap_set(parsed_args, arg->name, arg_converted);
        } else {                                                             // Required argument / Subcommand
            if (array_index_occupied(self->requiredArguments, real_index)) { // Required argument
                struct Arg *arg = (struct Arg *)self->requiredArguments->_values[real_index];
                void *arg_converted = convertToType(arg_raw, arg->type);

                if (!arg_converted) {
                    args_error(args, A0003, stringConcatenate("failed to convert argument to ", variableType_get(arg->type)),
                               index);
                }

                hashmap_set(parsed_args, arg->name, arg_converted);
                self->requiredArguments->_values[real_index] = NULL;
            } else { // Subcommand
                struct ArgsFormat *subcommandFormat = (struct ArgsFormat *)hashmap_get(self->subcommands, arg_raw);

                if (!subcommandFormat) {
                    args_error(args, A0001, "unknown subcommand", index);
                }

                struct Hashmap *subcommandParsedArgs = argsFormat_parse_(subcommandFormat, args, index + 1);
                hashmap_combine(parsed_args, subcommandParsedArgs);

                hashmap_free(&subcommandParsedArgs, NULL);
                argsFormat_free(&subcommandFormat);

                break;
            }
        }
    }

    if (array_contains(self->requiredArguments, argsFormat_parse_requiredArgumentsExist_, NULL)) {
        char *firstArgumentName = "";

        for (size_t index = 0; index < self->requiredArguments->length; index++) {
            struct Arg *arg = (struct Arg *)self->requiredArguments->_values[index];

            if (arg) {
                firstArgumentName = arg->name;

                break;
            }
        }

        args_error(args, A0004, stringConcatenate("missing required argument: ", firstArgumentName), 0);
    }

    return parsed_args;
}

#define argsFormat_parse(self, argv, argc) argsFormat_parse_(self, array_upgrade_stack(argv, argc), 1)

/**
 * Frees an ArgsFormat struct.
 *
 * @param self The current ArgsFormat struct.
 */
void argsFormat_free(struct ArgsFormat **self) {
    if (self && *self) {
        array_free(&(*self)->requiredArguments);     // Required arguments are by default on stack, so don't have to be
                                                     // explicitly freed
        hashmap_free(&(*self)->defaultValues, NULL); // The default values are on the stack.
        hashmap_free(&(*self)->subcommands, NULL);   // Again, the format is on the stack.

        free(*self);

        *self = NULL;
    } else {
        panic("ArgsFormat struct has already been freed");
    }
}
