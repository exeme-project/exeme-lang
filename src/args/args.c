/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#include "./args.h"
#include "../utils/conversions.h"
#include "../utils/hashmap.h"
#include "../utils/panic.h"
#include "../utils/str.h"
#include <stdio.h>
#include <string.h>

/**
 * Represents values for the reserved flags.
 */
const struct Array* const gp_ARGS_RESERVED_FLAGS_VALUE =
	&ARRAY_NEW_STACK("-h", "--help", "-v", "--version");

// NOLINTBEGIN(bugprone-easily-swappable-parameters)
struct ArgsFormat* args_format_new_internal(struct Array argumentsFormat, const char* p_name,
											const char* p_version, const char* p_description,
											struct Array*			 p_reservedFlags,
											const struct Subcommand* p_subcommandParent) {
	// NOLINTEND(bugprone-easily-swappable-parameters)
	struct ArgsFormat* lp_self = malloc(ARGSFORMAT_STRUCT_SIZE);

	if (!lp_self) {
		PANIC("failed to malloc Args struct");
	}

	lp_self->NAME			   = p_name;
	lp_self->VERSION		   = p_version;
	lp_self->DESCRIPTION	   = p_description;
	lp_self->SUBCOMMAND_PARENT = p_subcommandParent;
	lp_self->argumentsFormat   = argumentsFormat;
	lp_self->reservedFlags	   = NULL;

	if (p_reservedFlags) {
		lp_self->reservedFlags = array_new();

		for (size_t index = 0; index < p_reservedFlags->length; index++) {
#pragma clang diagnostic push // Save previous pragma diagnostic state so we can restore it later
#pragma clang diagnostic ignored                                                                   \
	"-Wvoid-pointer-to-int-cast" // Suppress the warning for the following code block
			// Well, this isn't liked by the compiler. But, it's the only way...
			// When creating the reserved flags, the flags are ENUM values (so int)
			// They are casted to (int *) when added to the array since (array_new_stack) has values
			// of const void* Obviously they are NOT int pointers, but that's the only way to make
			// it a pointer (to an int) Then here, we retrieve them. However, converting from const
			// void * to int is a pointer to non-pointer cast Why can't we just do: *(int
			// *)reservedFlags->_values[index]; Because that would be dereferencing a non-pointer,
			// which is invalid - and would cause a segfault So, we just cast to int And trade a
			// segfault for a compiler warningx
			int reservedFlag = (int)p_reservedFlags->_values[index];
#pragma clang diagnostic pop // Restore the previous pragma diagnostic state

			array_append(lp_self->reservedFlags,
						 gp_ARGS_RESERVED_FLAGS_VALUE->_values[reservedFlag]);
			array_append(lp_self->reservedFlags,
						 gp_ARGS_RESERVED_FLAGS_VALUE->_values[reservedFlag + 1]);
		}
	}

	args_format_check(lp_self);

	return lp_self;
}

bool args_format_check_parsed_arguments_name_match(
	const void* p_element, const void* p_match) { // NOLINT(bugprone-easily-swappable-parameters)
	char* lp_elementName = NULL;

	switch (((struct Command*)p_element)->type) {
	case COMMAND_TYPE_ARGUMENT:
		lp_elementName = ((struct Command*)p_element)->data.arg.name;
		break;
	case COMMAND_TYPE_SUBCOMMAND:
		lp_elementName = ((struct Command*)p_element)->data.subcommand.name;
		break;
	default:
		PANIC("invalid command type while checking parsed arguments name match");
	}

	return strcmp(lp_elementName, p_match) == 0;
}

bool args_format_check_parsed_arguments_short_flag_match(
	const void* p_element, const void* p_match) { // NOLINT(bugprone-easily-swappable-parameters)
	char* lp_elementFlagShort = NULL;

	switch (((struct Command*)p_element)->type) {
	case COMMAND_TYPE_ARGUMENT:
		lp_elementFlagShort = ((struct Command*)p_element)->data.arg.flagShort;
		break;
	case COMMAND_TYPE_SUBCOMMAND:
		return false;
	default:
		PANIC("invalid command type while checking parsed arguments short flag match");
	}

	if (!lp_elementFlagShort) { // Some arguments don't have a short flag
		return false;
	}

	return strcmp(lp_elementFlagShort, p_match) == 0;
}

