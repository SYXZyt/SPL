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
				class LwrEqu final : public Comparison
				{
				public:
					std::string _ToString()
					{
						std::stringstream ss;
						ss << "[LWREQU] - " << Size();
						return ss.str();
					}

					LwrEqu(Tokenisation::Token token, Value* line) : Comparison(token, line) {}
				};
			}
		}
	}
}