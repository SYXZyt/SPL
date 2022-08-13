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
				class Pow final : public Single
				{
				public:
					std::string _ToString()
					{
						std::stringstream ss;
						ss << "[POW] - " << Size();
						return ss.str();
					}

					Pow(Tokenisation::Token token) : Single(token) {}
				};
			}
		}
	}
}