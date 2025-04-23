#include "AlgorithmTabuSearch.h"
#include "AlgorithmGreedy.h"
#include "AlgorithmClarkeWright.h"
#include "AlgorithmRandomClients.h"

AlgorithmTabuSearch::AlgorithmTabuSearch() : Algorithm() {}

#include <unordered_set> // std::unordered_set

void AlgorithmTabuSearch::solveStartingWithRandomClientAlgorithm() {
	// U�ycie AlgorithmRandomClients do wygenerowania pocz�tkowego rozwi�zania
	AlgorithmRandomClients algorithmRandom;
	algorithmRandom.set(nodes, trucks, depotIndex);
	algorithmRandom.solve();
	std::vector<Truck> currentSolution = algorithmRandom.getTrucks();

	// Implementacja algorytmu tabu search
	int maxIterations = 1000;
	int tabuTenure = 10;
	std::unordered_set<std::vector<Truck>> tabuList;
	std::vector<Truck> bestSolution = currentSolution;
	double bestCost = calculateFitness(bestSolution);

	for (int iteration = 0; iteration < maxIterations; ++iteration) {
		std::vector<Truck> newSolution = generateNeighbor(currentSolution);
		double newCost = calculateFitness(newSolution);

		// Sprawd�, czy rozwi�zanie jest w li�cie tabu
		if (tabuList.find(newSolution) == tabuList.end()) {
			currentSolution = newSolution;

			// Aktualizuj najlepsze rozwi�zanie
			if (newCost < bestCost) {
				bestSolution = newSolution;
				bestCost = newCost;
			}

			// Dodaj rozwi�zanie do listy tabu
			tabuList.insert(newSolution);

			// Usu� najstarsze rozwi�zanie, je�li lista tabu przekracza rozmiar
			if (tabuList.size() > tabuTenure) {
				tabuList.erase(tabuList.begin());
			}
		}
	}

	setTrucks(bestSolution);
	sumOfallRoutes = bestCost;
}

void AlgorithmTabuSearch::solveStartingWithGreedyAlgorithm() {
	// U�ycie AlgorithmGreedy do wygenerowania pocz�tkowego rozwi�zania
	AlgorithmGreedy algorithmGreedy;
	algorithmGreedy.set(nodes, trucks, depotIndex);
	algorithmGreedy.solve();
	std::vector<Truck> currentSolution = algorithmGreedy.getTrucks();
	// Implementacja algorytmu tabu search
	int maxIterations = 1000;
	int tabuTenure = 10;
	std::unordered_set<std::vector<Truck>> tabuList;
	std::vector<Truck> bestSolution = currentSolution;
	double bestCost = calculateFitness(bestSolution);
	for (int iteration = 0; iteration < maxIterations; ++iteration) {
		std::vector<Truck> newSolution = generateNeighbor(currentSolution);
		double newCost = calculateFitness(newSolution);
		// Sprawd�, czy rozwi�zanie jest w li�cie tabu
		if (tabuList.find(newSolution) == tabuList.end()) {
			currentSolution = newSolution;
			// Aktualizuj najlepsze rozwi�zanie
			if (newCost < bestCost) {
				bestSolution = newSolution;
				bestCost = newCost;
			}
			// Dodaj rozwi�zanie do listy tabu
			tabuList.insert(newSolution);
			// Usu� najstarsze rozwi�zanie, je�li lista tabu przekracza rozmiar
			if (tabuList.size() > tabuTenure) {
				tabuList.erase(tabuList.begin());
			}
		}
	}
	setTrucks(bestSolution);
	sumOfallRoutes = bestCost;
}

void AlgorithmTabuSearch::solveStartingWithClarkeWrightAlgorithm() {
	// U�ycie AlgorithmClarkeWright do wygenerowania pocz�tkowego rozwi�zania
	AlgorithmClarkeWright algorithmClarkeWright;
	algorithmClarkeWright.set(nodes, trucks, depotIndex);
	algorithmClarkeWright.solve();
	std::vector<Truck> currentSolution = algorithmClarkeWright.getTrucks();
	// Implementacja algorytmu tabu search
	int maxIterations = 1000;
	int tabuTenure = 10;
	std::unordered_set<std::vector<Truck>> tabuList;
	std::vector<Truck> bestSolution = currentSolution;
	double bestCost = calculateFitness(bestSolution);
	for (int iteration = 0; iteration < maxIterations; ++iteration) {
		std::vector<Truck> newSolution = generateNeighbor(currentSolution);
		double newCost = calculateFitness(newSolution);
		// Sprawd�, czy rozwi�zanie jest w li�cie tabu
		if (tabuList.find(newSolution) == tabuList.end()) {
			currentSolution = newSolution;
			// Aktualizuj najlepsze rozwi�zanie
			if (newCost < bestCost) {
				bestSolution = newSolution;
				bestCost = newCost;
			}
			// Dodaj rozwi�zanie do listy tabu
			tabuList.insert(newSolution);
			// Usu� najstarsze rozwi�zanie, je�li lista tabu przekracza rozmiar
			if (tabuList.size() > tabuTenure) {
				tabuList.erase(tabuList.begin());
			}
		}
	}
	setTrucks(bestSolution);
	sumOfallRoutes = bestCost;
}

