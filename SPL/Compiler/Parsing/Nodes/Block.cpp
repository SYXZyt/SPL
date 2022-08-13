#include "Block.h"

int SPL::Compiler::Parser::Nodes::Block::Size()
{
    int i = 0;
    for (Node* node : nodes) i += node->Size();
    return i;
}

void SPL::Compiler::Parser::Nodes::Block::AddNode(Node* node)
{
    nodes.push_back(node);
}