#pragma once
#include <sstream>
#include <cstdarg>
#include <iostream>

#pragma region Windows_Stuff
#ifdef _WIN32
#include <Windows.h>
/// <summary>
/// Enum which stores all 16 colours that the terminal supports
/// </summary>
enum class ConsoleColour
{
	BLACK,
	DARK_BLUE,
	DARK_GREEN,
	DARK_CYAN,
	DARK_RED,
	DARK_PINK,
	DARK_YELLOW,
	GREY,
	DARK_GREY,
	BLUE,
	GREEN,
	CYAN,
	RED,
	PINK,
	YELLOW,
	WHITE,
};
static void SetConsoleColour(ConsoleColour color)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console, (WORD)color);
}
static void ResetConsoleColour()
{
	SetConsoleColour(ConsoleColour::WHITE);
}
#endif
#pragma endregion

#include "String.h"
#include "SPLErrors.h"
#include "SPL\Compiler\Tokenisation\Token.h"
#include "SPL\Compiler\Parsing\Nodes\Node.h"

using SPL::Compiler::Tokenisation::Token;
using SPL::Compiler::Parser::Nodes::Node;

/// <summary>
/// Report an error message, without killing program execution
/// </summary>
/// <param name="message">The error message</param>
static void ErrorNoExit(std::string message)
{
#ifdef _WIN32
	SetConsoleColour(ConsoleColour::RED);
#endif
	std::cerr << message << std::endl;

#ifdef _WIN32
	ResetConsoleColour();
#endif
}

static void ErrorNoExit(const SPL_ERROR_CODE code, std::string message)
{
	std::stringstream ss;
	ss << code << " : " << message;
	ErrorNoExit(ss.str());
}

/// <summary>
/// Report an error to the console
/// </summary>
/// <param name="message">The error message</param>
static void Error(std::string message)
{
	ErrorNoExit(message);
	exit(-1);
}

/// <summary>
/// Report an error message
/// </summary>
/// <param name="token">The token that caused the error</param>
/// <param name="message">The error message</param>
/// <param name="filename">The file that caught the error. Only available in debug mode</param>
static void Error(Token token, std::string message, std::string filename = "")
{
	std::stringstream ss;
	ss << message << "\nAt position " << token.GetPosition().X() + 1 << ", Line " << token.GetPosition().Y() + 1 << ", In '" << token.GetInputName() << "', at '" << token.GetLexeme() << '\'' << std::endl;

#ifdef _DEBUG
	ss << filename;
#endif

	Error(ss.str());
}

/// <summary>
/// Report an error message
/// </summary>
/// <param name="node">The node that caused the error</param>
/// <param name="message">The error message</param>
/// <param name="filename">The file that caught the error. Only available in debug mode</param>
static void Error(Node node, std::string message, std::string filename = "")
{
	Error(node.Token(), message, filename);
}

/// <summary>
/// Report an warning to the console
/// </summary>
/// <param name="message">The warning to print</param>
static void Warning(std::string message)
{
#ifdef _WIN32
	SetConsoleColour(ConsoleColour::DARK_YELLOW);
#endif
	std::cout << message << std::endl;
#ifdef _WIN32
	ResetConsoleColour();
#endif
}

/// <summary>
/// Report an warning to the console
/// </summary>
/// <param name="token">The token that caused the error</param>
/// <param name="message">The warning to print</param>
/// <param name="filename">The file that the error occurred in</param>
static void Warning(Token token, std::string message, std::string filename = "")
{
	std::stringstream ss;
	ss << message << "\nAt position " << token.GetPosition().X() + 1 << ", Line " << token.GetPosition().Y() + 1 << ", In '" << token.GetInputName() << "', at '" << token.GetLexeme() << '\'' << std::endl;

#ifdef _DEBUG
	ss << filename;
#endif

	Warning(ss.str());
}

/// <summary>
/// Report an warning to the console
/// </summary>
/// <param name="token">The node that caused the error</param>
/// <param name="message">The warning to print</param>
/// <param name="filename">The file that the error occurred in</param>
static void Warning(Node node, std::string message, std::string filename = "")
{
	Warning(node.Token(), message, filename);
}

static void Warning(const SPL_ERROR_CODE code, Node node, std::string message, std::string filename = "")
{
	std::stringstream ss;
	ss << code << " : " << message;
	Warning(node, ss.str(), filename);
}

static std::string GetMessageWithParams(std::string msg, int argc, std::string params[])
{
	std::string output = msg;;

	for (int i = 0; i < argc; i++)
	{
		std::stringstream ss;
		ss << "&SPL_";
		ss << i;
		output = Replace(output, ss.str(), params[i]);
	}

	return output;
}

static void Error(const SPL_ERROR_CODE code, std::string message)
{
	std::stringstream ss;
	ss << code << " : " << message;
	Error(ss.str());
}

static void Error(const SPL_ERROR_CODE code, Token t, std::string message, std::string filename)
{
	std::stringstream ss;
	ss << code << " : " << message;
	Error(t, ss.str(), filename);
}

static void Error(const SPL_ERROR_CODE code, Node n, std::string message, std::string filename)
{
	std::stringstream ss;
	ss << code << " : " << message;
	Error(n, ss.str(), filename);
}