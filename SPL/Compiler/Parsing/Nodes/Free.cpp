#include "Free.h"

int SPL::Compiler::Parser::Nodes::Free::Size()
{
    return static_cast<int>(1 + variableName.GetValueString().length());
}

SPL::Compiler::Tokenisation::Token SPL::Compiler::Parser::Nodes::Free::VariableName()
{
    return variableName;
}
