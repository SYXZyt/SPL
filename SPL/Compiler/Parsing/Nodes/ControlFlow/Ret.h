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
				class Ret final : public Single
				{
				public:
					std::string _ToString()
					{
						std::stringstream ss;
						ss << "[RET] - " << Size();
						return ss.str();
					}

					Ret(Tokenisation::Token token) : Single(token) {}
				};
			}
		}
	}
}