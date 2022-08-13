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
					bool isMutable;

				public:
					Tokenisation::Token Name();
					bool IsMutable();

					int Size();

					std::string _ToString()
					{
						std::stringstream ss;
						ss << "[SETPOP] - " << Size() << "MUT-" << (isMutable ? "1" : "0") << " <" << name.GetLexeme() << '>';
						return ss.str();
					}

					SetPop(Tokenisation::Token token, bool isMutable, Tokenisation::Token name) : Node(token)
					{
						this->isMutable = isMutable;
						this->name = name;
					}
				};
			}
		}
	}
}