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
				class Equ final : public Comparison
				{
				public:
					std::string _ToString()
					{
						std::stringstream ss;
						ss << "[EQU] - " << Size();
						return ss.str();
					}

					Equ(Tokenisation::Token token, Value* line) : Comparison(token, line) {}
				};
			}
		}
	}
}