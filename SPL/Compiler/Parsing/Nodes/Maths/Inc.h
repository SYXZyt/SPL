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
				class Inc final : public Single
				{
				public:
					Inc(Tokenisation::Token token) : Single(token) {}
				};
			}
		}
	}
}