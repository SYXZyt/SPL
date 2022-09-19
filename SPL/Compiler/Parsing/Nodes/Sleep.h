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
				class Sleep final : public Node
				{
				private:
					Value* delay;

				public:
					inline Value* GetDelay()
					{
						return delay;
					}

					int Size()
					{
						return static_cast<int>(sizeof(int)) + 1;
					}

					Sleep(Value* delay, Tokenisation::Token token) : Node(token)
					{
						this->delay = delay;
					}
				};
			}
		}
	}
}