bool args_format_check_parsed_arguments_long_flag_match(
	const void* p_element, const void* p_match) { // NOLINT(bugprone-easily-swappable-parameters)
	char* lp_elementFlagLong = NULL;

	switch (((struct Command*)p_element)->type) {
	case COMMAND_TYPE_ARGUMENT:
		lp_elementFlagLong = ((struct Command*)p_element)->data.arg.flagLong;
		break;
	case COMMAND_TYPE_SUBCOMMAND:
		return false;
	default:
		PANIC("invalid command type while checking parsed arguments long flag match");
	}

	return strcmp(lp_elementFlagLong, p_match) == 0;
}

void args_format_check(struct ArgsFormat* p_self) {
	p_self->requiredArguments = array_new();
	p_self->defaultValues =
		hashmap_new(hashmap_hash_djb2, DEFAULT_INITIAL_TABLE_COUNT, DEFAULT_LOAD_FACTOR);
	p_self->subcommands =
		hashmap_new(hashmap_hash_djb2, DEFAULT_INITIAL_TABLE_COUNT, DEFAULT_LOAD_FACTOR);

	struct Array* lp_parsedArguments = array_new();

	for (size_t index = 0; index < p_self->argumentsFormat.length; index++) {
		struct Command* lp_command = (struct Command*)p_self->argumentsFormat._values[index];

		switch (lp_command->type) {
		case COMMAND_TYPE_ARGUMENT: {
			struct Arg* lp_arg = &lp_command->data.arg;

			if (array_contains(lp_parsedArguments, args_format_check_parsed_arguments_name_match,
							   lp_arg->name)) {
				PANIC("duplicate argument / subcommand name");
			}

			if (!lp_arg->name) {
				PANIC("argument must have a name");
			} else if (!lp_arg->description) {
				PANIC("argument must have a description");
			}

			if (lp_arg->position != -1) { /* Required argument */
				if (array_index_occupied(p_self->requiredArguments, lp_arg->position)) {
					PANIC("duplicate required argument position");
				} else if (lp_arg->flagShort || lp_arg->flagLong) {
					PANIC("required argument cannot have a flag");
				} else if (lp_arg->def) {
					PANIC("required argument cannot have a default value");
				} else if ((size_t)lp_arg->position != p_self->requiredArguments->length + 1) {
					PANIC("required argument position must be in order");
				}

				array_append(p_self->requiredArguments, lp_arg);
			} else { /* Optional argument/flag */
				if (!lp_arg->flagLong) {
					PANIC("optional argument must have at least a long flag");
				} else if (lp_arg->type != VARIABLE_TYPE_NONE && !lp_arg->def) {
					PANIC("optional argument must have a non-NULL default value");
				} else if (lp_arg->type == VARIABLE_TYPE_NONE && lp_arg->def) {
					PANIC("optional flag must have a NULL default value");
				} else if (lp_arg->flagShort && strncmp(lp_arg->flagShort, "-", 1) != 0) {
					PANIC("short flag must start with '-'");
				} else if (strncmp(lp_arg->flagLong, "--", 2) != 0) {
					PANIC("long flag must start with '--'");
				} else if (lp_arg->flagShort
						   && array_contains(lp_parsedArguments,
											 args_format_check_parsed_arguments_short_flag_match,
											 lp_arg->flagShort)) {
					PANIC("duplicate short flag");
				} else if (array_contains(lp_parsedArguments,
										  args_format_check_parsed_arguments_long_flag_match,
										  lp_arg->flagLong)) {
					PANIC("duplicate long flag");
				} else if (lp_arg->flagShort
						   && array_contains(p_self->reservedFlags, array___match_string,
											 lp_arg->flagShort)) {
					PANIC("reserved short flag");
				} else if (array_contains(p_self->reservedFlags, array___match_string,
										  lp_arg->flagLong)) {
					PANIC("reserved long flag");
				}

				if (lp_arg->def) { // Optional argument (not flag)
					hashmap_set(p_self->defaultValues, lp_arg->name, lp_arg->def);
				}
			}

			array_append(lp_parsedArguments, lp_command);

			break;
		}
		case COMMAND_TYPE_SUBCOMMAND: {
			struct Subcommand* lp_subcommandRaw = &lp_command->data.subcommand;

			if (array_contains(lp_parsedArguments, args_format_check_parsed_arguments_name_match,
							   lp_subcommandRaw->name)) {
				PANIC("duplicate subcommand / argument name");
			}

			if (!lp_subcommandRaw->name) {
				PANIC("subcommand must have a name");
			} else if (!lp_subcommandRaw->help) {
				PANIC("subcommand must have a help message");
			} else if (lp_subcommandRaw->argumentsFormat.length == 0) {
				PANIC("subcommand must have arguments");
			}

			struct ArgsFormat* lp_subcommandFormat = args_format_new_internal(
				lp_subcommandRaw->argumentsFormat, p_self->NAME, p_self->VERSION,
				p_self->DESCRIPTION,
				array_contains(
					p_self->reservedFlags, array___match_string,
					(void*)gp_ARGS_RESERVED_FLAGS_VALUE->_values[ARGS_RESERVED_FLAG_HELP])
					? &ARRAY_NEW_STACK((int*)ARGS_RESERVED_FLAG_HELP)
					: NULL,
				lp_subcommandRaw);
			array_append(lp_parsedArguments, lp_command);

			hashmap_set(p_self->subcommands, lp_subcommandRaw->name, lp_subcommandFormat);

			break;
		}
		default:
			PANIC("invalid command type while parsing arguments format");
		}
	}

	array_free(&lp_parsedArguments);
}

