#include "AlgorithmSimulatedAnnealing.h"
#include "AlgorithmGreedy.h"
#include "AlgorithmClarkeWright.h"
#include "AlgorithmRandomClients.h"

AlgorithmSimulatedAnnealing::AlgorithmSimulatedAnnealing() : Algorithm() {}

void AlgorithmSimulatedAnnealing::solveStartingWithClarkeWrightAlgorithm() {
	// U�ycie AlgorithmGreedy do wygenerowania pocz�tkowego rozwi�zania
	AlgorithmClarkeWright algorithmClarkeWright;
	algorithmClarkeWright.set(nodes, trucks, depotIndex);
	algorithmClarkeWright.solve();
	std::vector<Truck> currentSolution = algorithmClarkeWright.getTrucks();


	// Implementacja algorytmu symulowanego wy�arzania
	double initialTemperature = 10000.0;
	double coolingRate = 0.999;
	double temperature = initialTemperature;
	std::vector<Truck> bestSolution = currentSolution;
	double bestCost = calculateFitness(bestSolution);
	while (temperature > 1) {
		std::vector<Truck> newSolution = generateNeighbor(currentSolution);
		double newCost = calculateFitness(newSolution);
		if (acceptanceProbability(bestCost, newCost, temperature) > static_cast<double>(rand()) / RAND_MAX) {
			currentSolution = newSolution;
			if (newCost < bestCost) {
				bestSolution = newSolution;
				bestCost = newCost;
			}
		}
		temperature *= coolingRate;
	}
	setTrucks(bestSolution);
	sumOfallRoutes = bestCost;
}

void AlgorithmSimulatedAnnealing::solveStartingWithRandomClientsAlgorithm() {
	// U�ycie AlgorithmGreedy do wygenerowania pocz�tkowego rozwi�zania
	AlgorithmRandomClients algorithmRandomClients;
	algorithmRandomClients.set(nodes, trucks, depotIndex);
	algorithmRandomClients.solve();
	std::vector<Truck> currentSolution = algorithmRandomClients.getTrucks();

	// Implementacja algorytmu symulowanego wy�arzania
	double initialTemperature = 10000.0;
	double coolingRate = 0.999;
	double temperature = initialTemperature;
	std::vector<Truck> bestSolution = currentSolution;
	double bestCost = calculateFitness(bestSolution);
	while (temperature > 1) {
		std::vector<Truck> newSolution = generateNeighbor(currentSolution);
		double newCost = calculateFitness(newSolution);
		if (acceptanceProbability(bestCost, newCost, temperature) > static_cast<double>(rand()) / RAND_MAX) {
			currentSolution = newSolution;
			if (newCost < bestCost) {
				bestSolution = newSolution;
				bestCost = newCost;
			}
		}
		temperature *= coolingRate;
	}
	setTrucks(bestSolution);
	sumOfallRoutes = bestCost;
}

void AlgorithmSimulatedAnnealing::solveStartingWithGreedyAlgorithm() {
	// U�ycie AlgorithmGreedy do wygenerowania pocz�tkowego rozwi�zania
	AlgorithmGreedy greedyAlgorithm;
	greedyAlgorithm.set(nodes, trucks, depotIndex);
	greedyAlgorithm.solve();
	std::vector<Truck> currentSolution = greedyAlgorithm.getTrucks();

	
	// Implementacja algorytmu symulowanego wy�arzania
	double initialTemperature = 10000.0;
	double coolingRate = 0.999;
	double temperature = initialTemperature;
	std::vector<Truck> bestSolution = currentSolution;
	double bestCost = calculateFitness(bestSolution);
	while (temperature > 1) {
		std::vector<Truck> newSolution = generateNeighbor(currentSolution);
		double newCost = calculateFitness(newSolution);
		if (acceptanceProbability(bestCost, newCost, temperature) > static_cast<double>(rand()) / RAND_MAX) {
			currentSolution = newSolution;
			if (newCost < bestCost) {
				bestSolution = newSolution;
				bestCost = newCost;
			}
		}
		temperature *= coolingRate;
	}
	setTrucks(bestSolution);
	sumOfallRoutes = bestCost;
}

double AlgorithmSimulatedAnnealing::calculateFitness(const std::vector<Truck>& trucks) {
	double totalDistance = 0.0;
	for (const auto& truck : trucks) {
		totalDistance += truck.getRouteLength();
	}
	return totalDistance;
}

std::vector<Truck> AlgorithmSimulatedAnnealing::generateNeighbor(const std::vector<Truck>& currentSolution) {
	std::vector<Truck> newSolution = currentSolution;

	// Wybierz losow� ci�ar�wk�
	int truckIndex = rand() % newSolution.size();

	// Sprawd�, czy trasa ci�ar�wki ma wystarczaj�c� liczb� punkt�w (co najmniej 3)
	if (newSolution[truckIndex].getRoute().size() > 2) {
		// Wybierz dwa r�ne indeksy w zakresie od 1 do size - 2
		int nodeIndex1 = (rand() % (newSolution[truckIndex].getRoute().size() - 2)) + 1;
		int nodeIndex2 = (rand() % (newSolution[truckIndex].getRoute().size() - 2)) + 1;

		// Upewnij si�, �e indeksy s� r�ne
		while (nodeIndex1 == nodeIndex2) {
			nodeIndex2 = (rand() % (newSolution[truckIndex].getRoute().size() - 2)) + 1;
		}

		// Zamie� wylosowane punkty w trasie
		newSolution[truckIndex].swapRoute(nodeIndex1, nodeIndex2);
	}

	return newSolution;
}

double AlgorithmSimulatedAnnealing::acceptanceProbability(double bestCost, double newCost, double temperature) {
	if (newCost < bestCost) {
		return 1.0;
	}
	return exp((bestCost - newCost) / temperature);
}