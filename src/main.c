/**
 * Part of the Exeme Language Project, under the MIT license. See '/LICENSE' for
 * license information. SPDX-License-Identifier: MIT License.
 */

#include "./includes.c"
#include "./parser/parser.c"

#pragma pack(1)

int main(void) {
	struct Parser *parser = parser_new("../../programs/main.exl");

	while (parser_parse(parser)) {
	}

	parser_free(parser);
}
