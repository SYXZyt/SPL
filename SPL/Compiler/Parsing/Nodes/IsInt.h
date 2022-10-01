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
				class IsInt final : public Single
				{
				public:
					IsInt(Tokenisation::Token token) : Single(token) {}
				};
			}
		}
	}
}