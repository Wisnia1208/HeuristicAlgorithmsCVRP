#pragma once
#include "Algorithm.h"
class AlgorithmClarkeWright :
    public Algorithm
{
public:
	AlgorithmClarkeWright();
	void solve();
	int calculateRouteDemand(const std::vector<Node> route);
	double calculateRouteLength(const std::vector<Node> route);
	std::vector<std::vector<double>> calculateSavingsMatrix();
};

