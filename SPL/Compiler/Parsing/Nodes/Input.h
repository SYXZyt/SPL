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
				class Input final : public Single
				{
				public:
					Input(Tokenisation::Token token) : Single(token) {}
				};
			}
		}
	}
}