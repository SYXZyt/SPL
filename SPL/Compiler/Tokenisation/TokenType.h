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

				L_CURLY,
				R_CURLY,
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
		case TokenType::EQUALS:
			return "(==)";
		case TokenType::NEQ:
			return "(!=)";
		case TokenType::GREATER:
			return "(>)";
		case TokenType::GREATER_EQ:
			return "(>=)";
		case TokenType::LOWER:
			return "(<)";
		case TokenType::LOWER_EQ:
			return "(<=)";
		default:
			return "(UNKNOWN)";
	}
}