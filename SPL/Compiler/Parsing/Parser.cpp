#include "Parser.h"

std::vector<SPL::Compiler::Parser::Nodes::Node*> SPL::Compiler::Parser::Parser::Parse()
{
    std::vector<Node*> statements;

    while (PeekCurrent().GetTokenType() != Tokenisation::TokenType::_NULL)
    {
        statements.push_back(Statement());
    }

    std::vector<Node*> temp;
    for (Node* n : statements) if (n != nullptr) temp.push_back(n);
    return temp;
}

SPL::Compiler::Parser::Parser::Parser(std::vector<Token> tokens)
{
	this->tokens = tokens;
	currentToken = 0;
}

void SPL::Compiler::Parser::Parser::Advance(int totalTokensToSkip)
{
	//In order to skip a set amount of tokens, we can simple call the other advance function x amount of times
	for (int i = 0; i < totalTokensToSkip; i++) Advance();
}

Token SPL::Compiler::Parser::Parser::Advance()
{
    currentToken++;

	if (currentToken < tokens.size())
	{
		return tokens[currentToken];
	}

	return NullToken();
}

Token SPL::Compiler::Parser::Parser::PeekCurrent()
{
	if ((size_t)currentToken < tokens.size())
	{
		return tokens[currentToken];
	}

	return NullToken();
}

Token SPL::Compiler::Parser::Parser::PeekNext()
{
	if ((size_t)currentToken + 1 < tokens.size())
	{
		return tokens[(size_t)currentToken + 1];
	}

	return NullToken();
}

SPL::Compiler::Parser::Nodes::Let* SPL::Compiler::Parser::Parser::ParseVariableDefination()
{
    //So we know that the current token is a let keyword, so we can skip this token
    Token let = PeekCurrent();
    Token name = Advance();

    Advance();

    //Check that the name is an identifier
    if (name.GetTokenType() != Tokenisation::TokenType::IDENTIFIER)
    {
        SPL_ERROR_CODE errorCode = name.GetTokenType() != Tokenisation::TokenType::KEYWORD ? SPL_VARNAME_NOT_IDEN : SPL_VARNAME_IS_KEY;
        Error(errorCode, name, ErrorMessages[errorCode], "Parser.cpp");
    }

    //Now that we have the name, we need to get the value stored, using ParseExpressino to get the value
    Value* value = ParseExpression();

    //Finally we can now construct a LetDef node
    return new Let(let, value, name);
}

SPL::Compiler::Parser::Nodes::Constant* SPL::Compiler::Parser::Parser::ParseConstantStatement()
{
    Token _const = PeekCurrent();

    //Check the variable name
    Token name = Advance();
    Advance();

    //Make sure the name is valid
    if (name.GetTokenType() != Tokenisation::TokenType::IDENTIFIER)
    {
        //We need to throw a different error based on if this is a keyword or nor
        SPL_ERROR_CODE errorCode = name.GetTokenType() != Tokenisation::TokenType::KEYWORD ? SPL_VARNAME_NOT_IDEN : SPL_VARNAME_IS_KEY;
        Error(errorCode, name, ErrorMessages[errorCode], "Parser.cpp");
    }

    Value* value = ParseExpression();

    //Check that the value is not an identifier, as they are a runtime value, whereas const expects a compile time value
    if (value->Type() == ValueType::IDENTIFIER)
    {
        Error(SPL_CONST_NOT_LITERAL, *value, ErrorMessages[SPL_CONST_NOT_LITERAL], "Parser.cpp");
    }

    return new Constant(_const, name, value);
}

SPL::Compiler::Parser::Nodes::Free* SPL::Compiler::Parser::Parser::ParseFreeStatement()
{
    Token freeKeyword = PeekCurrent();

    //We expect an identifier, so check one has been provided
    if (Advance().GetTokenType() == Tokenisation::TokenType::IDENTIFIER)
    {
        Free* free = new Free(freeKeyword, PeekCurrent());
        Advance();

        return free;
    }

    Error(SPL_FREE_NOT_VAR, freeKeyword, ErrorMessages[SPL_FREE_NOT_VAR], "Parser.cpp");
    return nullptr;
}

