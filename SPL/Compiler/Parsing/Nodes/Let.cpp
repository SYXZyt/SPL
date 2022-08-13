#include "Let.h"

SPL::Compiler::Parser::Nodes::Value* SPL::Compiler::Parser::Nodes::Let::GetValue()
{
	return value;
}

SPL::Compiler::Tokenisation::Token SPL::Compiler::Parser::Nodes::Let::Name()
{
	return name;
}

bool SPL::Compiler::Parser::Nodes::Let::IsMutable()
{
	return isMutable;
}

int SPL::Compiler::Parser::Nodes::Let::Size()
{
	int size = 1;
	size += value->Size();
	size += static_cast<int>(name.GetValueString().length());
	return size;
}