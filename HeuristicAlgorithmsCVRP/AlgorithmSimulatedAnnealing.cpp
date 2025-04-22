#include "AlgorithmSimulatedAnnealing.h"
#include "AlgorithmGreedy.h"
#include "AlgorithmClarkeWright.h"
#include "AlgorithmRandomClients.h"

AlgorithmSimulatedAnnealing::AlgorithmSimulatedAnnealing() : Algorithm() {}

void AlgorithmSimulatedAnnealing::setExperiment(const Experiment& experiment) {
	this->experiment = experiment;
}

void AlgorithmSimulatedAnnealing::solveStartingWithClarkeWrightAlgorithm() {
	// U¿ycie AlgorithmGreedy do wygenerowania pocz¹tkowego rozwi¹zania
	AlgorithmClarkeWright algorithmClarkeWright;
	algorithmClarkeWright.set(nodes, trucks, depotIndex);
	algorithmClarkeWright.solve();
	std::vector<Truck> currentSolution = algorithmClarkeWright.getTrucks();


	// Implementacja algorytmu symulowanego wy¿arzania
	double initialTemperature = 1000.0;
	double coolingRate = 0.995;
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
	// U¿ycie AlgorithmGreedy do wygenerowania pocz¹tkowego rozwi¹zania
	AlgorithmRandomClients algorithmRandomClients;
	algorithmRandomClients.set(nodes, trucks, depotIndex);
	algorithmRandomClients.solve();
	std::vector<Truck> currentSolution = algorithmRandomClients.getTrucks();

	// Implementacja algorytmu symulowanego wy¿arzania
	double initialTemperature = 1000.0;
	double coolingRate = 0.995;
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
	// U¿ycie AlgorithmGreedy do wygenerowania pocz¹tkowego rozwi¹zania
	AlgorithmGreedy greedyAlgorithm;
	greedyAlgorithm.set(nodes, trucks, depotIndex);
	greedyAlgorithm.solve();
	std::vector<Truck> currentSolution = greedyAlgorithm.getTrucks();
	
	// Implementacja algorytmu symulowanego wy¿arzania
	double initialTemperature = 1000.0;
	double coolingRate = 0.995;
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

	/*
	// Wybierz losow¹ ciê¿arówkê
	int truckIndex = rand() % newSolution.size();

	// SprawdŸ, czy trasa ciê¿arówki ma wystarczaj¹c¹ liczbê punktów (co najmniej 3)
	if (newSolution[truckIndex].getRoute().size() > 2) {
		// Wybierz dwa ró¿ne indeksy w zakresie od 1 do size - 2
		int nodeIndex1 = (rand() % (newSolution[truckIndex].getRoute().size() - 2)) + 1;
		int nodeIndex2 = (rand() % (newSolution[truckIndex].getRoute().size() - 2)) + 1;

		// Upewnij siê, ¿e indeksy s¹ ró¿ne
		while (nodeIndex1 == nodeIndex2) {
			nodeIndex2 = (rand() % (newSolution[truckIndex].getRoute().size() - 2)) + 1;
		}

		// Zamieñ wylosowane punkty w trasie
		newSolution[truckIndex].swapRoute(nodeIndex1, nodeIndex2);
	}*/

	//tutaj dodaæ zamienianie dwóch punktów w ró¿nych ciê¿arówkach
	int truckIndex1 = rand() % newSolution.size();
	int truckIndex2 = rand() % newSolution.size();
	while (truckIndex1 == truckIndex2) {
		truckIndex2 = rand() % newSolution.size();
	}

	int nodeIndex1 = (rand() % (newSolution[truckIndex1].getRoute().size() - 2)) + 1;
	int nodeIndex2 = (rand() % (newSolution[truckIndex2].getRoute().size() - 2)) + 1;

	if (newSolution[truckIndex1].getRoute().size() <= 2 || newSolution[truckIndex2].getRoute().size() <= 2) {
		return newSolution; // Nie mo¿na zamieniæ punktów
	}

	if (newSolution[truckIndex1].getLoad() -
		experiment.getNodeByCoordinates(newSolution[truckIndex2].getRoute()[nodeIndex2].x, newSolution[truckIndex2].getRoute()[nodeIndex2].y).getDemand() +
		experiment.getNodeByCoordinates(newSolution[truckIndex1].getRoute()[nodeIndex1].x, newSolution[truckIndex1].getRoute()[nodeIndex1].y).getDemand() > 0
		&&
		newSolution[truckIndex2].getLoad() -
		experiment.getNodeByCoordinates(newSolution[truckIndex1].getRoute()[nodeIndex1].x, newSolution[truckIndex1].getRoute()[nodeIndex1].y).getDemand() +
		experiment.getNodeByCoordinates(newSolution[truckIndex2].getRoute()[nodeIndex2].x, newSolution[truckIndex2].getRoute()[nodeIndex2].y).getDemand() > 0)
	{
		std::vector<ImVec2> tempRoute1 = newSolution[truckIndex1].getRoute();
		std::vector<ImVec2> tempRoute2 = newSolution[truckIndex2].getRoute();
		std::swap(tempRoute1[nodeIndex1], tempRoute2[nodeIndex2]);
		newSolution[truckIndex1].setRoute(tempRoute1);
		newSolution[truckIndex2].setRoute(tempRoute2);
	}


	return newSolution;
}

double AlgorithmSimulatedAnnealing::acceptanceProbability(double bestCost, double newCost, double temperature) {
	if (newCost < bestCost) {
		return 1.0;
	}
	return exp((bestCost - newCost) / temperature);
}