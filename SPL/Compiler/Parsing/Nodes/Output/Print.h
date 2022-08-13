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
				class Print final : public Node
				{
				private:
					Value* toPrint;

				public:
					Value* ToPrint();
					int Size();

					std::string _ToString()
					{
						std::stringstream ss;
						ss << "[PRINT] - " << Size() << " <" << toPrint->_ToString() << '>';
						return ss.str();
					}

					Print(const Print& p)
					{
						toPrint = new Value(*p.toPrint);
						token = p.token;
					}
					Print(Tokenisation::Token token, Value* toPrint) : Node(token)
					{
						this->toPrint = toPrint;
					}
					~Print()
					{
						delete toPrint;
						toPrint = nullptr;
					}
				};
			}
		}
	}
}