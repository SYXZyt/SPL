#pragma once
#include "Node.h"
#include "../../Tokenisation/Token.h"

namespace SPL
{
	namespace Compiler
	{
		namespace Parser
		{
			namespace Nodes
			{
				class Single : public Node
				{
				public:
					int Size();

					std::string _ToString()
					{
						std::stringstream ss;
						ss << "[DEV(SINGLE)] - " << Size();
						return ss.str();
					}

					Single(Tokenisation::Token token) : Node(token) {}
				};
			}
		}
	}
}