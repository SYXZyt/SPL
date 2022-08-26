#pragma once
#include <string>
#include <vector>

#include "Nodes\Let.h"
#include "Nodes\Free.h"
#include "Nodes\Node.h"
#include "Nodes\Value.h"
#include "Nodes\Block.h"
#include "Nodes\Input.h"
#include "Nodes\Single.h"
#include "Nodes\Constant.h"
#include "Nodes\Maths\Add.h"
#include "Nodes\Maths\Div.h"
#include "Nodes\Maths\Mul.h"
#include "Nodes\Maths\Pow.h"
#include "Nodes\Maths\Sub.h"
#include "Nodes\Maths\Inc.h"
#include "Nodes\Maths\Dec.h"
#include "Nodes\Maths\Mod.h"
#include "Nodes\Stack\Pop.h"
#include "Nodes\Stack\Push.h"
#include "Nodes\Stack\SetPop.h"
#include "Nodes\Maths\Concat.h"
#include "Nodes\Output\Print.h"
#include "Nodes\Casting\ToInt.h"
#include "../../../ErrorStream.h"
#include "Nodes\Casting\ToFloat.h"
#include "../Tokenisation/Token.h"
#include "Nodes\ControlFlow\Ret.h"
#include "Nodes\Comparisons\Equ.h"
#include "Nodes\Comparisons\Neq.h"
#include "Nodes\Comparisons\Grt.h"
#include "Nodes\Comparisons\Lwr.h"
#include "Nodes\Casting\ToString.h"
#include "Nodes\ControlFlow\Exit.h"
#include "Nodes\ControlFlow\Goto.h"
#include "Nodes\ControlFlow\Call.h"
#include "Nodes\Comparisons\GrtEqu.h"
#include "Nodes\Comparisons\LwrEqu.h"

using namespace SPL::Compiler::Parser::Nodes;
using namespace SPL::Compiler::Parser::Nodes::Casting;

namespace SPL
{
	namespace Compiler
	{
		namespace Parser
		{
			/// <summary>
			/// Class which will convert code tokens to code nodes, making them closer to their final byte code representations
			/// </summary>
			class Parser final
			{
			private:
				std::vector<Tokenisation::Token> tokens;
				int currentToken;

				/// <summary>
				/// Advance the token pointer by a set amount
				/// </summary>
				/// <param name="totalTokensToSkip">How many tokens to advance by</param>
				void Advance(int totalTokensToSkip);

				/// <summary>
				/// Advance to the next token
				/// </summary>
				/// <returns>The token at the next position</returns>
				Token Advance();

				/// <summary>
				/// Get the current token
				/// </summary>
				/// <returns>The current token</returns>
				Token PeekCurrent();

				/// <summary>
				/// Get the next token
				/// </summary>
				/// <returns>The next token</returns>
				Token PeekNext();

				/// <summary>
				/// Generate a blank token
				/// </summary>
				/// <returns>The blank token</returns>
				inline Token NullToken()
				{
					return Token("NULL", SPL::Compiler::Tokenisation::TokenType::_NULL, Vector2(), "NULL");
				}

				Nodes::Let* ParseVariableDefination();
				Nodes::Constant* ParseConstantStatement();
				Nodes::Free* ParseFreeStatement();
				Nodes::Print* ParsePrintStatement();
				Nodes::Exit* ParseExitStatement();
				Nodes::Goto* ParseGotoStatement();
				Nodes::Push* ParsePushStatement();
				Nodes::SetPop* ParseSetPopStatement();
				Nodes::Concat* ParseConcatStatement();
				Nodes::Pop* ParsePopStatement();
				Nodes::Call* ParseCallStatement();
				Nodes::Ret* ParseRetStatement();
				Nodes::Input* ParseInputStatement();
				
#pragma region Comparisons
				Nodes::Equ* ParseEquStatement();
				Nodes::Neq* ParseNeqStatement();
				Nodes::Grt* ParseGrtStatement();
				Nodes::GrtEqu* ParseGrtEquStatement();
				Nodes::Lwr* ParseLwrStatement();
				Nodes::LwrEqu* ParseLwrEquStatement();
#pragma endregion

#pragma region Maths
				Nodes::Add* ParseAddStatement();
				Nodes::Sub* ParseSubStatement();
				Nodes::Mul* ParseMulStatement();
				Nodes::Div* ParseDivStatement();
				Nodes::Pow* ParsePowStatement();
				Nodes::Inc* ParseIncStatement();
				Nodes::Dec* ParseDecStatement();
				Nodes::Mod* ParseModStatement();
#pragma endregion
#pragma region
				Nodes::Casting::ToFloat* ParseToFloatStatement();
				Nodes::Casting::ToInt* ParseToIntStatement();
				Nodes::Casting::ToString* ParseToStringStatement();
#pragma endregion

				Nodes::Value* ParseExpression();
				Nodes::Node* Statement();

			public:
				/// <summary>
				/// Convert loaded tokens into nodes
				/// </summary>
				/// <returns>List of the newly generated nodes</returns>
				std::vector<Nodes::Node*> Parse();

				/// <summary>
				/// Create a new parser
				/// </summary>
				/// <param name="tokens">The tokens to parse</param>
				Parser(std::vector<Token> tokens);
			};
		}
	}
}