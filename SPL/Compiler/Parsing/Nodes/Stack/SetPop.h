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
				class SetPop final : public Node
				{
				private:
					Value* name;

				public:
					Value* Name();

					int Size();

					std::string _ToString()
					{
						std::stringstream ss;
						ss << "[SETPOP] - " << Size() << " <" << name->_ToString() << '>';
						return ss.str();
					}

					SetPop(Tokenisation::Token token, Value* name) : Node(token)
					{
						this->name = name;
					}
				};
			}
		}
	}
}