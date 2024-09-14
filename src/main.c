/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#include "./includes.c"

#include "./args/args.c"
#include "./compiler/compiler.c"

int main(int argc, char **argv) {
    struct Args *args = NULL;
    struct Compiler *compiler = NULL;

    setlocale(LC_ALL, "");

    args = args_new(argc, argv);
    args_parse(args);

    compiler = compiler_new("../../programs/complex.exl");

    while (compiler_compile(compiler)) {
    }

    compiler_free(&compiler);
}
