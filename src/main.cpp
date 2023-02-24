// Part of the Exeme Language Project, under the MIT license. See '/LICENSE' for
// license information. SPDX-License-Identifier: MIT License.

#include "parser/parser.cpp"

#pragma pack(1)

int main() {
	auto parser = new Parser(OS == 1 ? "../../src/programs/complex.exl"
									 : "../src/programs/complex.exl");

	while (parser->parse()) {
	}
}
