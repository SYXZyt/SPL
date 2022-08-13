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
					/// Handle a float cast node
					/// </summary>
					class ToFloat final : public Single
					{
					public:
						std::string NodeType()
						{
							return "TOFLOAT";
						}

						std::string _ToString()
						{
							std::stringstream ss;
							ss << "[TOFLOAT] - " << Size();
							return ss.str();
						}

						/// <summary>
						/// Create a new 'ToFloat' node
						/// </summary>
						/// <param name="token">The token that made this node</param>
						ToFloat(Tokenisation::Token token) : Single(token) {}
					};
				}
			}
		}
	}
}