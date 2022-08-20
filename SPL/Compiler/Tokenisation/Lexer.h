#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <iostream>

#include "Token.h"
#include "TokenType.h"
#include "../../../String.h"
#include "../../../Vector2.h"
#include "../../../Constants.h"
#include "../../../SPLErrors.h"
#include "../../../ErrorStream.h"

namespace SPL
{
	namespace Compiler
	{
		namespace Tokenisation
		{
			/// <summary>
			/// Class which will convert a series of characters, into their token counterparts
			/// </summary>
			class Lexer final
			{
			private:
				std::string inputName;

				int pos;
				int posOnLine;
				int line;

				std::string input;

				char currentChar;

				std::vector<std::string> keywords;

				/// <summary>
				/// Advance the token pointer
				/// </summary>
				void Advance();

				char Peek();

				/// <summary>
				/// Generate a vector at the current position in the input text
				/// </summary>
				/// <returns>Generated vector</returns>
				Vector2 Pos();

				/// <summary>
				/// Generate an identifier or keyword using the current character as a starting point
				/// </summary>
				/// <returns>Generated token</returns>
				Token BuildIdentifier();

				/// <summary>
				/// Generate a string using the current character as a starting point
				/// </summary>
				/// <returns>Generated token</returns>
				Token BuildString();

				/// <summary>
				/// Generate an int or float using the current character as a starting point
				/// </summary>
				/// <returns>Generated token</returns>
				Token BuildNumber();

			public:
				/// <summary>
				/// Using the saved characters, generate all tokens
				/// </summary>
				/// <returns>All of the tokens that have been generated</returns>
				std::vector<Token> GenerateTokens();

				Lexer() = delete;
				Lexer(const Lexer&) = delete;

				/// <summary>
				/// Create a new Lexer
				/// </summary>
				/// <param name="input">The text to tokenise</param>
				/// <param name="inputName">The name of the stream where this text came from. If from the console for example, it will be <stdio></param>
				Lexer(std::string input, std::string inputName);
			};
		}
	}
}