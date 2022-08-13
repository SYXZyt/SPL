#pragma once
#include <vector>

#include "Node.h"
#include "../../Tokenisation/Token.h"

namespace SPL
{
	namespace Compiler
	{
		namespace Parser
		{
			namespace Nodes
			{
				class Block final : public Node
				{
				private:
					std::vector<Node*> nodes;

				public:
					int Size();

					void AddNode(Node* node);

					std::string _ToString()
					{
						std::stringstream ss;
						ss << "[BLOCK] - " << Size();
						for (Node* n : nodes)
						{
							ss << "\n" << n->_ToString();
						} ss << "\b[BLOCK_END]";
						return ss.str();
					}

					Block(Tokenisation::Token token) : Node(token)
					{
						nodes = std::vector<Node*>();
					}
				};
			}
		}
	}
}