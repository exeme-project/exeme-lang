// Part of the Exeme Language Project, under the MIT license. See '/LICENSE' for
// license information. SPDX-License-Identifier: MIT License.

#pragma once

#include "../ast/ast.cpp"

class Compiler {
  private:
	void compileNext(struct AstToken *token) {
		if (token != nullptr) {
			std::cout << "Compiler received token '"
					  << AstTokenNames[static_cast<size_t>(token->identifier)]
					  << "'\n\n";
		} else {
			std::cout << "NotImplemented: Parsing of the received token is not "
						 "implemented yet\n\n";
		}
	}

  public:
	Ast *ast;
	std::string assembly;

	Compiler(std::string filePath) {
		this->ast = new Ast(filePath);
		this->assembly = "";
	}

	void compile() {
		while (this->ast->parse()) {
			this->compileNext(this->ast->token);

			delete this->ast->token;
			this->ast->token = nullptr;
		}
	}
};
