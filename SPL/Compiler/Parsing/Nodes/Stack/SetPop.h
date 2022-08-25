#pragma once
#include "../Node.h"
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
					Tokenisation::Token name;

				public:
					Tokenisation::Token Name();

					int Size();

					std::string _ToString()
					{
						std::stringstream ss;
						ss << "[SETPOP] - " << Size() << " <" << name.GetValueString() << '>';
						return ss.str();
					}

					SetPop(Tokenisation::Token token, Tokenisation::Token name) : Node(token)
					{
						this->name = name;
					}
				};
			}
		}
	}
}