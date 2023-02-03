// Part of the Exeme language project, under the MIT license. See '/LICENSE' for
// license information. SPDX-License-Identifier: MIT License.

#include "parser/parser.cpp"

int main() {
	Parser *parser = new Parser("../src/main.exl");

	parser->parse();

	delete parser->lexer;
	delete parser;
}