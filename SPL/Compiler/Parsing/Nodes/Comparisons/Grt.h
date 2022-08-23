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
				class Grt final : public Comparison
				{
				public:
					std::string _ToString()
					{
						std::stringstream ss;
						ss << "[GRT] - " << Size();
						return ss.str();
					}

					Grt(Tokenisation::Token token, Value* line) : Comparison(token, line) {}
				};
			}
		}
	}
}