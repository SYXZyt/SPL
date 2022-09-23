#pragma once
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
				class Raise final : public Node
				{
				private:
					Value* errorMessage;

				public:
					inline Value* GetErrorMessage()
					{
						return errorMessage;
					}

					int Size()
					{
						return errorMessage->Size() + 1;
					}

					Raise(Value* errorMessage, Tokenisation::Token token) : Node(token)
					{
						this->errorMessage = errorMessage;
					}
				}; 
			}
		}
	}
}