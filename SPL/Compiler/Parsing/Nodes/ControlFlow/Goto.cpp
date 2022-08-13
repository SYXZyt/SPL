#include "Goto.h"

int SPL::Compiler::Parser::Nodes::Goto::Size()
{
    return 1 + sizeof(int);
}

SPL::Compiler::Parser::Nodes::Value* SPL::Compiler::Parser::Nodes::Goto::Line()
{
    return line;
}