double AlgorithmTabuSearch::calculateFitness(const std::vector<Truck>& trucks) {
	double totalDistance = 0.0;
	for (const auto& truck : trucks) {
		totalDistance += truck.getRouteLength();
	}
	return totalDistance;
}

std::vector<Truck> AlgorithmTabuSearch::generateNeighbor(const std::vector<Truck>& currentSolution) {
	std::vector<Truck> newSolution = currentSolution;
	std::vector<Truck> betterSolution = newSolution;

	for (int i = 0; i < rand() % 100; i++)
	{
		int approach = rand() % 5; // Wybierz losowe podej�cie do generowania s�siada

		switch (approach) {
		case 0: {
			// zamie� dwa punkty w tej samej ci�ar�wce
			// Wybierz losow� ci�ar�wk�
			int truckIndex = rand() % newSolution.size();

			// Sprawd�, czy trasa ci�ar�wki ma wystarczaj�c� liczb� punkt�w (co najmniej 4)
			if (newSolution[truckIndex].getRoute().size() > 3) {
				// Wybierz dwa r�ne indeksy w zakresie od 1 do size - 2
				int nodeIndex1 = (rand() % (newSolution[truckIndex].getRoute().size() - 2)) + 1;
				int nodeIndex2 = (rand() % (newSolution[truckIndex].getRoute().size() - 2)) + 1;

				// Upewnij si�, �e indeksy s� r�ne
				while (nodeIndex1 == nodeIndex2) {
					nodeIndex2 = (rand() % (newSolution[truckIndex].getRoute().size() - 2)) + 1;
				}

				// Zamie� wylosowane punkty w trasie
				newSolution[truckIndex].swapRoute(nodeIndex1, nodeIndex2);
				break;
			}
		}
		case 1: {
			int truckIndex1 = rand() % newSolution.size();
			while (newSolution[truckIndex1].getRoute().size() < 3) {
				truckIndex1 = rand() % newSolution.size();
			}
			int truckIndex2 = rand() % newSolution.size();
			while (truckIndex1 == truckIndex2 or newSolution[truckIndex2].getRoute().size() < 3) {
				truckIndex2 = rand() % newSolution.size();
			}

			int nodeIndex1 = (rand() % (newSolution[truckIndex1].getRoute().size() - 2)) + 1;
			int nodeIndex2 = (rand() % (newSolution[truckIndex2].getRoute().size() - 2)) + 1;

			if (newSolution[truckIndex1].getRoute().size() <= 2 || newSolution[truckIndex2].getRoute().size() <= 2) {
				return newSolution; // Nie mo�na zamieni� punkt�w
			}

			if (newSolution[truckIndex1].getLoad() -
				getNodeDemand(newSolution[truckIndex2].getRoute()[nodeIndex2].x, newSolution[truckIndex2].getRoute()[nodeIndex2].y) +
				getNodeDemand(newSolution[truckIndex1].getRoute()[nodeIndex1].x, newSolution[truckIndex1].getRoute()[nodeIndex1].y) > 0
				&&
				newSolution[truckIndex2].getLoad() -
				getNodeDemand(newSolution[truckIndex1].getRoute()[nodeIndex1].x, newSolution[truckIndex1].getRoute()[nodeIndex1].y) +
				getNodeDemand(newSolution[truckIndex2].getRoute()[nodeIndex2].x, newSolution[truckIndex2].getRoute()[nodeIndex2].y) > 0)
			{
				std::vector<ImVec2> tempRoute1 = newSolution[truckIndex1].getRoute();
				std::vector<ImVec2> tempRoute2 = newSolution[truckIndex2].getRoute();
				std::swap(tempRoute1[nodeIndex1], tempRoute2[nodeIndex2]);
				newSolution[truckIndex1].setLoad(newSolution[truckIndex1].getLoad() +
					getNodeDemand(tempRoute2[nodeIndex2].x, tempRoute2[nodeIndex2].y) -
					getNodeDemand(tempRoute1[nodeIndex1].x, tempRoute1[nodeIndex1].y));
				newSolution[truckIndex2].setLoad(newSolution[truckIndex2].getLoad() +
					getNodeDemand(tempRoute1[nodeIndex1].x, tempRoute1[nodeIndex1].y) -
					getNodeDemand(tempRoute2[nodeIndex2].x, tempRoute2[nodeIndex2].y));
				newSolution[truckIndex1].setRoute(tempRoute1);
				newSolution[truckIndex2].setRoute(tempRoute2);
				break;
			}
		}
		case 2: {
			//zabierz punkt z jednej trasy i wsad� go do drugiej
			// Wybierz losow� ci�ar�wk�
			int truckIndex1 = rand() % newSolution.size();

			// Sprawd�, czy trasa ci�ar�wki ma wystarczaj�c� liczb� punkt�w (co najmniej 2)
			while (newSolution[truckIndex1].getRoute().size() < 3) {
				truckIndex1 = rand() % newSolution.size();
			}
			int nodeIndex1 = (rand() % (newSolution[truckIndex1].getRoute().size() - 2)) + 1;

			int demand = getNodeDemand(newSolution[truckIndex1].getRoute()[nodeIndex1].x, newSolution[truckIndex1].getRoute()[nodeIndex1].y);

			// Zbierz wszystkie ci�ar�wki, kt�re mog� przyj�� punkt
			std::vector<int> eligibleTrucks;
			for (size_t i = 0; i < newSolution.size(); ++i) {
				if (i != truckIndex1 && newSolution[i].getLoad() >= demand) {
					eligibleTrucks.push_back(i);
				}
			}

			// Je�li nie ma �adnej ci�ar�wki, kt�ra mo�e przyj�� punkt, zwr�� niezmienione rozwi�zanie
			if (eligibleTrucks.empty()) {
				return newSolution;
			}

			int truckIndex2 = rand() % newSolution.size();
			while (truckIndex1 == truckIndex2 || newSolution[truckIndex2].getLoad() < demand) {
				truckIndex2 = rand() % newSolution.size();
			}

			int nodeIndex2 = 0; // Wska�nik na pozycj� w trasie ci�ar�wki docelowej
			// Wylosuj pozycj� w trasie ci�ar�wki docelowej
			if (newSolution[truckIndex2].getRoute().size() == 2) {
				nodeIndex2 = 1; // Je�li trasa ma tylko dwa punkty, wstaw punkt na drug� pozycj�
			}
			else {
				nodeIndex2 = (rand() % (newSolution[truckIndex2].getRoute().size() - 2)) + 1;
			}

			// Zaktualizuj �adunki ci�ar�wek
			newSolution[truckIndex1].setLoad(newSolution[truckIndex1].getLoad() + demand);
			newSolution[truckIndex2].setLoad(newSolution[truckIndex2].getLoad() - demand);

			// Przenie� punkt z jednej trasy do drugiej
			newSolution[truckIndex2].route.insert(newSolution[truckIndex2].route.begin() + nodeIndex2, newSolution[truckIndex1].getRoute()[nodeIndex1]);
			newSolution[truckIndex1].route.erase(newSolution[truckIndex1].route.begin() + nodeIndex1);
			break;
		}
		case 3: {
			//lustrzane odbicie
			// Wybierz losow� ci�ar�wk�
			int truckIndex = rand() % newSolution.size();
			// Sprawd�, czy trasa ci�ar�wki ma wystarczaj�c� liczb� punkt�w (co najmniej 4)
			while (newSolution[truckIndex].getRoute().size() < 4) {
				truckIndex = rand() % newSolution.size();
			}
			// Wybierz losowy punkt w trasie
			int nodeIndex1 = (rand() % (newSolution[truckIndex].getRoute().size() - 2)) + 1;
			int nodeIndex2 = (rand() % (newSolution[truckIndex].getRoute().size() - 2)) + 1;
			// Upewnij si�, �e indeksy s� r�ne
			while (nodeIndex1 == nodeIndex2) {
				nodeIndex2 = (rand() % (newSolution[truckIndex].getRoute().size() - 2)) + 1;
			}
			// Zamie� wylosowane punkty w trasie
			if (nodeIndex1 > nodeIndex2) {
				std::reverse(newSolution[truckIndex].route.begin() + nodeIndex2, newSolution[truckIndex].route.begin() + nodeIndex1 + 1);
			}
			break;
		}
		case 4: {
			int truckIndex = rand() % newSolution.size();
			newSolution[truckIndex].twoOpt();
			break;
		}
		default:
			break;
		}
		if (calculateFitness(newSolution) < calculateFitness(betterSolution)) {
			betterSolution = newSolution;
		}
		else {
			newSolution = betterSolution;
		}
	}

	return newSolution;
}