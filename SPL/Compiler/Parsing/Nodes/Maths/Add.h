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
				class Add final : public Single
				{
				public:
					std::string _ToString()
					{
						std::stringstream ss;
						ss << "[ADD] - " << Size();
						return ss.str();
					}

					Add(Tokenisation::Token token) : Single(token) {}
				};
			}
		}
	}
}