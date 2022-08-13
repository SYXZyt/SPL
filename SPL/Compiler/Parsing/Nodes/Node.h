#pragma once
#include <sstream>

#include "../../Tokenisation/Token.h"

namespace SPL
{
	namespace Compiler
	{
		namespace Parser
		{
			namespace Nodes
			{
				class Node
				{
				protected:
					Tokenisation::Token token;

				public:
					inline Tokenisation::Token Token()
					{
						return token;
					}

					virtual int Size();
					virtual std::string _ToString();

					Node();
					Node(Tokenisation::Token token);
					virtual ~Node() {}
				};

				class NullNode final : public Node
				{
					std::string NodeType()
					{
						return "NULL";
					}
				};
			}
		}
	}
}