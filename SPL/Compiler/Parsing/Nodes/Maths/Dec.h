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
				class Dec final : public Single
				{
				public:
					Dec(Tokenisation::Token token) : Single(token) {}
				};
			}
		}
	}
}