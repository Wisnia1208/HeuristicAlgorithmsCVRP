#pragma once
#include "Algorithm.h"
class AlgorithmClarkeWright :
    public Algorithm
{
public:
	AlgorithmClarkeWright();
	void solve();
	int calculateRouteDemand(const std::vector<Node> route);
	int calculateRouteDemand(const std::vector<ImVec2> route);
	double calculateRouteLength(const std::vector<Node> route);
	std::vector<std::vector<double>> calculateSavingsMatrix();
};