SPL::Compiler::Parser::Nodes::Print* SPL::Compiler::Parser::Parser::ParsePrintStatement()
{
    Token printKey = PeekCurrent();
    Advance();

    //Get an expression to print
    return new Print(printKey, ParseExpression());
}

SPL::Compiler::Parser::Nodes::Exit* SPL::Compiler::Parser::Parser::ParseExitStatement()
{
    Token exitKeyword = PeekCurrent();
    Advance();
    Value* exitValue = ParseExpression();

    return new Exit(exitKeyword, exitValue);
}

SPL::Compiler::Parser::Nodes::Goto* SPL::Compiler::Parser::Parser::ParseGotoStatement()
{
    Token gotoToken = PeekCurrent();
    Advance();

    Value* line = ParseExpression();

    return new Goto(gotoToken, line);
}

SPL::Compiler::Parser::Nodes::Push* SPL::Compiler::Parser::Parser::ParsePushStatement()
{
    Token pushToken = PeekCurrent();
    Advance();

    Value* toPush = ParseExpression();

    return new Push(pushToken, toPush);
}

SPL::Compiler::Parser::Nodes::SetPop* SPL::Compiler::Parser::Parser::ParseSetPopStatement()
{
    Token initialKeyword = PeekCurrent();
    Advance();

    Value* name = ParseExpression();

    //Check that the name is an identifier
    if (name->Token().GetTokenType() != Tokenisation::TokenType::IDENTIFIER)
    {
        SPL_ERROR_CODE errorCode = name->Token().GetTokenType() != Tokenisation::TokenType::KEYWORD ? SPL_VARNAME_NOT_IDEN : SPL_VARNAME_IS_KEY;
        Error(errorCode, name->Token(), ErrorMessages[errorCode], "Parser.cpp");
    }

    return new SetPop(initialKeyword, name);
}

SPL::Compiler::Parser::Nodes::Concat* SPL::Compiler::Parser::Parser::ParseConcatStatement()
{
    Concat* concat = new Concat(PeekCurrent());
    Advance();
    return concat;
}

SPL::Compiler::Parser::Nodes::Pop* SPL::Compiler::Parser::Parser::ParsePopStatement()
{
    Pop* pop = new Pop(PeekCurrent());
    Advance();
    return pop;
}

SPL::Compiler::Parser::Nodes::Call* SPL::Compiler::Parser::Parser::ParseCallStatement()
{
    Token call = PeekCurrent();
    Advance();

    //Thanks to the preprocessor, we know that we have should have an integer which contains the line number
    //We should double check anyway to be sure
    if (PeekCurrent().GetTokenType() != Tokenisation::TokenType::INT)
    {
        Error(SPL_CALL_INVALID_ADDRESS, PeekNext(), ErrorMessages[SPL_CALL_INVALID_ADDRESS], "Parser.cpp");
    }

    return new Call(call, ParseExpression());
}

SPL::Compiler::Parser::Nodes::Ret* SPL::Compiler::Parser::Parser::ParseRetStatement()
{
    Ret* ret = new Ret(PeekCurrent());
    Advance();
    return ret;
}

SPL::Compiler::Parser::Nodes::Input* SPL::Compiler::Parser::Parser::ParseInputStatement()
{
    Input* input = new Input(PeekCurrent());
    Advance();
    return input;
}

SPL::Compiler::Parser::Nodes::Console* SPL::Compiler::Parser::Parser::ParseConsoleStatement()
{
    Token t = PeekCurrent();
    Advance();
    Value* consoleOp = ParseExpression();

    //Check we have an identifier
    if (consoleOp->Type() != ValueType::IDENTIFIER)
    {
        Error(SPL_INVALID_CONSOLE_OP, *consoleOp, ErrorMessages[SPL_INVALID_CONSOLE_OP], "Parser.cpp");
    }

    return new Console(consoleOp, t);
}

SPL::Compiler::Parser::Nodes::Sleep* SPL::Compiler::Parser::Parser::ParseSleepStatement()
{
    Token t = PeekCurrent();
    Advance();

    Value* delay = ParseExpression();

    //Make sure an integer was provided
    if (delay->Type() != ValueType::INT)
    {
        Error(SPL_SLEEP_NO_INT, *delay, ErrorMessages[SPL_SLEEP_NO_INT], "Parser.cpp");
    }

    return new Nodes::Sleep(delay, t);
}

