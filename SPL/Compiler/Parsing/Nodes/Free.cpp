#include "Free.h"

int SPL::Compiler::Parser::Nodes::Free::Size()
{
    return 5;
}

SPL::Compiler::Tokenisation::Token SPL::Compiler::Parser::Nodes::Free::VariableName()
{
    return variableName;
}
