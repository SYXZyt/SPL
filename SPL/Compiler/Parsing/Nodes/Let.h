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
					bool isMutable;

				public:
					Value* GetValue();
					Tokenisation::Token Name();
					bool IsMutable();

					int Size();

					std::string _ToString()
					{
						std::stringstream ss;
						ss << "[LET] - " << Size() << " MUT-" << (isMutable ? "1" : "0") << " <" << value->_ToString() << "> " << name.GetLexeme();
						return ss.str();
					}

					Let(const Let& l)
					{
						value = l.value;
						name = l.name;
						isMutable = l.isMutable;
					}

					Let(Tokenisation::Token token, Value* value, Tokenisation::Token name, bool isMutable) : Node(token)
					{
						this->value = value;
						this->name = name;
						this->isMutable = isMutable;
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