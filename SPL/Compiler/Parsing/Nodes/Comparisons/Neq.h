#pragma once
#include "../Value.h"
#include "Comparison.h"
#include "../../../Tokenisation/Token.h"

namespace SPL
{
	namespace Compiler
	{
		namespace Parser
		{
			namespace Nodes
			{
				class Neq final : public Comparison
				{
				public:
					std::string _ToString()
					{
						std::stringstream ss;
						ss << "[NEQ] - " << Size();
						return ss.str();
					}

					Neq(Tokenisation::Token token, Value* line) : Comparison(token, line) {}
				};
			}
		}
	}
}