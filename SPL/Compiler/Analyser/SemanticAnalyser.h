#pragma once
#include <stack>
#include <vector>
#include <unordered_map>

#include "../../../Stack.h"
#include "../../../SPLErrors.h"
#include "../../../ErrorStream.h"
#include "../Parsing/Nodes/Let.h"
#include "../Parsing/Nodes/Node.h"
#include "../Parsing/Nodes/Free.h"
#include "../Parsing/Nodes/Block.h"
#include "../Parsing/Nodes/Maths/Add.h"
#include "../Parsing/Nodes/Maths/Sub.h"
#include "../Parsing/Nodes/Maths/Mul.h"
#include "../Parsing/Nodes/Maths/Div.h"
#include "../Parsing/Nodes/Maths/Pow.h"
#include "../Parsing/Nodes/Stack/Pop.h"
#include "../Parsing/Nodes/Stack/Push.h"
#include "../Parsing/Nodes/Maths/Concat.h"
#include "../Parsing/Nodes/Output/Print.h"
#include "../Parsing/Nodes/Stack/SetPop.h"
#include "../Parsing/Nodes/Casting/ToInt.h"
#include "../Parsing/Nodes/ControlFlow/Ret.h"
#include "../Parsing/Nodes/Casting/ToFloat.h"
#include "../Parsing/Nodes/Casting/ToString.h"
#include "../Parsing/Nodes/ControlFlow/Goto.h"
#include "../Parsing/Nodes/ControlFlow/Call.h"
#include "../Parsing/Nodes/ControlFlow/Exit.h"

namespace SPL
{
	namespace Compiler
	{
		namespace Analyser
		{
			/// <summary>
			/// Struct storing nodes that can be removed from the binary
			/// </summary>
			struct SemanticAnalyserResults
			{
				std::vector<Node*> nodesToCull;
			};

			/// <summary>
			/// Struct storing the nodes to analyser, but also some extra data about the nodes that the analyser needs
			/// </summary>
			struct SemanticNode
			{
				Node* node;
				bool hasBeenReached;

				//We don't need a destructor as these nodes are used else where

				/// <summary>
				/// Create a null node
				/// </summary>
				SemanticNode()
				{
					node = nullptr;
					hasBeenReached = false;
				}

				/// <summary>
				/// Create a new node based off of a normal node
				/// </summary>
				/// <param name="node">The node to store</param>
				SemanticNode(Node* node)
				{
					this->node = node;
					hasBeenReached = false;
				}

				/// <summary>
				/// Create a shallow copy of a node
				/// </summary>
				/// <param name="sn">The node to copy</param>
				SemanticNode(const SemanticNode& sn)
				{
					node = sn.node;
					hasBeenReached = sn.hasBeenReached;
				}

				/// <summary>
				/// Create a new node
				/// </summary>
				/// <param name="node">The node to base this node on</param>
				/// <param name="hasBeenReached">Has this node been reached</param>
				SemanticNode(Node* node, bool hasBeenReached)
				{
					this->node = node;
					this->hasBeenReached = hasBeenReached;
				}
			};

			/// <summary>
			/// Struct representing a variable
			/// </summary>
			struct Variable
			{
				bool isMutable;
				bool hasBeenReferenced;

				/// <summary>
				/// Create a blank variable
				/// </summary>
				Variable()
				{
					isMutable = false;
					hasBeenReferenced = false;
				}

				/// <summary>
				/// Create a shallow copy of a variable
				/// </summary>
				/// <param name="v">The variable to copy</param>
				Variable(const Variable& v)
				{
					isMutable = v.isMutable;
					hasBeenReferenced = v.hasBeenReferenced;
				}

				/// <summary>
				/// Create a new variable
				/// </summary>
				/// <param name="isMutable">The variables access modifier</param>
				/// <param name="hasBeenReferenced">Has this node been referenced</param>
				Variable(bool isMutable, bool hasBeenReferenced = false)
				{
					this->isMutable = isMutable;
					this->hasBeenReferenced = hasBeenReferenced;
				}
			};

			/// <summary>
			/// Class responsible for doing a final check on compiled code, to make sure it follows rules and has no errors
			/// </summary>
			class SemanticAnalyser final
			{
			private:
				std::unordered_map <std::string, Variable> variables;
				std::vector<SemanticNode> nodes;
				std::stack<int> callstack;
				int stackSize;

				/// <summary>
				/// Get the index of a node, based on what line to look for
				/// </summary>
				/// <param name="nodeRequesting">The node requesting the lookup</param>
				/// <param name="line">The line to look for</param>
				/// <returns>The index of the node found</returns>
				int GetNodeIndexFromLine(Node* nodeRequesting, int line);

			public:
				/// <summary>
				/// Analyse the nodes that have been loaded
				/// </summary>
				SemanticAnalyserResults Analyse();

				/// <summary>
				/// Create a new analyser
				/// </summary>
				/// <param name="nodes"></param>
				SemanticAnalyser(std::vector<Parser::Nodes::Node*> nodes);
			};
		}
	}
}