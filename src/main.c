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
static const struct Array ARGUMENTS_FORMAT = array_new_stack(
    &arg_init(.name = "stdlib", .description = "The path to the folder containing the standard library",
              .def = "./../../lib", .flagShort = "-s", .flagLong = "--stdlib", .type = VARIABLE_TYPE_STRING),
    &subcommand_init(.name = "run", .help = "Runs the specified program",
                     .argumentsFormat =
                         array_new_stack(&arg_init(.name = "file", .description = "The path of the file to compile",
                                                   .type = VARIABLE_TYPE_STRING, .position = 0))),
    &subcommand_init(.name = "build", .help = "Builds the specified program",
                     .argumentsFormat =
                         array_new_stack(&arg_init(.name = "file", .description = "The path of the file to compile",
                                                   .type = VARIABLE_TYPE_STRING, .position = 0))));

int main(int _, char **argv) {
    setlocale(LC_ALL, "");

    struct ArgsFormat *argsFormat = argsFormat_new(ARGUMENTS_FORMAT);
    struct Hashmap *parsed_args = argsFormat_parse(argsFormat, argv);

    struct Compiler *compiler = compiler_new("../../programs/complex.exl");

    while (compiler_compile(compiler)) {
    }

    compiler_free(&compiler);
}
