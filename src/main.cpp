// Part of the Exeme Language Project, under the MIT license. See '/LICENSE' for
// license information. SPDX-License-Identifier: MIT License.

#include "ast/ast.cpp"

#pragma pack(1)

int main() {
	Ast *ast = new Ast("../src/main.exl");

	ast->parse();

	ast->lexer->clearTokens();
	delete ast->lexer;
	delete ast;
}