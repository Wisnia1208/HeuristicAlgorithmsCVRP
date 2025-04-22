#pragma once
#include "Algorithm.h"
#include "Experiment.h"
#include "Node.h"
#include "imgui.h"
class AlgorithmSimulatedAnnealing :
    public Algorithm
{
private:
	Experiment experiment;
public:
	AlgorithmSimulatedAnnealing();
	void solveStartingWithGreedyAlgorithm();
	void solveStartingWithClarkeWrightAlgorithm();
	void solveStartingWithRandomClientsAlgorithm();
	double calculateFitness(const std::vector<Truck>& trucks);
	std::vector<Truck> generateNeighbor(const std::vector<Truck>& currentSolution);
	double acceptanceProbability(double bestCost, double newCost, double temperature);
};

