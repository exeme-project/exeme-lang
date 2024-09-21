/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#include "./includes.c"

#include "./args/args.c"
#include "./compiler/compiler.c"
#include "./utils/map.c"

/*
 * Represents the config for parsing arguments.
 */
static const struct Array ARGUMENTS_FORMAT = array_new_stack(
    &arg_init(.name = "file", .description = "The path of the file to compile", .type = VARIABLE_TYPE_STRING, .position = 0),
    &arg_init(.name = "stdlib", .description = "The path to the folder containing the standard library",
              .def = "./../../lib", .flagShort = "-s", .flagLong = "--stdlib", .type = VARIABLE_TYPE_STRING));

int main(int argc, char **argv) {
    setlocale(LC_ALL, "");

    struct Args *args = args_new(argc, argv, ARGUMENTS_FORMAT);
    struct Map *parsed_args = args_parse(args);

    struct Compiler *compiler = compiler_new("../../programs/complex.exl");

    while (compiler_compile(compiler)) {
    }

    compiler_free(&compiler);
}
