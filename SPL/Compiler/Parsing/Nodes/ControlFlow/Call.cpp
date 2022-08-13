#include "Call.h"

int SPL::Compiler::Parser::Nodes::Call::Size()
{
	return 1 + sizeof(int);
}

SPL::Compiler::Parser::Nodes::Value* SPL::Compiler::Parser::Nodes::Call::Line()
{
	return line;
}