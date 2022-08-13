#include "PreProcessor.h"

std::vector<SPL::Compiler::Tokenisation::Token> SPL::Compiler::PreProcessing::PreProcessor::Process()
{
	using namespace SPL::Compiler::Tokenisation;

	std::unordered_map<std::string, int> labels;
	std::vector<Token> tokens;

    //Loop over every token in the preParsed array, and check if the token has the type "TokenType.COLON"
    for (int i = 0; i < preParsed.size(); i++)
    {
        if (CurrentToken(i).GetTokenType() == TokenType::COLON)
        {
            //If this is a label def, we need to store the next tokens name in the labels dictionary, along with it's line number
            if (Peek(i).GetTokenType() == TokenType::IDENTIFIER)
            {
                i++;
                Token curr = CurrentToken(i);

                labels.insert(std::make_pair(curr.GetLexeme(), curr.GetPosition().Y()));
            }
            else
            {
                Error(Peek(i), "Expected an identifier after a label definition", "PreProcessor.cpp");
            }
        }
    }

    //Loop over every token and handle built ins as their name needs to be changed
    for (int i = 0; i < preParsed.size(); i++)
    {
        if (CurrentToken(i).GetTokenType() == TokenType::MINUS)
        {
            //Check the type of the next token, as minus can only be applied to numbers
            TokenType type = Peek(i).GetTokenType();
            if (type == TokenType::FLOAT || type == TokenType::INT)
            {
                i++;
                Token curr = CurrentToken(i);

                curr.SetLexeme("-" + curr.GetLexeme());

                if (curr.GetTokenType() == TokenType::INT)
                {
                    curr.SetValueInt(curr.GetValueInt() * -1);
                }
                else
                {
                    curr.SetValueFloat(curr.GetValueFloat() * -1);
                }

                tokens.push_back(curr);
            }
            else
            {
                Error(CurrentToken(i), "Expected a numerical value to follow '-'", "PreProcessor.cpp");
            }
        }
        else if (CurrentToken(i).GetTokenType() == TokenType::KEYWORD && CurrentToken(i).GetLexeme() == "println")
        {
            Token initPrint = CurrentToken(i);
            initPrint.SetLexeme("print");
            initPrint.SetValueString("print");
            tokens.push_back(initPrint);
            tokens.push_back(CurrentToken(++i));


            //Generate a new token to handle the new line part of the print
            Token a = Token("print", TokenType::KEYWORD, initPrint.GetPosition(), initPrint.GetInputName());
            Token b = Token("\r\n", TokenType::STRING, initPrint.GetPosition(), initPrint.GetInputName());
            a.SetValueString("print");
            b.SetValueString("\r\n");

            tokens.push_back(a);
            tokens.push_back(b);
        }
        else if (CurrentToken(i).GetTokenType() == TokenType::KEYWORD && (CurrentToken(i).GetLexeme() == "goto" || CurrentToken(i).GetLexeme() == "call"))
        {
            //Check if an identifier was passed, if not, we need to check for a number, otherwise this statement is invalid
            if (Peek(i).GetTokenType() == TokenType::IDENTIFIER)
            {
                tokens.push_back(CurrentToken(i++));

                //If the identifier is not in the dictionary, then we have to report an error as we can't continue otherwise
                if (!labels.count(CurrentToken(i).GetLexeme()))
                {
                    std::stringstream ss;
                    ss << "The label '" << CurrentToken(i).GetLexeme() << "' does not exist";
                    Error(CurrentToken(i), ss.str(), "PreProcessor.cpp");
                }

                int lineNumber = labels[CurrentToken(i).GetLexeme()];
                Token t = Token(std::to_string(lineNumber), TokenType::INT, CurrentToken(i).GetPosition(), CurrentToken(i).GetInputName());
                t.SetValueInt(lineNumber);
                tokens.push_back(t);
            }
            else if (Peek(i).GetTokenType() == TokenType::INT)
            {
                if (CurrentToken(i).GetLexeme() == "call") Error(CurrentToken(i), "'call' expects a label name", "PreProcessor.cpp");

                tokens.push_back(CurrentToken(i++));

                Token lineNum = CurrentToken(i);
                int newLine = std::stoi(lineNum.GetLexeme()) - 1;
                lineNum.SetLexeme(std::to_string(newLine));
                lineNum.SetValueInt(lineNum.GetValueInt() - 1);

                tokens.push_back(lineNum);
            }
            else
            {
                Error(CurrentToken(i), "'goto' expects a line number or a label name", "PreProcessor.cpp");
            }
        }
        else
        {
            tokens.push_back(CurrentToken(i));
        }
    }

    return tokens;
}

SPL::Compiler::PreProcessing::PreProcessor::PreProcessor(std::vector<SPL::Compiler::Tokenisation::Token> preParsed)
{
	this->preParsed = preParsed;
}