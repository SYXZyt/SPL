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
				class Mul final : public Single
				{
				public:
					std::string _ToString()
					{
						std::stringstream ss;
						ss << "[MUL] - " << Size();
						return ss.str();
					}

					Mul(Tokenisation::Token token) : Single(token) {}
				};
			}
		}
	}
}