SPL::Compiler::Parser::Nodes::Equ* SPL::Compiler::Parser::Parser::ParseEquStatement()
{
    Token t = PeekCurrent();
    Advance();

    Value* line = ParseExpression();
    return new Equ(t, line);
}

SPL::Compiler::Parser::Nodes::Neq* SPL::Compiler::Parser::Parser::ParseNeqStatement()
{
    Token t = PeekCurrent();
    Advance();

    Value* line = ParseExpression();
    return new Neq(t, line);
}

SPL::Compiler::Parser::Nodes::Grt* SPL::Compiler::Parser::Parser::ParseGrtStatement()
{
    Token t = PeekCurrent();
    Advance();

    Value* line = ParseExpression();
    return new Grt(t, line);
}

SPL::Compiler::Parser::Nodes::GrtEqu* SPL::Compiler::Parser::Parser::ParseGrtEquStatement()
{
    Token t = PeekCurrent();
    Advance();

    Value* line = ParseExpression();
    return new GrtEqu(t, line);
}

SPL::Compiler::Parser::Nodes::Lwr* SPL::Compiler::Parser::Parser::ParseLwrStatement()
{
    Token t = PeekCurrent();
    Advance();

    Value* line = ParseExpression();
    return new Lwr(t, line);
}

SPL::Compiler::Parser::Nodes::LwrEqu* SPL::Compiler::Parser::Parser::ParseLwrEquStatement()
{
    Token t = PeekCurrent();
    Advance();

    Value* line = ParseExpression();
    return new LwrEqu(t, line);
}

SPL::Compiler::Parser::Nodes::Add* SPL::Compiler::Parser::Parser::ParseAddStatement()
{
    Add* add = new Add(PeekCurrent());
    Advance();
    return add;
}

SPL::Compiler::Parser::Nodes::Sub* SPL::Compiler::Parser::Parser::ParseSubStatement()
{
    Sub* sub = new Sub(PeekCurrent());
    Advance();
    return sub;
}

SPL::Compiler::Parser::Nodes::Mul* SPL::Compiler::Parser::Parser::ParseMulStatement()
{
    Mul* mul = new Mul(PeekCurrent());
    Advance();
    return mul;
}

SPL::Compiler::Parser::Nodes::Div* SPL::Compiler::Parser::Parser::ParseDivStatement()
{
    Div* div = new Div(PeekCurrent());
    Advance();
    return div;
}

SPL::Compiler::Parser::Nodes::Pow* SPL::Compiler::Parser::Parser::ParsePowStatement()
{
    Pow* pow = new Pow(PeekCurrent());
    Advance();
    return pow;
}

SPL::Compiler::Parser::Nodes::Inc* SPL::Compiler::Parser::Parser::ParseIncStatement()
{
    Inc* inc = new Inc(PeekCurrent());
    Advance();
    return inc;
}

SPL::Compiler::Parser::Nodes::Dec* SPL::Compiler::Parser::Parser::ParseDecStatement()
{
    Dec* dec = new Dec(PeekCurrent());
    Advance();
    return dec;
}

SPL::Compiler::Parser::Nodes::Mod* SPL::Compiler::Parser::Parser::ParseModStatement()
{
    Mod* mod = new Mod(PeekCurrent());
    Advance();
    return mod;
}

SPL::Compiler::Parser::Nodes::Casting::ToFloat* SPL::Compiler::Parser::Parser::ParseToFloatStatement()
{
    ToFloat* toFloat = new ToFloat(PeekCurrent());
    Advance();
    return toFloat;
}

SPL::Compiler::Parser::Nodes::Casting::ToInt* SPL::Compiler::Parser::Parser::ParseToIntStatement()
{
    ToInt* toInt = new ToInt(PeekCurrent());
    Advance();
    return toInt;
}

SPL::Compiler::Parser::Nodes::Casting::ToString* SPL::Compiler::Parser::Parser::ParseToStringStatement()
{
    ToString* toString = new ToString(PeekCurrent());
    Advance();
    return toString;
}

