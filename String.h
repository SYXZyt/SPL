#pragma once
#include <string>
#include <vector>

/// <summary>
/// Checks if a character can be considered whitespace
/// </summary>
/// <param name="c">The character to check</param>
/// <returns>True if the character is whitespace</returns>
static bool IsWhiteSpace(const char c)
{
	return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

/// <summary>
/// Add a null terminator to a string if it does not have one
/// </summary>
/// <param name="s">The string to terminate</param>
/// <returns>The string with a terminator on the end</returns>
static std::string& Terminate(std::string& s)
{
	if (s.size() == 0)
	{
		s = "\0";
		return s;
	}

	if (s.back() != '\0') s += '\0';
	return s;
}

/// <summary>
/// Convert a string into a list of ASCII characters
/// </summary>
/// <param name="s">The string to convert</param>
/// <returns>The list containing the ASCII characters</returns>
static std::vector<unsigned char> GetAscii(std::string s)
{
	if (s.size() == 0)
	{
		std::vector<unsigned char> v;
		v.push_back(0x00);
		return v;
	}

	Terminate(s);

	std::vector<unsigned char> v;
	for (char c : s) v.push_back((unsigned)c);
	return v;
}

/// <summary>
/// Replace all escape sequences with non escape characters. E.g. \n -> \\n
/// </summary>
/// <param name="s">The string to escape</param>
/// <returns>The string with no escape sequences</returns>
static std::string Escape(std::string s)
{
	std::string newString = "";
	for (char c : s)
	{
		if (c == '\a')
		{
			newString += "\\a";
		}
		else if (c == '\b')
		{
			newString += "\\b";
		}
		else if (c == '\f')
		{
			newString += "\\f";
		}
		else if (c == '\n')
		{
			newString += "\\n";
		}
		else if (c == '\r')
		{
			newString += "\\r";
		}
		else if (c == '\t')
		{
			newString += "\\t";
		}
		else if (c == '\v')
		{
			newString += "\\v";
		}
		else if (c == '\0')
		{
			newString += "\\0";
		}
		else
		{
			newString += c;
		}
	}

	return newString;
}

static std::string Replace(std::string& str, const std::string original, const std::string substr)
{
	if (str.empty()) return "";

	size_t startPos = 0;
	while ((startPos = str.find(original, startPos)) != std::string::npos)
	{
		str.replace(startPos, original.length(), substr);
		startPos += substr.length();
	}

	return str;
}