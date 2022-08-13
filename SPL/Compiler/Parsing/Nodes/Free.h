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
				class Free final : public Node
				{
				private:
					Tokenisation::Token variableName;

				public:
					int Size();

					std::string _ToString()
					{
						std::stringstream ss;
						ss << "[FREE] - " << Size() << " { " << Escape(variableName.GetValueString()) << " }";
						return ss.str();
					}

					Tokenisation::Token VariableName();

					Free(Tokenisation::Token token, Tokenisation::Token variableName) : Node(token)
					{
						this->variableName = variableName;
					}
				};
			}
		}
	}
}