/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#include "./includes.c"

#include "./args/args.c"
#include "./compiler/compiler.c"
#include "./utils/hashmap.c"

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

    struct ArgsFormat *argsFormat = argsFormat_new(ARGUMENTS_FORMAT);
    struct Hashmap *parsed_args = argsFormat_parse(argsFormat, (const void **)argv, argc);

    argsFormat_free(&argsFormat);

    struct Compiler *compiler = compiler_new(hashmap_get(parsed_args, "file"));

    while (compiler_compile(compiler)) {
    }

    compiler_free(&compiler);
    hashmap_free(&parsed_args, NULL);
}
