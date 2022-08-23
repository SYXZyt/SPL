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
				class Comparison : public Node
				{
				private:
					Value* line;

				public:
					std::string _ToString()
					{
						std::stringstream ss;
						ss << "[CMP] - " << Size();
						return ss.str();
					}

					Value* Line()
					{
						return line;
					}

					int Size()
					{
						return 1 + line->Size();
					}

					Comparison(Tokenisation::Token token, Value* line) : Node(token)
					{
						this->line = line;
					}
				};
			}
		}
	}
}