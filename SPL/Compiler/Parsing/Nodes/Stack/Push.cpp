#include "Push.h"

SPL::Compiler::Parser::Nodes::Value* SPL::Compiler::Parser::Nodes::Push::GetValue()
{
    return value;
}

int SPL::Compiler::Parser::Nodes::Push::Size()
{
    return 1 + value->Size();
}
