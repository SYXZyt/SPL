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
				class Goto final : public Node
				{
				private:
					Value* line;

				public:
					int Size();
					Value* Line();

					std::string _ToString()
					{
						std::stringstream ss;
						ss << "[GOTO] - " << Size() << " <" << Line()->_ToString() << '>';
						return ss.str();
					}

					Goto(const Goto& g)
					{
						line = new Value(*g.line);
						token = g.token;
					}
					Goto(Tokenisation::Token token, Value* line) : Node(token)
					{
						this->line = line;
					}
					~Goto()
					{
						delete line;
						line = nullptr;
					}
				};
			}
		}
	}
}