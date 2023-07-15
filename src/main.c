/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#include "./compiler/compiler.c"
#include "./includes.c"

#pragma pack(1)

int main(void) {
	struct Compiler *compiler = compiler_new("../../programs/test.exl");

	while (compiler_compile(compiler)) {
	}

	compiler_free(&compiler);
}
