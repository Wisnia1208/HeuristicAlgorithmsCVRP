#pragma once
#include "Algorithm.h"
class AlgorithmTabuSearch :
    public Algorithm
{
public:
	AlgorithmTabuSearch();
	double calculateFitness(const std::vector<Truck>& trucks);
	std::vector<Truck> generateNeighbor(const std::vector<Truck>& currentSolution);
	void solveStartingWithRandomClientAlgorithm();
	void solveStartingWithGreedyAlgorithm();
	void solveStartingWithClarkeWrightAlgorithm();
};

