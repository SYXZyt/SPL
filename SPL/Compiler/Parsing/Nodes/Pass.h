#pragma once
#include "Single.h"
#include "../../Tokenisation/Token.h"

namespace SPL
{
	namespace Compiler
	{
		namespace Parser
		{
			namespace Nodes
			{
				class Pass final : public Single
				{
				public:
					std::string _ToString()
					{
						std::stringstream ss;
						ss << "[PASS] - " << Size();
						return ss.str();
					}

					Pass(Tokenisation::Token token) : Single(token) {}
				};
			}
		}
	}
}