char* args_format_error_convert_to_string(const void* p_element) { return (char*)p_element; }

__attribute__((noreturn)) void args_error(struct Array				  args,
										  const enum ErrorIdentifiers ERROR_MSG_NUMBER,
										  const char* p_errorMsg, const size_t ARG_INDEX) {
	const char*	 lp_argumentIndexString			= ARG_INDEX == 0 ? "-1" : ul_to_string(ARG_INDEX);
	const size_t l_ARGUMENT_INDEX_STRING_LENGTH = strlen_safe(lp_argumentIndexString);
	const char*	 lp_line = array_join(&args, " ", args_format_error_convert_to_string);

	printf("-%s> %s\n%s | %s\n%s", repeat_chr('-', l_ARGUMENT_INDEX_STRING_LENGTH), "~/cli",
		   lp_argumentIndexString, lp_line, repeat_chr(' ', l_ARGUMENT_INDEX_STRING_LENGTH + 3));

	size_t argumentStartIndex = 0;

	for (size_t index = 0; index < ((ARG_INDEX == 0) ? args.length : ARG_INDEX); index++) {
		argumentStartIndex += strlen_safe((char*)args._values[index]) + 1;
	}

	// argumentStartIndex--; // Remove the last space. NB: Commented out to remove `-1` from the
	// bottom branch below

	if (ARG_INDEX == 0) {
		printf("%s^ ", repeat_chr(' ', argumentStartIndex - 2));
	} else {
		printf("%s%s ", repeat_chr(' ', argumentStartIndex),
			   repeat_chr('^', strlen_safe((char*)args._values[ARG_INDEX])));
	}

	printf("%serror[%s]:%s %s\n", gp_F_BRIGHT_RED, error_get(ERROR_MSG_NUMBER), gp_S_RESET,
		   p_errorMsg);

	exit(EXIT_FAILURE); // NOLINT(concurrency-mt-unsafe)
}

bool args_format_parse_optional_argument_flag_match(
	const void* p_element, const void* p_match) { // NOLINT(bugprone-easily-swappable-parameters)
	struct Command* lp_command = (struct Command*)p_element;

	switch (lp_command->type) {
	case COMMAND_TYPE_ARGUMENT:
		if (lp_command->data.arg.position != -1) { // Required argument
			return false;
		} else {
			return (lp_command->data.arg.flagShort
						? strcmp(lp_command->data.arg.flagShort, (char*)p_match) == 0
						: false)
				   || strcmp(lp_command->data.arg.flagLong, (char*)p_match) == 0;
		}
	case COMMAND_TYPE_SUBCOMMAND:
		return false;
	default:
		PANIC("invalid command type while parsing optional argument flag match");
	}
}

// NOLINTBEGIN(bugprone-easily-swappable-parameters, misc-unused-parameters,
// readability-identifier-length, readability-identifier-naming)
bool args_format_parse_required_arguments_exist(const void* p_element, const void* _) {
	// NOLINTEND(bugprone-easily-swappable-parameters, misc-unused-parameters,
	// readability-identifier-length, readability-identifier-naming)
	return p_element != NULL;
}

