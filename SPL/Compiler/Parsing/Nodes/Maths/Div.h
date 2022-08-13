#pragma once
#include "../Single.h"
#include "../../../Tokenisation/Token.h"

namespace SPL
{
	namespace Compiler
	{
		namespace Parser
		{
			namespace Nodes
			{
				class Div final : public Single
				{
				public:
					std::string _ToString()
					{
						std::stringstream ss;
						ss << "[DIV] - " << Size();
						return ss.str();
					}

					Div(Tokenisation::Token token) : Single(token) {}
				};
			}
		}
	}
}