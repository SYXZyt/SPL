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
				namespace Casting
				{
					/// <summary>
					/// Handle a string cast node
					/// </summary>
					class ToString final : public Single
					{
					public:
						std::string NodeType()
						{
							return "TOSTRING";
						}

						std::string _ToString()
						{
							std::stringstream ss;
							ss << "[TOSTRING] - " << Size();
							return ss.str();
						}

						/// <summary>
						/// Create a new 'ToString' node
						/// </summary>
						/// <param name="token">The token that made this node</param>
						ToString(Tokenisation::Token token) : Single(token) {}
					};
				}
			}
		}
	}
}