__attribute__((noreturn)) void args_format_help(struct ArgsFormat* p_self) {
	if (p_self->SUBCOMMAND_PARENT) {
		printf("%s\n\n", p_self->SUBCOMMAND_PARENT->help);
	} else {
		printf("%s (%s) is %s\n\n", p_self->NAME, p_self->VERSION, p_self->DESCRIPTION);
	}

	printf("Usage: %s [command] [arguments] [options]\n", p_self->NAME);

	struct String* lp_commandString			  = string_new("\0", true);
	struct String* lp_requiredArgumentsString = string_new("\0", true);
	struct String* lp_optionsString			  = string_new("\0", true);
	struct String* lp_flagsString			  = string_new("\0", true);

	for (size_t index = 0; index < p_self->argumentsFormat.length; index++) {
		struct Command* lp_command = (struct Command*)p_self->argumentsFormat._values[index];

		switch (lp_command->type) {
		case COMMAND_TYPE_SUBCOMMAND: {
			struct Subcommand* lp_subcommand = &lp_command->data.subcommand;
			char*			   lp_helpStr =
				CONCATENATE_STRING("  ", lp_subcommand->name, "\t", lp_subcommand->help, "\n");

			string_append_str(lp_commandString, lp_helpStr);
			free(lp_helpStr);

			break;
		}
		case COMMAND_TYPE_ARGUMENT: {
			struct Arg* lp_arg = &lp_command->data.arg;

			if (lp_arg->type == VARIABLE_TYPE_NONE) {
				if (lp_arg->position == -1) {
					char* lp_helpStr =
						CONCATENATE_STRING("  ", lp_arg->flagShort ? lp_arg->flagShort : "\b", " ",
										   lp_arg->flagLong, "\t", lp_arg->description, "\n");

					string_append_str(lp_flagsString, lp_helpStr);
					free(lp_helpStr);
				}
			} else {
				if (lp_arg->position == -1) {
					char* lp_helpStr = CONCATENATE_STRING(
						"  ", lp_arg->flagShort ? lp_arg->flagShort : "\b", " ", lp_arg->flagLong,
						"\t", lp_arg->description, " (", variable_type_get(lp_arg->type), ")\n");

					string_append_str(lp_optionsString, lp_helpStr);
					free(lp_helpStr);
				} else {
					char* lp_helpStr =
						CONCATENATE_STRING("  ", lp_arg->name, "\t", lp_arg->description, " (",
										   variable_type_get(lp_arg->type), ")\n");

					string_append_str(lp_requiredArgumentsString, lp_helpStr);
					free(lp_helpStr);
				}
			}

			break;
		}
		default:
			PANIC("invalid command type while printing help message");
		}
	}

	if (lp_commandString->length > 0) {
		printf("\nCommands:\n%s", lp_commandString->_value);
	}

	if (lp_requiredArgumentsString->length > 0) {
		printf("\nRequired arguments:\n%s", lp_requiredArgumentsString->_value);
	}

	if (lp_optionsString->length > 0) {
		printf("\nOptions:\n%s", lp_optionsString->_value);
	}

	if (lp_flagsString->length > 0) {
		printf("\nFlags:\n%s", lp_flagsString->_value);
	}

	string_free(&lp_commandString);
	string_free(&lp_requiredArgumentsString);
	string_free(&lp_optionsString);
	string_free(&lp_flagsString);

	exit(EXIT_SUCCESS); // NOLINT(concurrency-mt-unsafe)
}

