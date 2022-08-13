#include "Exit.h"

int SPL::Compiler::Parser::Nodes::Exit::Size()
{
    return 1 + value->Size();
}

SPL::Compiler::Parser::Nodes::Value* SPL::Compiler::Parser::Nodes::Exit::GetValue()
{
    return value;
}
