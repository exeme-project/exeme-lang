/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#include "./includes.c"

#include "./args/args.c"
#include "./compiler/compiler.c"

#pragma pack(1)

int main(int argc, char **argv) {
    struct Args *args = NULL;
    struct Compiler *compiler = NULL;

    setlocale(LC_ALL, "");

    args = args_new(argc, argv);

    compiler = compiler_new("../../programs/test.exl");

    while (compiler_compile(compiler)) {
    }

    compiler_free(&compiler);
}
