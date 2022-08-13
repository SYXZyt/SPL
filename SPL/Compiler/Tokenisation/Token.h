#pragma once
#include <string>
#include <limits>
#include <ostream>

#include "TokenType.h"
#include "../../../String.h"
#include "../../../Vector2.h"

namespace SPL
{
	namespace Compiler
	{
		namespace Tokenisation
		{
			/// <summary>
			/// Class responsible for storing the basic form of codee
			/// </summary>
			class Token final
			{
			private:
				std::string lexeme;
				Vector2 pos;
				std::string inputName;
				TokenType type;

#pragma region Data
				std::string str;
				float flt;
				int integer;
#pragma endregion

			public:
				/// <summary>
				/// Get the lexeme of the token
				/// </summary>
				/// <returns>The lexeme</returns>
				inline std::string GetLexeme()
				{
					return lexeme;
				}

				/// <summary>
				/// Get the position of the token
				/// </summary>
				/// <returns>The position</returns>
				inline Vector2 GetPosition()
				{
					return pos;
				}

				/// <summary>
				/// Get the input stream name of the token
				/// </summary>
				/// <returns>The input stream name</returns>
				inline std::string GetInputName()
				{
					return inputName;
				}

				/// <summary>
				/// Get the token type of the token
				/// </summary>
				/// <returns>The token type</returns>
				inline TokenType GetTokenType()
				{
					return type;
				}

				/// <summary>
				/// Get the string value of the token
				/// </summary>
				/// <returns>The value</returns>
				inline std::string GetValueString()
				{
					return str;
				}

				/// <summary>
				/// Get the float value of the token
				/// </summary>
				/// <returns>The value</returns>
				inline float GetValueFloat()
				{
					return flt;
				}

				/// <summary>
				/// Get the int value of the token
				/// </summary>
				/// <returns>The value</returns>
				inline int GetValueInt()
				{
					return integer;
				}

				/// <summary>
				/// Set a new string value for the token
				/// </summary>
				/// <param name="str">The value to set</param>
				void SetValueString(std::string str);

				/// <summary>
				/// Set a new float value for the token
				/// </summary>
				/// <param name="flt">The value to set</param>
				void SetValueFloat(float flt);

				/// <summary>
				/// Set a new int value for the token
				/// </summary>
				/// <param name="integer">The value to set</param>
				void SetValueInt(int integer);

				/// <summary>
				/// Update the lexeme for a token
				/// </summary>
				/// <param name="newLexeme">The new updated lexeme</param>
				void SetLexeme(std::string newLexeme);

				/// <summary>
				/// Create a new token
				/// </summary>
				Token();

				/// <summary>
				/// Create a copy of a token
				/// </summary>
				/// <param name="t">Token to copy</param>
				Token(const Token& t);

				/// <summary>
				/// Create a new token
				/// </summary>
				/// <param name="lexeme">Lexeme of the token</param>
				/// <param name="type">Type of the token</param>
				/// <param name="pos">Position of the token</param>
				/// <param name="inputName">Input stream name of the token</param>
				Token(std::string lexeme, TokenType type, Vector2 pos, std::string inputName);
			};
		}
	}
}

static std::ostream& operator<<(std::ostream& os, SPL::Compiler::Tokenisation::Token t)
{
	using SPL::Compiler::Tokenisation::TokenType;

	os << "[TOKEN-" << t.GetInputName() << "] ";
	os << '\'' << Escape(t.GetLexeme()) << "' ";
	os << TokenTypeToString(t.GetTokenType()) << ' ';

	if (t.GetTokenType() == TokenType::FLOAT)
	{
		os << "Raw Value: <";
		os << t.GetValueFloat();
		os << ">";
	}
	else if (t.GetTokenType() == TokenType::INT)
	{
		os << "Raw Value: <";
		os << t.GetValueInt();
		os << ">";
	}
	else if (t.GetTokenType() == TokenType::STRING)
	{
		os << "Raw Value: <";
		os << Escape(t.GetValueString());
		os << ">";
	}
	return os;
}