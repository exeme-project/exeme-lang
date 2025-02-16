/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#include "../lexer/lexer.h"
#include "./args/args.h"
#include "./compiler/compiler.h"
#include "./utils/hashmap.h"
#include "./utils/panic.h"
#include <locale.h>

#define NAME	"exeme"
#define VERSION "v0.0.1-alpha"
#define DESCRIPTION                                                                                \
	"an optimised, elegant, and compiled programming language.\nBuilt by skifli@github, FOSS on "  \
	"exeme-project@github."

/*
 * Represents the config for parsing arguments.
 */
static const struct Array g_ARGUMENTS_FORMAT = ARRAY_NEW_STACK(
	&ARG_INIT(.name		   = "stdlib",
			  .description = "The path to the folder containing the standard library",
			  .def = "./../../lib", .flagShort = "-s", .flagLong = "--stdlib",
			  .type = VARIABLE_TYPE_STRING),
	&SUBCOMMAND_INIT(.name = "run", .help = "Runs the specified program",
					 .argumentsFormat = ARRAY_NEW_STACK(
						 &ARG_INIT(.name = "file", .description = "The path of the file to compile",
								   .type = VARIABLE_TYPE_STRING, .position = 1))),
	&SUBCOMMAND_INIT(.name = "build", .help = "Builds the specified program",
					 .argumentsFormat = ARRAY_NEW_STACK(
						 &ARG_INIT(.name = "file", .description = "The path of the file to compile",
								   .type = VARIABLE_TYPE_STRING, .position = 1))));

int main(int argc, char** argv) {
	char* lp_updatedLocale = setlocale(LC_ALL, ""); // NOLINT(concurrency-mt-unsafe)

	if (lp_updatedLocale == NULL) {
		PANIC("failed to update locale to ALL");
	}

	struct ArgsFormat* lp_argsFormat = ARGS_FORMAT_NEW(
		g_ARGUMENTS_FORMAT, NAME, VERSION, DESCRIPTION,
		&ARRAY_NEW_STACK((int*)ARGS_RESERVED_FLAG_HELP,
						 (int*)ARGS_RESERVED_FLAG_VERSION)); // NOLINT(performance-no-int-to-ptr)

	struct Hashmap* lp_parsedArgs = ARGS_FORMAT_PARSE(lp_argsFormat, (const void**)argv, argc);

	args_format_free(&lp_argsFormat);

	void** lp_filePath = hashmap_get(lp_parsedArgs, "file");

	if (!lp_filePath) {
		error("no file path specified");
	}

	struct Compiler* lp_compiler = compiler_new(*lp_filePath);

	while (compiler_compile(lp_compiler)) {
	}

	compiler_free(&lp_compiler);
	hashmap_free(&lp_parsedArgs, NULL);
}
