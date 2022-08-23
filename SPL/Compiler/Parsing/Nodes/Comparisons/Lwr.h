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
				class Lwr final : public Comparison
				{
				public:
					std::string _ToString()
					{
						std::stringstream ss;
						ss << "[LWR] - " << Size();
						return ss.str();
					}

					Lwr(Tokenisation::Token token, Value* line) : Comparison(token, line) {}
				};
			}
		}
	}
}