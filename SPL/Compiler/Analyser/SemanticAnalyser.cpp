#include "SemanticAnalyser.h"

const std::string filename = "SemanticAnalyser.cpp";

int SPL::Compiler::Analyser::SemanticAnalyser::GetNodeIndexFromLine(Node* nodeRequesting, int line)
{
	for (int i = 0; i < nodes.size(); i++)
	{
		if (nodes[i].node->Token().GetPosition().Y() >= line)
		{
			return i;
		}
	}

	Error(SPL_OUT_OF_FILE, *nodeRequesting, ErrorMessages[SPL_OUT_OF_FILE], filename);
	return 0;
}

SPL::Compiler::Analyser::SemanticAnalyserResults SPL::Compiler::Analyser::SemanticAnalyser::Analyse()
{
	//This is the v2 on my analyser.
	//This version will also keep track of goto statements unlike the other one. It will also return a custom struct containing data on what to do in order to optimise the code, before
	//it is assembled.
	//The job of this analyser is to keep track of created variables to make sure we don't overwrite a const, or reference a variable that doesn't exist. It will also throw warnings such as
	//variables not referenced
	using namespace SPL::Compiler::Parser::Nodes;
	using namespace SPL::Compiler::Parser::Nodes::Casting;

	std::vector<Node*> nodesToCull;

	//We want to loop until we reach an exit node, or until eof
	int nodeIndex = 0;
	while (nodeIndex < nodes.size())
	{
		if (callstack.size() >= callstackMaxSize) Error(SPL_STACKOVERFLOW, ErrorMessages[SPL_STACKOVERFLOW]);

		//This dynamic cast will return null if the current node is not that type, so by checking it in an if, we can select the specific object type
		if (dynamic_cast<Let*>(nodes[nodeIndex].node))
		{
			Let* let = dynamic_cast<Let*>(nodes[nodeIndex].node);
			//Our goal here is to:
			//Check if this variable exists. If not, just write it
			//If it does exist, check if it is constant, so we can throw an error
			//If it is not constant, overwrite it
			std::string varName = let->Name().GetLexeme();
			bool isMutable = let->IsMutable();

			if (variables.count(varName))
			{
				if (!variables[varName].hasBeenReferenced)
				{
					Warning(WSPL_UNUSED_VAR, *let, ErrorMessages[WSPL_UNUSED_VAR], filename);
				}


				if (!variables[varName].isMutable)
				{
					Error(SPL_CONST_OVERWRITE, *let, ErrorMessages[SPL_CONST_OVERWRITE], filename);
				}
			}

			variables[varName] = Variable(isMutable);
		}
		else if (dynamic_cast<Print*>(nodes[nodeIndex].node))
		{
			Print* print = dynamic_cast<Print*>(nodes[nodeIndex].node);

			//Make sure we are printing an existing piece of data
			if (print->ToPrint()->Type() == ValueType::IDENTIFIER)
			{
				std::string varName = print->ToPrint()->Token().GetLexeme();

				if (!variables.count(varName))
				{
					Error(SPL_REF_VAR, *print, ErrorMessages[SPL_REF_VAR], filename);
				}

				variables[varName].hasBeenReferenced = true;
			}
		}
		else if (dynamic_cast<Free*>(nodes[nodeIndex].node))
		{
			Free* free = dynamic_cast<Free*>(nodes[nodeIndex].node);
			if (!variables.count(free->VariableName().GetLexeme()))
			{
				Error(SPL_REF_VAR, *free, ErrorMessages[SPL_REF_VAR], filename);
			}
		}
		else if (dynamic_cast<Exit*>(nodes[nodeIndex].node))
		{
			Exit* exit = dynamic_cast<Exit*>(nodes[nodeIndex].node);
			nodes[nodeIndex].hasBeenReached = true;

			//Make sure if the exit code is a variable, that it currently exists
			if (exit->GetValue()->Type() == ValueType::IDENTIFIER)
			{
				if (!variables.count(exit->GetValue()->Token().GetLexeme()))
				{
					Error(SPL_REF_VAR, *exit, ErrorMessages[SPL_REF_VAR], filename);
				}
			}

			break;
		}
		else if (dynamic_cast<Push*>(nodes[nodeIndex].node))
		{
			Push* push = dynamic_cast<Push*>(nodes[nodeIndex].node);

			if (push->GetValue()->Type() == ValueType::IDENTIFIER)
			{
				if (!variables.count(push->GetValue()->Token().GetLexeme()))
				{
					Error(SPL_REF_VAR, *push, ErrorMessages[SPL_REF_VAR], filename);
				}
			}

			stackSize++;
		}
		else if (dynamic_cast<Pop*>(nodes[nodeIndex].node))
		{
			if (stackSize <= 0) Error(SPL_POP_EMPTY, *nodes[nodeIndex].node, ErrorMessages[SPL_POP_EMPTY], filename);

			stackSize--;
		}
		else if (dynamic_cast<Goto*>(nodes[nodeIndex].node))
		{
			if (!nodes[nodeIndex].hasBeenReached)
			{
				Goto* _goto = dynamic_cast<Goto*>(nodes[nodeIndex].node);
				nodes[nodeIndex].hasBeenReached = true;
				nodeIndex = GetNodeIndexFromLine(_goto, _goto->Line()->Token().GetValueInt());

				//We want to run continue to make sure that we don't increment the index
				nodes[nodeIndex].hasBeenReached = true;
				continue;
			}
		}
		else if (dynamic_cast<Call*>(nodes[nodeIndex].node))
		{
			Call* call = dynamic_cast<Call*>(nodes[nodeIndex].node);
			nodes[nodeIndex].hasBeenReached = true;
			callstack.push(++nodeIndex);
			nodeIndex = GetNodeIndexFromLine(call, call->Line()->Token().GetValueInt());

			//We want to run continue to make sure that we don't increment the index

			continue;;
		}
		else if (SetPop* sp = dynamic_cast<SetPop*>(nodes[nodeIndex].node))
		{
			std::string name = sp->Name().GetLexeme();

			//Check that there is a value on the stack
			if (stackSize == 0) Error(SPL_SETPOP_STACK_EMPTY , *sp, ErrorMessages[SPL_SETPOP_STACK_EMPTY], filename);

			stackSize--;
			variables[name] = Variable(sp->IsMutable());
		}
		else if (dynamic_cast<Ret*>(nodes[nodeIndex].node))
		{
			Ret* ret = dynamic_cast<Ret*>(nodes[nodeIndex].node);

			if (callstack.empty()) Error(SPL_RET_EMPTY , *ret, ErrorMessages[SPL_RET_EMPTY], filename);

			nodes[nodeIndex].hasBeenReached = true;
			nodeIndex = callstack.top();
			callstack.pop();
			continue;
		}

		//Step one, we want to set this node as visited
		nodes[nodeIndex].hasBeenReached = true;

		//Make sure we move to the next node, so we avoid an infinite loop
		nodeIndex++;
	}

	//Before we end, we need to report all unreached code
	for (SemanticNode sn : nodes)
	{
		if (!sn.hasBeenReached)
		{
			Warning(WSPL_UNREACHABLE, *sn.node, ErrorMessages[WSPL_UNREACHABLE], filename);
			nodesToCull.push_back(sn.node);
		}
	}

	SemanticAnalyserResults results;
	results.nodesToCull = nodesToCull;
	return results;
}

SPL::Compiler::Analyser::SemanticAnalyser::SemanticAnalyser(std::vector<Parser::Nodes::Node*> nodes)
{
	this->nodes = std::vector<SemanticNode>();
	for (Node* n : nodes) this->nodes.push_back(n);

	variables = std::unordered_map<std::string, Variable>();
	callstack = std::stack<int>();
}