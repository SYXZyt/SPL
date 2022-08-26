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
				class Mod final : public Single
				{
				public:
					Mod(Tokenisation::Token token) : Single(token) {}
				};
			}
		}
	}
}