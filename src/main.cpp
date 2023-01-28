#include "parser/parser.cpp"

int main() {
	Parser *parser = new Parser("main.exl");

	parser->parse();
}