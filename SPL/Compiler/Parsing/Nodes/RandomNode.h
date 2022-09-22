#pragma once
#include "Value.h"
#include "Single.h"
#include "../../Tokenisation/Token.h"

namespace SPL
{
	namespace Compiler
	{
		namespace Parser
		{
			namespace Nodes
			{
				class RandomNode final : public Node
				{
				private:
					Value* exclusiveMax;

				public:
					inline Value* GetMax()
					{
						return exclusiveMax;
					}

					int Size()
					{
						return static_cast<int>(sizeof(int)) + 1;
					}

					RandomNode(Value* exclusiveMax, Tokenisation::Token token) : Node(token)
					{
						this->exclusiveMax = exclusiveMax;
					}
				};
			}
		}
	}
}