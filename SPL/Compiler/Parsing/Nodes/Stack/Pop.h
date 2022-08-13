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
				class Pop final : public Single
				{
				public:
					std::string _ToString()
					{
						std::stringstream ss;
						ss << "[POP] - " << Size();
						return ss.str();
					}

					Pop(Tokenisation::Token token) : Single(token) {}
				};
			}
		}
	}
}