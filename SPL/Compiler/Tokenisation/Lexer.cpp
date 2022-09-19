#include "Lexer.h"

void SPL::Compiler::Tokenisation::Lexer::Advance()
{
	pos++;
	posOnLine++;

	if (pos >= input.length())
	{
		currentChar = '\0';
		return;
	}

	currentChar = input[pos];

	//Pos is the current character index, posOnLine is the position on this line, which is why we use it for error messages
	if (currentChar == '\n')
	{
		posOnLine = 0;
		line++;
	}
}

Vector2 SPL::Compiler::Tokenisation::Lexer::Pos()
{
	return Vector2(posOnLine, line);
}

SPL::Compiler::Tokenisation::Token SPL::Compiler::Tokenisation::Lexer::BuildIdentifier()
{
	Vector2 pos = Pos();
	TokenType type = TokenType::IDENTIFIER;

	std::string name;
	while (isalnum(currentChar) || currentChar == '.' || currentChar == '_')
	{
		name += currentChar;
		Advance();
	} Advance();

	if (std::count(keywords.begin(), keywords.end(), name)) type = TokenType::KEYWORD;

	Token t = Token(name, type, pos, inputName);
	t.SetValueString(Terminate(name));
	return t;
}

SPL::Compiler::Tokenisation::Token SPL::Compiler::Tokenisation::Lexer::BuildString()
{
	Vector2 pos = Pos();

	//Skip the opening quote
	Advance();

	std::string value = "";

	while (true)
	{
		//We need to check if the current character is the end quote
		if (currentChar == '"') break;

		if (currentChar == '\0' || currentChar == '\n')
		{
			Token t = Token(value, TokenType::STRING, pos, inputName);
			Error(SPL_UNTERMINATED_STRING, t, ErrorMessages[SPL_UNTERMINATED_STRING], "Lexer.cpp");
		}

		//Check if the current character is a backslash. so we can parse an escape sequence
		if (currentChar == '\\')
		{
			Advance();

			char escape;
			switch (currentChar)
			{
				case 'a':
					escape = '\a';
					break;
				case 'b':
					escape = '\b';
					break;
				case 'f':
					escape = '\f';
					break;
				case 'n':
					escape = '\n';
					break;
				case 'r':
					escape = '\r';
					break;
				case 't':
					escape = '\t';
					break;
				case 'v':
					escape = '\v';
					break;
				case '\'':
					escape = '\'';
					break;
				case '"':
					escape = '"';
					break;
				case '\\':
					escape = '\\';
					break;
				case '0':
					escape = '0'; //\0 is used to determine when a string ends, and it could cause issues so we just want to put the 0 instead
					break;
				default:
					escape = '#'; //This is not a valid escape sequence, so we can use this to check if the inputted one was also invalid
					break;
			}

			if (escape == '#')
			{
				Token t = Token("\\" + currentChar, TokenType::STRING, pos, inputName);
				std::string errorString = "\\"; errorString += currentChar;
				std::string params[1]{ errorString };
				std::string message = GetMessageWithParams(ErrorMessages[SPL_UNRECOGNISED_ESCAPE], 1, params);
				Error(SPL_UNRECOGNISED_ESCAPE, t, message, "Lexer.cpp");
			}

			value += escape;
			Advance();
			continue;
		}

		value += currentChar;
		Advance();
	} Advance();

	Token t = Token(value, TokenType::STRING, pos, inputName);
	Terminate(value);
	t.SetValueString(value);
	return t;
}

SPL::Compiler::Tokenisation::Token SPL::Compiler::Tokenisation::Lexer::BuildNumber()
{
	std::string strRepr = "";
	int decimalCount = 0;
	Vector2 pos = Pos();

	while ((currentChar >= '0' && currentChar <= '9') || currentChar == '.')
	{
		if (currentChar == '.')
		{
			decimalCount++;
		}

		strRepr += currentChar;
		Advance();
	}

	//Check that the number follows a valid format
	if (decimalCount > 1)
	{
		Token t = Token(strRepr, TokenType::INT, pos, inputName);
		Error(SPL_MALFORMED_NUMERIC, t, ErrorMessages[SPL_MALFORMED_NUMERIC], "Lexer.cpp");
	}

	Token t;
	if (decimalCount == 0)
	{
		t = Token(strRepr, TokenType::INT, pos, inputName);
		t.SetValueInt(std::stoi(strRepr));
	}
	else
	{
		t = Token(strRepr, TokenType::FLOAT, pos, inputName);
		t.SetValueFloat(std::stof(strRepr));
	}

	return t;
}

std::vector<SPL::Compiler::Tokenisation::Token> SPL::Compiler::Tokenisation::Lexer::GenerateTokens()
{
	std::vector<Token> tokens;

	//In order to make sure every character has been tokenised, we need to loop over every character until we find a null character, which will indicate when we have reached the end of the input
	while (currentChar != '\0')
	{
		//We need to check if the current character is whitespace, as there is nothing useful we can do with it, so we can skip it
		//We also have to run the check above again, as it's possible that after skipping characters, that we will land on a null character, and that will throw errors that the user can't control
		while (IsWhiteSpace(currentChar)) Advance();
		if (currentChar == '\0') continue;

		if (currentChar >= '0' && currentChar <= '9')
		{
			Token t = BuildNumber();
			tokens.push_back(t);
		}
		else if (isalnum(currentChar) || currentChar == '.' || currentChar == '_')
		{
			Token t = BuildIdentifier();
			tokens.push_back(t);
		}
		else if (currentChar == '"')
		{
			Token t = BuildString();
			tokens.push_back(t);
		}
		else if (currentChar == '-')
		{
			Token t = Token(std::string(1, currentChar), TokenType::MINUS, Pos(), inputName);
			tokens.push_back(t);
			Advance();
		}
		else if (currentChar == ':')
		{
			Token t = Token(std::string(1, currentChar), TokenType::COLON, Pos(), inputName);
			tokens.push_back(t);
			Advance();
		}
		else if (currentChar == ';') //In order to handle comments, we will skip every char until either end of line or end of file
		{
			while (currentChar != '\n' && currentChar != '\0')
			{
				Advance();
			}
		}
		else
		{
			Token t = Token(std::to_string(currentChar), TokenType::IDENTIFIER, Pos(), inputName);
			std::string params[]{ std::string(1, currentChar) };
			std::string message = GetMessageWithParams(ErrorMessages[SPL_ILLEGAL_CHARACTER], 1, params);
			Error(SPL_ILLEGAL_CHARACTER, t, message, "Lexer.cpp");
		}
	}

	return tokens;
}

SPL::Compiler::Tokenisation::Lexer::Lexer(std::string input, std::string inputName)
{
	this->input = input;
	this->inputName = inputName;

	pos = -1;
	posOnLine = -1;
	line = 0;
	currentChar = '\0';

	Advance();

	keywords =
	{
		"print",
		"println",
		"let",
		"const",
		"goto",
		"exit",
		"push",
		"pop",
		"setpop",
		"add",
		"sub",
		"mul",
		"div",
		"pow",
		"to_float",
		"to_int",
		"to_string",
		"free",
		"concat",
		"call",
		"ret",
		"equ",
		"neq",
		"grt",
		"grtequ",
		"lwr",
		"lwrequ",
		"inc",
		"dec",
		"input",
		"mod",
		"console",
		"sleep",
	};
}