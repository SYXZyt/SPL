#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <unordered_map>

#include "../../../ErrorStream.h"
#include "../Tokenisation/Token.h"
#include "../Tokenisation/TokenType.h"

namespace SPL
{
	namespace Compiler
	{
		namespace PreProcessing
		{
			/// <summary>
			/// Class to edit tokens before they need to be parsed
			/// </summary>
			class PreProcessor final
			{
			private:
				std::vector<SPL::Compiler::Tokenisation::Token> preParsed;

				/// <summary>
				/// Get the token at a given index
				/// </summary>
				/// <param name="i">The current index</param>
				/// <returns>The token at the given index, or a blank token if the index is out of bounds</returns>
				inline SPL::Compiler::Tokenisation::Token CurrentToken(int i)
				{
					return i >= preParsed.size() ? SPL::Compiler::Tokenisation::Token() : preParsed[i];
				}

				/// <summary>
				/// Get the token next to the given index
				/// </summary>
				/// <param name="i">The current index</param>
				/// <returns>The token next to the given index, or a blank token if the index is out of bounds</returns>
				inline SPL::Compiler::Tokenisation::Token Peek(int i)
				{
					return CurrentToken(i + 1);
				}
			public:
				/// <summary>
				/// Process all of the tokens that have been loaded
				/// </summary>
				/// <returns></returns>
				std::vector<SPL::Compiler::Tokenisation::Token> Process();

				/// <summary>
				/// Create a new PreProcessor
				/// </summary>
				/// <param name="preParsed">The tokens to be processed</param>
				PreProcessor(std::vector<SPL::Compiler::Tokenisation::Token> preParsed);
			};
		}
	}
}