#include "SetPop.h"

SPL::Compiler::Parser::Nodes::Value* SPL::Compiler::Parser::Nodes::SetPop::Name()
{
    return name;
}

int SPL::Compiler::Parser::Nodes::SetPop::Size()
{
	int size = 1;
	size += name->Size();
	return size;
}