#pragma once
#include <vector>

#include "Block.h"
#include "../Node.h"
#include "../Condition.h"
#include "../../../Tokenisation/Token.h"

namespace SPL
{
	namespace Compiler
	{
		namespace Parser
		{
			namespace Nodes
			{
				class If final : public Node
				{
				private:
					Condition* condition;
					Block* block;
					If* elseNode;

				public:
					Condition* GetCondition()
					{
						return condition;
					}

					Block* GetBlock()
					{
						return block;
					}

					If* ElseNode()
					{
						return elseNode;
					}

					int Size()
					{
						int size = 6;
						size += condition->Size();
						if (elseNode != nullptr) size += elseNode->Size() + 4;
						return size;
					}

					If() : Node()
					{
						condition = nullptr;
						block = nullptr;
						elseNode = nullptr;
					}

					If(Tokenisation::Token token, Condition* condition, Block* block) : Node(token)
					{
						this->block = block;
						this->condition = condition;
						elseNode = nullptr;
					}

					If(Tokenisation::Token token, Condition* condition, Block* block, If* elseNode) : Node(token)
					{
						this->block = block;
						this->condition = condition;
						this->elseNode = elseNode;
					}
				};
			}
		}
	}
}