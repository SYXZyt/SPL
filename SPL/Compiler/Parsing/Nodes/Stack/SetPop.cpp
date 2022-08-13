#include "SetPop.h"

SPL::Compiler::Tokenisation::Token SPL::Compiler::Parser::Nodes::SetPop::Name()
{
    return name;
}

bool SPL::Compiler::Parser::Nodes::SetPop::IsMutable()
{
    return isMutable;
}

int SPL::Compiler::Parser::Nodes::SetPop::Size()
{
    return static_cast<int>(1 + name.GetLexeme().length());
}