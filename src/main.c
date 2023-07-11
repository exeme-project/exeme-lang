/**
 * Part of the Exeme Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#include "./compiler/compiler.c"
#include "./includes.c"


#pragma pack(1)

int main(void) {
	struct Parser *parser = parser_new("../../programs/test.exl");

	while (parser_parse(parser, true)) {
	}

	parser_free(parser);
}
