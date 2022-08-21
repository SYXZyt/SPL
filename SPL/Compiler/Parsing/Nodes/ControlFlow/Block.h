#pragma once
#include <vector>

#include "../Node.h"
#include "../../../Tokenisation/Token.h"

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
					std::vector<Node*> blockNodes;

				public:
					void AddNode(Node* n)
					{
						blockNodes.push_back(n);
					}

					const std::vector<Node*> GetNodes()
					{
						return blockNodes;
					}

					int Count()
					{
						return blockNodes.size();
					}

					int Size()
					{
						int size = 0;
						for (Node* n : blockNodes)
						{
							size += n->Size();
						}

						return size;
					}

					Block() : Node() {}
					Block(Tokenisation::Token token) : Node(token)
					{
						blockNodes = std::vector<Node*>();
					}
				};
			}
		}
	}
}