SPL::Compiler::Parser::Nodes::Value* SPL::Compiler::Parser::Parser::ParseExpression()
{
    //All valid types that an expression can be. In the future we need to check for comparisons.
    std::vector<Tokenisation::TokenType> validExpressionTypes = { Tokenisation::TokenType::INT, Tokenisation::TokenType::FLOAT, Tokenisation::TokenType::IDENTIFIER, Tokenisation::TokenType::STRING };
    
    if (std::count(validExpressionTypes.begin(), validExpressionTypes.end(), PeekCurrent().GetTokenType()))
    {
        Token t = PeekCurrent();

        if (t.GetTokenType() == Tokenisation::TokenType::STRING)
        {
            std::string lex = t.GetLexeme();
            std::string val = t.GetValueString();
            t.SetValueString(Terminate(val));
        }

        Value* v = new Value(t, TokenTypeToValueType(t.GetTokenType()));
        Advance();
        return v;
    }
    else
    {
        Error(SPL_EXPRESSION, PeekCurrent(), ErrorMessages[SPL_EXPRESSION], "Parser.cpp");
        return nullptr;
    }
}

SPL::Compiler::Parser::Nodes::Node* SPL::Compiler::Parser::Parser::Statement()
{
    if (PeekCurrent().GetTokenType() == Tokenisation::TokenType::KEYWORD)
    {
        std::string lex = PeekCurrent().GetLexeme();
        if (lex == "let") return ParseVariableDefination();
        else if (lex == "const") return ParseConstantStatement();
        else if (lex == "print") return ParsePrintStatement();
        else if (lex == "free") return ParseFreeStatement();
        else if (lex == "exit") return ParseExitStatement();
        else if (lex == "goto") return ParseGotoStatement();
        else if (lex == "call") return ParseCallStatement();
        else if (lex == "ret") return ParseRetStatement();
        else if (lex == "push") return ParsePushStatement();
        else if (lex == "add") return ParseAddStatement();
        else if (lex == "inc") return ParseIncStatement();
        else if (lex == "dec") return ParseDecStatement();
        else if (lex == "sub") return ParseSubStatement();
        else if (lex == "mul") return ParseMulStatement();
        else if (lex == "div") return ParseDivStatement();
        else if (lex == "pow") return ParsePowStatement();
        else if (lex == "setpop") return ParseSetPopStatement();
        else if (lex == "to_float") return ParseToFloatStatement();
        else if (lex == "to_int") return ParseToIntStatement();
        else if (lex == "to_string") return ParseToStringStatement();
        else if (lex == "concat") return ParseConcatStatement();
        else if (lex == "pop") return ParsePopStatement();
        else if (lex == "equ") return ParseEquStatement();
        else if (lex == "neq") return ParseNeqStatement();
        else if (lex == "grt") return ParseGrtStatement();
        else if (lex == "grtequ") return ParseGrtEquStatement();
        else if (lex == "lwr") return ParseLwrStatement();
        else if (lex == "lwrequ") return ParseLwrEquStatement();
        else if (lex == "input") return ParseInputStatement();
        else if (lex == "mod") return ParseModStatement();
        else if (lex == "console") return ParseConsoleStatement();
        else if (lex == "sleep") return ParseSleepStatement();
        else
        {
            std::string params[]{ PeekCurrent().GetLexeme()};
            std::string message = GetMessageWithParams(ErrorMessages[SPL_UNEXPECTED_KEYWORD], 1, params);
            Error(SPL_UNEXPECTED_KEYWORD, PeekCurrent(), message, "Parser.cpp");
            return nullptr;
        }
    }
    else if (PeekCurrent().GetTokenType() == Tokenisation::TokenType::COLON)
    {
        //For now we can just skip the labels
        Advance(2);
        return nullptr;
    }
    else
    {
        std::string params[]{ PeekCurrent().GetLexeme() };
        std::string message = GetMessageWithParams(ErrorMessages[SPL_UNEXPECTED_TOKEN], 1, params);
        Error(SPL_UNEXPECTED_TOKEN, PeekCurrent(), message, "Parser.cpp");
        return nullptr;
    }
}