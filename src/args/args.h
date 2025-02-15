/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#pragma once

#include "../errors.h"
#include "../globals.h"

/**
 * Represents an argument.
 */
struct Arg {
	// cppcheck-suppress unusedStructMember
	char *			  name, *description, *def, *flagShort, *flagLong;
	enum VariableType type;
	// cppcheck-suppress unusedStructMember
	int position; /* If != NULL then required, else not required */
				  // cppcheck-suppress-end unusedStructMember
};

/**
 * Represents a subcommand.
 */
struct Subcommand {
	char *		 name, *help;
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
		struct Arg		  arg;
		struct Subcommand subcommand;
	} data;
};

/**
 * Represents arguments.
 */
struct ArgsFormat {
	const char *			 NAME, *VERSION, *DESCRIPTION;
	const struct Subcommand* SUBCOMMAND_PARENT;
	struct Array			 argumentsFormat;
	struct Array *			 requiredArguments, *reservedFlags;
	struct Hashmap *		 defaultValues, *subcommands;
};

extern const struct Array* const gp_ARGS_RESERVED_FLAGS_VALUE;

/**
 * Represents reserved flags.
 */
enum ArgsReservedFlags {
	ARGS_RESERVED_FLAG_HELP	   = 0,
	ARGS_RESERVED_FLAG_VERSION = 2,
};

#define ARG_INIT(...)                                                                              \
	((struct Command){.type		= COMMAND_TYPE_ARGUMENT,                                           \
					  .data.arg = (struct Arg){.position = -1, ##__VA_ARGS__}})
#define SUBCOMMAND_INIT(...)                                                                       \
	((struct Command){.type			   = COMMAND_TYPE_SUBCOMMAND,                                  \
					  .data.subcommand = (struct Subcommand){__VA_ARGS__}})
#define ARGSFORMAT_STRUCT_SIZE sizeof(struct ArgsFormat)

/**
 * Creates a new ArgsFormat struct.
 *
 * @param argumentsFormat    The arguments config.
 * @param p_name             The name of the program.
 * @param p_version          The version of the program.
 * @param p_description      The description of the program.
 * @param p_reservedFlags    The reserved flags.
 * @param p_subcommandParent The parent subcommand.
 *
 * @return The created Args struct.
 */
struct ArgsFormat* args_format_new_internal(struct Array argumentsFormat, const char* p_name,
											const char* p_version, const char* p_description,
											struct Array*			 p_reservedFlags,
											const struct Subcommand* p_subcommandParent);

#define ARGS_FORMAT_NEW(argumentsFormat, NAME, VERSION, DESCRIPTION, reservedFlags)                \
	args_format_new_internal(argumentsFormat, NAME, VERSION, DESCRIPTION, reservedFlags, NULL)

/**
 * Checks the config for parsing arguments.
 *
 * @param p_self The current Args struct.
 */
void args_format_check(struct ArgsFormat* p_self);

/**
 * Prints an argument parsing error and exits.
 *
 * @param args             The arguments.
 * @param ERROR_MSG_NUMBER The error message number.
 * @param p_errorMsg       The error message.
 * @param ARG_INDEX        The index of the erroneous argument (0 is equivalent to NULL since 0 is
 *                         the binary, so the end).
 */
// NOLINTBEGIN(readability-avoid-const-params-in-decls)
__attribute__((noreturn)) void args_error(struct Array				  args,
										  const enum ErrorIdentifiers ERROR_MSG_NUMBER,
										  const char* p_errorMsg, const size_t ARG_INDEX);
// NOLINTEND(readability-avoid-const-params-in-decls)

/**
 * Prints the help message and exits.
 *
 * @param p_self The current ArgsFormat struct.
 */
__attribute__((noreturn)) void args_format_help(struct ArgsFormat* p_self);

/**
 * Parses arguments.
 *
 * @param p_self The current ArgsFormat struct.
 * @param args The arguments to parse.
 * @param indexOffset The index to start checking arguments from.
 *
 * @return The parsed arguments as a hashmap.
 */
struct Hashmap* args_format_parse(struct ArgsFormat* p_self, struct Array args, size_t indexOffset);

/**
 * Frees an ArgsFormat struct.
 *
 * @param p_self The current ArgsFormat struct.
 */
void args_format_free(struct ArgsFormat** p_self);
