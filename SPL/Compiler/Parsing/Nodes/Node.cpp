#include "Node.h"

int SPL::Compiler::Parser::Nodes::Node::Size()
{
	return 0;
}

std::string SPL::Compiler::Parser::Nodes::Node::_ToString()
{
	std::stringstream ss;
	ss << "[NODE] - " << Size() << " {" << Token() << '}';

	return ss.str();
}

SPL::Compiler::Parser::Nodes::Node::Node()
{
	token = Token();
}

SPL::Compiler::Parser::Nodes::Node::Node(Tokenisation::Token token)
{
	this->token = token;
}