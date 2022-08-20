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
				_NULL,

				//Conditions
				EQUALS,
				GREATER,
				GREATER_EQ,
				LOWER,
				LOWER_EQ,
				NEQ,
			};
		}
	}
}

static std::string TokenTypeToString(SPL::Compiler::Tokenisation::TokenType t)
{
	using SPL::Compiler::Tokenisation::TokenType;

	switch (t)
	{
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
		case TokenType::_NULL:
			return "(NULL)";
		default:
			return "(UNKNOWN)";
	}
}