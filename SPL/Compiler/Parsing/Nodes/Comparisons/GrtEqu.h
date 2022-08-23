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
				class GrtEqu final : public Comparison
				{
				public:
					std::string _ToString()
					{
						std::stringstream ss;
						ss << "[GRTEQU] - " << Size();
						return ss.str();
					}

					GrtEqu(Tokenisation::Token token, Value* line) : Comparison(token, line) {}
				};
			}
		}
	}
}