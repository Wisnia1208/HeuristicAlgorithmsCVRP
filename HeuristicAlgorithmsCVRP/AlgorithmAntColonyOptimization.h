#pragma once
#include "Algorithm.h"
class AlgorithmAntColonyOptimization :
    public Algorithm
{
public:
	AlgorithmAntColonyOptimization();
	void solve();
private:
	int findNodeIndex(const ImVec2& coordinates) const;
	double calculateSolutionLength(const std::vector<Truck>& solution) const;
	Node selectNextNode(
		const Node& currentNode,
		const std::vector<Node>& unvisitedNodes,
		const std::vector<std::vector<double>>& pheromones,
		double alpha,
		double beta
	) const;
	Node findNodeByCoordinates(const ImVec2& coordinates) const;
};

