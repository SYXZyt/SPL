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
				class Sub final : public Single
				{
				public:
					std::string _ToString()
					{
						std::stringstream ss;
						ss << "[SUB] - " << Size();
						return ss.str();
					}

					Sub(Tokenisation::Token token) : Single(token) {}
				};
			}
		}
	}
}