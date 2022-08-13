#pragma once
#include <string>

namespace SPL
{
	namespace Compiler
	{
		namespace Tokenisation
		{
			/// <summary>
			/// Enum storing the different types of tokens
			/// </summary>
			enum class TokenType
			{
				IDENTIFIER,
				KEYWORD,
				MINUS,
				INT,
				FLOAT,
				STRING,
				COLON,
				BUILTIN,
				_NULL,
			};
		}
	}
}

static std::string TokenTypeToString(SPL::Compiler::Tokenisation::TokenType t)
{
	using SPL::Compiler::Tokenisation::TokenType;

	switch (t)
	{
		case TokenType::BUILTIN:
			return "(BUILTIN)";
		case TokenType::COLON:
			return "(COLON)";
		case TokenType::FLOAT:
			return "(FLOAT)";
		case TokenType::IDENTIFIER:
			return "(IDENTIFIER)";
		case TokenType::INT:
			return "(INT)";
		case TokenType::KEYWORD:
			return "(KEYWORD)";
		case TokenType::MINUS:
			return "(MINUS)";
		case TokenType::STRING:
			return "(STRING)";
		default:
			return "(UNKNOWN)";
	}
}