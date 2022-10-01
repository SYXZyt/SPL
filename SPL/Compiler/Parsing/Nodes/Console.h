#pragma once
#include "Value.h"
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
				class Console final : public Single
				{
				private:
					Value* consoleOp;

				public:
					inline Value* GetConsoleOp()
					{
						return consoleOp;
					}

					Console(Value* consoleOp, Tokenisation::Token token) : Single(token)
					{
						this->consoleOp = consoleOp;
					}
				};
			}
		}
	}
}
