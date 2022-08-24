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
				class Let final : public Node
				{
				private:
					Value* value;
					Tokenisation::Token name;

				public:
					Value* GetValue();
					Tokenisation::Token Name();

					int Size();

					std::string _ToString()
					{
						std::stringstream ss;
						ss << "[LET] - " << Size() << " <" << value->_ToString() << "> " << name.GetLexeme();
						return ss.str();
					}

					Let(const Let& l)
					{
						value = l.value;
						name = l.name;
					}

					Let(Tokenisation::Token token, Value* value, Tokenisation::Token name) : Node(token)
					{
						this->value = value;
						this->name = name;
					}

					~Let()
					{
						delete value;
						value = nullptr;
					}
				};
			}
		}
	}
}