#pragma once
#include "../Node.h"
#include "../Value.h"
#include "../../../Tokenisation/Token.h"

namespace SPL
{
	namespace Compiler
	{
		namespace Parser
		{
			namespace Nodes
			{
				class Exit final : public Node
				{
				private:
					Value* value;

				public:
					int Size();
					Value* GetValue();

					std::string _ToString()
					{
						std::stringstream ss;
						ss << "[EXIT] - " << Size() << " <" << value->_ToString() << '>';
						return ss.str();
					}

					Exit(const Exit& e)
					{
						value = new Value(*e.value);
						token = e.token;
					}
					Exit(Tokenisation::Token token, Value* value) : Node(token)
					{
						this->value = value;
					}
					~Exit()
					{
						delete value;
						value = nullptr;
					}
				};
			}
		}
	}
}