struct Hashmap* args_format_parse_internal(struct ArgsFormat* p_self, struct Array args,
										   size_t indexOffset) {
	struct Hashmap* lp_parsedArgs =
		hashmap_new(hashmap_hash_djb2, DEFAULT_INITIAL_TABLE_COUNT, DEFAULT_LOAD_FACTOR);
	hashmap_combine(lp_parsedArgs, p_self->defaultValues); // Set up the default values

	for (size_t index = indexOffset; index < args.length; index++) {
		char*  lp_argRaw = (char*)args._values[index];
		size_t realIndex = index - indexOffset; // The real index of the argument, used with some
												// internal data structures

		if (strncmp(lp_argRaw, "-", 1) == 0) { // Long / short flag
			bool longFlag = strncmp(lp_argRaw, "--", 2) == 0;
			int	 argumentFormatIndex =
				array_find(&p_self->argumentsFormat, args_format_parse_optional_argument_flag_match,
						   lp_argRaw);

			if (argumentFormatIndex == -1) {
				int reservedFlagsIndex =
					array_find(p_self->reservedFlags, array___match_string, lp_argRaw);

				if (reservedFlagsIndex != -1) {
					switch (reservedFlagsIndex
							- (reservedFlagsIndex
							   % 2)) { // Converts to nearest even number, starting from 0
					case ARGS_RESERVED_FLAG_HELP:
						args_format_help(p_self);
					case ARGS_RESERVED_FLAG_VERSION:
						printf("%s %s\n", p_self->NAME, p_self->VERSION);

						exit(EXIT_SUCCESS); // NOLINT(concurrency-mt-unsafe)
					default:
						PANIC("invalid reserved flag index");
					}
				} else {
					args_error(args, A0001,
							   CONCATENATE_STRING("unknown ", longFlag ? "long" : "short", " flag"),
							   index);
				}
			} else {
				struct Arg* lp_arg =
					&((struct Command*)p_self->argumentsFormat._values[argumentFormatIndex])
						 ->data.arg;

				if (lp_arg->type == VARIABLE_TYPE_NONE) { // Optional flag
					hashmap_set(lp_parsedArgs, lp_arg->name, NULL);
				} else if (index++ == args.length - 1) { // Optional argument and no value
					args_error(args, A0002,
							   CONCATENATE_STRING("missing value for ", longFlag ? "long" : "short",
												  " flag"),
							   realIndex);
				} else { // Optional argument and value
					void* lp_argConverted =
						convert_to_type((char*)args._values[index], lp_arg->type);

					if (!lp_argConverted) {
						args_error(args, A0003,
								   CONCATENATE_STRING("failed to convert argument to ",
													  variable_type_get(lp_arg->type)),
								   index);
					}

					hashmap_set(lp_parsedArgs, lp_arg->name, lp_argConverted);
				}
			}
		} else { // Required argument / Subcommand
			if (array_index_occupied(p_self->requiredArguments, realIndex)) { // Required argument
				struct Arg* lp_arg = (struct Arg*)p_self->requiredArguments->_values[realIndex];
				void*		lp_argConverted = convert_to_type(lp_argRaw, lp_arg->type);

				if (!lp_argConverted) {
					args_error(args, A0003,
							   CONCATENATE_STRING("failed to convert argument to ",
												  variable_type_get(lp_arg->type)),
							   index);
				}

				hashmap_set(lp_parsedArgs, lp_arg->name, lp_argConverted);
				p_self->requiredArguments->_values[realIndex] = NULL;
			} else { // Subcommand
				struct ArgsFormat** lp_subcommandFormatPointer =
					(struct ArgsFormat**)hashmap_get(p_self->subcommands, lp_argRaw);

				if (!lp_subcommandFormatPointer || !*lp_subcommandFormatPointer) {
					args_error(args, A0001, "unknown subcommand", index);
				}

				struct ArgsFormat* lp_subcommandFormat = *lp_subcommandFormatPointer;

				struct Hashmap* lp_subcommandParsedArgs =
					args_format_parse_internal(lp_subcommandFormat, args, index + 1);
				hashmap_combine(lp_parsedArgs, lp_subcommandParsedArgs);

				hashmap_free(&lp_subcommandParsedArgs, NULL);
				args_format_free(&lp_subcommandFormat);

				break;
			}
		}
	}

	if (array_contains(p_self->requiredArguments, args_format_parse_required_arguments_exist,
					   NULL)) {
		char* lp_firstArgumentName = "";

		for (size_t index = 0; index < p_self->requiredArguments->length; index++) {
			struct Arg* lp_arg = (struct Arg*)p_self->requiredArguments->_values[index];

			if (lp_arg) {
				lp_firstArgumentName = lp_arg->name;

				break;
			}
		}

		args_error(args, A0004,
				   CONCATENATE_STRING("missing required argument: ", lp_firstArgumentName), 0);
	}

	return lp_parsedArgs;
}

void args_format_free(struct ArgsFormat** p_self) {
	if (p_self && *p_self) {
		array_free(&(*p_self)->requiredArguments); // Required arguments (inner) are by default on
												   // stack, so don't have to be explicitly freed

		if ((*p_self)->reservedFlags) {			   // Can be NULL if no reserved flags
			array_free(&(*p_self)->reservedFlags); // Reserved flags are on the stack
		}

		hashmap_free(&(*p_self)->defaultValues, NULL); // The default values are on the stack
		hashmap_free(&(*p_self)->subcommands, NULL);   // Again, the format is on the stack

		free(*p_self);

		*p_self = NULL;
	} else {
		PANIC("ArgsFormat struct has already been freed");
	}
}
