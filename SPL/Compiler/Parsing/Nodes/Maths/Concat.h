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
				class Concat final : public Single
				{
				public:
					std::string _ToString()
					{
						std::stringstream ss;
						ss << "[CONCAT] - " << Size();
						return ss.str();
					}

					Concat(Tokenisation::Token token) : Single(token) {}
				};
			}
		}
	}
}