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
					Tokenisation::Token name;

				public:
					Value* GetValue()
					{
						return value;
					}

					Tokenisation::Token GetName()
					{
						return name;
					}

					std::string _ToString()
					{
						std::stringstream ss;
						ss << "[CONST] - " << value->_ToString();
						return ss.str();
					}

					Constant()
					{
						value = nullptr;
					}
					Constant(Tokenisation::Token token, Tokenisation::Token name, Value* value) : Node(token)
					{
						this->value = value;
						this->name = name;
					}
					Constant(const Constant& c)
					{
						this->value = c.value;
						this->name = c.name;
					}
					~Constant()
					{
						delete value;
					}
				};
			}
		}
	}
}