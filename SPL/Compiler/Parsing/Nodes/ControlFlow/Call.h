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
				class Call final : public Node
				{
				private:
					Value* line;

				public:
					int Size();
					Value* Line();

					std::string _ToString()
					{
						std::stringstream ss;
						ss << "[CALL] - " << Size() << " <" << Line()->_ToString() << '>';
						return ss.str();
					}

					Call(const Call& c)
					{
						line = new Value(*c.line);
						token = c.token;
					}

					Call(Tokenisation::Token token, Value* line) : Node(token)
					{
						this->line = line;
					}

					~Call()
					{
						delete line;
						line = nullptr;
					}
				};
			}
		}
	}
}