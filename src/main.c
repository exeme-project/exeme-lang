/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#include "./includes.c"

#include "./args/args.c"
#include "./compiler/compiler.c"
#include "./utils/hashmap.c"

#define NAME "exeme"
#define VERSION "v0.0.1-alpha"
#define DESCRIPTION                                                                                                         \
	"an optimised, elegant, and compiled programming language.\nBuilt by skifli@github, FOSS on exeme-project@github."

/*
 * Represents the config for parsing arguments.
 */
static struct Array ARGUMENTS_FORMAT;

int main(int argc, char **argv) {
	setlocale(LC_ALL, "");

	ARGUMENTS_FORMAT = array_new_stack(
		 &arg_init(.name = "stdlib", .description = "The path to the folder containing the standard library",
					  .def = "./../../lib", .flagShort = "-s", .flagLong = "--stdlib", .type = VARIABLE_TYPE_STRING),
		 &subcommand_init(.name = "run", .help = "Runs the specified program",
								.argumentsFormat =
									 array_new_stack(&arg_init(.name = "file", .description = "The path of the file to compile",
																		.type = VARIABLE_TYPE_STRING, .position = 1))),
		 &subcommand_init(.name = "build", .help = "Builds the specified program",
								.argumentsFormat =
									 array_new_stack(&arg_init(.name = "file", .description = "The path of the file to compile",
																		.type = VARIABLE_TYPE_STRING, .position = 1))));

	struct ArgsFormat *argsFormat =
		 argsFormat_new(ARGUMENTS_FORMAT, NAME, VERSION, DESCRIPTION,
							 &array_new_stack((int *)ARGS_RESERVED_FLAG_HELP, (int *)ARGS_RESERVED_FLAG_VERSION));
	struct Hashmap *parsed_args = argsFormat_parse(argsFormat, (const void **)argv, argc);

	argsFormat_free(&argsFormat);

	void **FILE_PATH = hashmap_get(parsed_args, "file");

	if (!FILE_PATH) {
		error("no file path specified");
	}

	struct Compiler *compiler = compiler_new(*FILE_PATH);

	while (compiler_compile(compiler)) {
	}

	compiler_free(&compiler);
	hashmap_free(&parsed_args, NULL);
}
