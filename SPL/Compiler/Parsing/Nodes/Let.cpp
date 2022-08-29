#include "Let.h"

SPL::Compiler::Parser::Nodes::Value* SPL::Compiler::Parser::Nodes::Let::GetValue()
{
	return value;
}

SPL::Compiler::Tokenisation::Token SPL::Compiler::Parser::Nodes::Let::Name()
{
	return name;
}

int SPL::Compiler::Parser::Nodes::Let::Size()
{
	int size = 1;
	size += value->Size();
	size += 4; //Name index takes 4 bytes
	return size;
}