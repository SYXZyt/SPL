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
				class Push final : public Node
				{
				private:
					Value* value;

				public:
					Value* GetValue();
					int Size();

					std::string _ToString()
					{
						std::stringstream ss;
						ss << "[PUSH] - " << Size() << " <" << value->_ToString() << '>';
						return ss.str();
					}

					Push(const Push& v)
					{
						value = new Value(*v.value);
						token = v.token;
					}
					Push(Tokenisation::Token token, Value* value) : Node(token)
					{
						this->value = value;
					}
					~Push()
					{
						delete value;
						value = nullptr;
					}
				};
			}
		}
	}
}