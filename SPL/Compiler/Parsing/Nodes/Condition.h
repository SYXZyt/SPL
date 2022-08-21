#pragma once
#include "Node.h"
#include "Value.h"
#include "../../Tokenisation/Token.h"

namespace SPL
{
	namespace Compiler
	{
		namespace Parser
		{
			namespace Nodes
			{
				class Condition final : public Node
				{
				private:
					Value* lhs;
					Tokenisation::Token op;
					Value* rhs;

				public:
					Value* GetLhs()
					{
						return lhs;
					}

					Tokenisation::Token GetOp()
					{
						return op;
					}

					Value* GetRhs()
					{
						return rhs;
					}

					int Size()
					{
						return (1 + lhs->Size()) + (1 + rhs->Size()); //The conditional will simplify to two push instructions
					}

					Condition(Value* lhs, Tokenisation::Token op, Value* rhs) : Node(op)
					{
						this->lhs = lhs;
						this->rhs = rhs;
						this->op = op;
					}
				};
			}
		}
	}
}