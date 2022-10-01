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
				class IsFloat final : public Single
				{
				public:
					IsFloat(Tokenisation::Token token) : Single(token) {}
				};
			}
		}
	}
}