#include "Print.h"

SPL::Compiler::Parser::Nodes::Value* SPL::Compiler::Parser::Nodes::Print::ToPrint()
{
    return toPrint;
}

int SPL::Compiler::Parser::Nodes::Print::Size()
{
    return 1 + toPrint->Size();
}
