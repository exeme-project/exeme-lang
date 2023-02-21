// Part of the Exeme Language Project, under the MIT license. See '/LICENSE' for
// license information. SPDX-License-Identifier: MIT License.

#include "compiler/compiler.cpp"

#pragma pack(1)

int main() {
	auto compiler =
		new Compiler(OS == 1 ? "../../src/main.exl" : "../src/main.exl");

	compiler->compile();
}
