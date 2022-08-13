#pragma once
#include <ostream>

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
				namespace Casting
				{
					/// <summary>
					/// Handle an int cast node
					/// </summary>
					class ToInt final : public Single
					{
					public:
						std::string NodeType()
						{
							return "TOINT";
						}

						std::string _ToString()
						{
							std::stringstream ss;
							ss << "[TOINT] - " << Size();
							return ss.str();
						}

						/// <summary>
						/// Create a new 'ToInt' node
						/// </summary>
						/// <param name="token">The token that made this node</param>
						ToInt(Tokenisation::Token token) : Single(token) {}
					};
				}
			}
		}
	}
}