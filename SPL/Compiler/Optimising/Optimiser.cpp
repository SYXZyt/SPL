#include "Optimiser.h"
#include <iostream>
std::vector<Node*>& SPL::Compiler::Optimising::Optimiser::Optimise(std::vector<Node*>& nodes, Analyser::SemanticAnalyserResults results)
{
	for (int i = 0; i < nodes.size(); i++)
	{
		for (Node* n : results.nodesToCull)
		{
			if (nodes[i] == n)
			{
				nodes.erase(nodes.begin() + i);
			}
		}
	}

	return nodes;
}