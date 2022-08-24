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
				class Constant final : public Node
				{
				private:
					Value* value;

				public:
					Value* GetValue()
					{
						return value;
					}

					std::string _ToString()
					{
						std::stringstream ss;
						ss << "[CONST] - " << value;
						return ss.str();
					}

					Constant(Tokenisation::Token token, Value* value) : Node(token)
					{
						this->value = value;
					}
				};
			}
		}
	}
}