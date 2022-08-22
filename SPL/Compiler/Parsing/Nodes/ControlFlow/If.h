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
					Block* elseNode;

				public:
					Condition* GetCondition()
					{
						return condition;
					}

					Block* GetBlock()
					{
						return block;
					}

					Block* ElseNode()
					{
						return elseNode;
					}

					std::string _ToString()
					{
						std::stringstream ss;
						ss << "[IF] - " << Size() << "(" << condition->_ToString() << ") (" << block->_ToString() << ") (" << (elseNode == nullptr ? "ELSE_NULL" : elseNode->_ToString()) << ")";
						return ss.str();
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

					If(Tokenisation::Token token, Condition* condition, Block* block, Block* elseNode) : Node(token)
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