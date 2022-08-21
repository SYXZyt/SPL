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
	Advance();

	//Check if the user has provided the correct access modifier, and then set a bool accordingly
	if (PeekCurrent().GetTokenType() != Tokenisation::TokenType::KEYWORD && (PeekCurrent().GetLexeme() != ConstantKeyword && PeekCurrent().GetLexeme() != MutableKeyword))
	{
		Error(SPL_MISSING_ACCESS_MODIFIER, let, ErrorMessages[SPL_MISSING_ACCESS_MODIFIER], "Parser.cpp");
	}

	bool isMutable = PeekCurrent().GetLexeme() == MutableKeyword;
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
	return new Let(let, value, name, isMutable);
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

	Token access;

	//Check that an access token was provided
	if (PeekCurrent().GetLexeme() == ConstantKeyword || PeekCurrent().GetLexeme() == MutableKeyword)
	{
		access = PeekCurrent();
	}
	else
	{
		Error(SPL_MISSING_ACCESS_MODIFIER, PeekCurrent(), ErrorMessages[SPL_MISSING_ACCESS_MODIFIER], "Parser.cpp");
	}

	Token name = PeekNext();
	Advance(2);

	//Check that the name is an identifier
	if (name.GetTokenType() != Tokenisation::TokenType::IDENTIFIER)
	{
		SPL_ERROR_CODE errorCode = name.GetTokenType() != Tokenisation::TokenType::KEYWORD ? SPL_VARNAME_NOT_IDEN : SPL_VARNAME_IS_KEY;
		Error(errorCode, name, ErrorMessages[errorCode], "Parser.cpp");
	}

	return new SetPop(initialKeyword, access.GetLexeme() == MutableKeyword, name);
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

SPL::Compiler::Parser::Nodes::If* SPL::Compiler::Parser::Parser::ParseIfStatement()
{
	Token ifToken = PeekCurrent();
	Advance();

	//Check that we have been given a condition
	Condition* condition = ParseCondition();

	//Now we need to parse a code block
	//We don't want to just expect a block, but any valid statement, or else "else if" statements would require this "else { if ... {}}"
	Block* ifBlock;
	Node* ifStatement = Statement();

	//Basically if the user entered this `if ... println "true"` we want to put the single statement into a block so it becomes `if ... {println "true"}`
	if (ifBlock = dynamic_cast<Block*>(ifStatement)) {}
	else
	{
		ifBlock = new Block(ifStatement->Token());
		ifBlock->AddNode(ifStatement);
	}

	//Now we can check if an else statement is next, if not, set it to null
	If* elseBranch = nullptr;
	if (PeekNext().GetLexeme() == "else")
	{
		Advance();
		elseBranch = dynamic_cast<If*>(Statement());
	}

	return new If(ifToken, condition, ifBlock, elseBranch);
}

SPL::Compiler::Parser::Nodes::Condition* SPL::Compiler::Parser::Parser::ParseCondition()
{
	std::vector<Tokenisation::TokenType> validTypes =
	{
		Tokenisation::TokenType::EQUALS,
		Tokenisation::TokenType::NEQ,
		Tokenisation::TokenType::GREATER,
		Tokenisation::TokenType::GREATER_EQ,
		Tokenisation::TokenType::LOWER,
		Tokenisation::TokenType::LOWER_EQ,
	};

	Value* lhs = ParseExpression();

	Tokenisation::Token op;
	if (!std::count(validTypes.begin(), validTypes.end(), PeekCurrent().GetTokenType()))
	{
		std::string params[] {PeekCurrent().GetLexeme()};
		Error(SPL_INVALID_CONDITIONAL, PeekCurrent(), GetMessageWithParams(ErrorMessages[SPL_INVALID_CONDITIONAL], 1, params), "Parser.cpp");
	} op = PeekCurrent();
	Advance();

	Value* rhs = ParseExpression();

	return new Condition(lhs, op, rhs);
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
		else if (lex == "print") return ParsePrintStatement();
		else if (lex == "free") return ParseFreeStatement();
		else if (lex == "exit") return ParseExitStatement();
		else if (lex == "goto") return ParseGotoStatement();
		else if (lex == "call") return ParseCallStatement();
		else if (lex == "ret") return ParseRetStatement();
		else if (lex == "push") return ParsePushStatement();
		else if (lex == "add") return ParseAddStatement();
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
		else
		{
			std::string params[]{ PeekCurrent().GetLexeme() };
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