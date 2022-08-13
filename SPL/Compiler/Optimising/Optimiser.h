#pragma once
#include <vector>

#include "../Analyser/SemanticAnalyser.h"

namespace SPL
{
	namespace Compiler
	{
		namespace Optimising
		{
			/// <summary>
			/// Class which can apply some optimisation to preassembled SPL
			/// </summary>
			class Optimiser
			{
			public:
				/// <summary>
				/// Optimise the loaded nodes
				/// </summary>
				/// <param name="nodes">The nodes to optimise</param>
				/// <param name="results">The results from the analyser</param>
				/// <returns>Updated nodes</returns>
				static std::vector<Node*>& Optimise(std::vector<Node*>& nodes, Analyser::SemanticAnalyserResults results);
			};
		}
	}
}