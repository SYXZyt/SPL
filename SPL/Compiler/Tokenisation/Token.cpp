#include "Token.h"

void SPL::Compiler::Tokenisation::Token::SetValueString(std::string str)
{
	this->str = str;
	flt = std::numeric_limits<float>::quiet_NaN();
	integer = 0;
}

void SPL::Compiler::Tokenisation::Token::SetValueFloat(float flt)
{
	str = "";
	this->flt = flt;
	integer = 0;
}

void SPL::Compiler::Tokenisation::Token::SetValueInt(int integer)
{
	str = "";
	flt = std::numeric_limits<float>::quiet_NaN();;
	this->integer = integer;
}

void SPL::Compiler::Tokenisation::Token::SetLexeme(std::string newLexeme)
{
	lexeme = newLexeme;
}

SPL::Compiler::Tokenisation::Token::Token()
{
	pos = Vector2();
	type = TokenType::STRING;
	lexeme = "SPL_NULL";
	inputName = "SPL_NULL";
}

SPL::Compiler::Tokenisation::Token::Token(const Token& t)
{
	pos = t.pos;
	type = t.type;
	lexeme = t.lexeme;
	inputName = t.inputName;

	flt = t.flt;
	str = t.str;
	integer = t.integer;
}

SPL::Compiler::Tokenisation::Token::Token(std::string lexeme, TokenType type, Vector2 pos, std::string inputName)
{
	this->lexeme = lexeme;
	this->type = type;
	this->pos = pos;
	this->inputName = inputName;
	str = "";
	flt = std::numeric_limits<float>::quiet_NaN();
	integer = 0;
}