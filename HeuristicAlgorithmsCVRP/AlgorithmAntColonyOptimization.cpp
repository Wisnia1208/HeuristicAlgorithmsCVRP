#include "AlgorithmAntColonyOptimization.h"

AlgorithmAntColonyOptimization::AlgorithmAntColonyOptimization() {}

void AlgorithmAntColonyOptimization::solve() {
    // Parametry algorytmu ACO domy�lne
    const int numAnts = 50; // Liczba mr�wek
    const int numIterations = 100; // Liczba iteracji
    const double alpha = 1.0; // Waga feromon�w
    const double beta = 2.0; // Waga heurystyki
    const double evaporationRate = 0.5; // Wsp�czynnik parowania feromon�w
    const double Q = 100.0; // Sta�a do aktualizacji feromon�w

	// Parametry algorytmu ACO eksploracja
    /*const int numAnts = nodes.size();
    const int numIterations = 200;
    const double alpha = 1.0;
    const double beta = 3.0;
    const double evaporationRate = 0.7;
    const double Q = 50.0;*/

	//Parametry algorytmu ACO eksploatacja
    /*const int numAnts = nodes.size() / 2;
    const int numIterations = 100;
    const double alpha = 2.0;
    const double beta = 1.0;
    const double evaporationRate = 0.3;
    const double Q = 200.0;*/

    // Inicjalizacja feromon�w
    int numNodes = nodes.size();
    std::vector<std::vector<double>> pheromones(numNodes, std::vector<double>(numNodes, 1.0));

    // Najlepsze rozwi�zanie
    double bestRouteLength = std::numeric_limits<double>::max();
    std::vector<Truck> bestSolution;

    for (int iteration = 0; iteration < numIterations; ++iteration) {
        // Tworzenie tras dla ka�dej mr�wki
        std::vector<std::vector<Truck>> antSolutions(numAnts);

        for (int ant = 0; ant < numAnts; ++ant) {
            // Inicjalizacja tras dla mr�wki
            std::vector<Truck> trucks = this->trucks;

            // Ustawienie depotu jako pierwszego punktu trasy dla ka�dej ci�ar�wki
            for (auto& truck : trucks) {
                truck.addToRoute(nodes[depotIndex].getCoordinates());
            }

            // Lista nieodwiedzonych w�z��w
            std::vector<Node> unvisitedNodes = nodes;
            unvisitedNodes.erase(unvisitedNodes.begin() + depotIndex); // Usu� depot z listy

            // Budowanie trasy dla ka�dej ci�ar�wki
            for (auto& truck : trucks) {
                while (!unvisitedNodes.empty()) {
                    Node currentNode = findNodeByCoordinates(truck.getRoute().back());
                    Node nextNode = selectNextNode(currentNode, unvisitedNodes, pheromones, alpha, beta);

                    // Sprawd�, czy ci�ar�wka mo�e obs�u�y� zapotrzebowanie
                    if (truck.getLoad() >= nextNode.getDemand()) {
                        truck.addToRoute(nextNode.getCoordinates());
                        truck.setLoad(truck.getLoad() - nextNode.getDemand());
                        unvisitedNodes.erase(std::remove(unvisitedNodes.begin(), unvisitedNodes.end(), nextNode), unvisitedNodes.end());
                    }
                    else {
                        // Powr�t do depo i rozpocz�cie nowej trasy
                        truck.addToRoute(nodes[depotIndex].getCoordinates());
                        break;
                    }
                }

                // Powr�t do depo, je�li ci�ar�wka zako�czy�a tras�
                if (!truck.getRoute().empty() && truck.getRoute().back() != nodes[depotIndex].getCoordinates()) {
                    truck.addToRoute(nodes[depotIndex].getCoordinates());
                }
            }

            // Obs�uga pozosta�ych w�z��w
            for (const auto& node : unvisitedNodes) {
                for (auto& truck : trucks) {
                    if (truck.getLoad() >= node.getDemand()) {
                        truck.addToRoute(node.getCoordinates());
                        truck.setLoad(truck.getLoad() - node.getDemand());
                        break;
                    }
                }
            }

            antSolutions[ant] = trucks;
        }

        // Aktualizacja feromon�w
        for (auto& row : pheromones) {
            for (auto& value : row) {
                value *= (1.0 - evaporationRate); // Parowanie feromon�w
            }
        }

        for (const auto& solution : antSolutions) {
            double routeLength = calculateSolutionLength(solution);
            if (routeLength < bestRouteLength) {
                bestRouteLength = routeLength;
                bestSolution = solution;
            }

            // Aktualizacja feromon�w na podstawie rozwi�zania
            for (const auto& truck : solution) {
                const auto& route = truck.getRoute();
                for (size_t i = 0; i < route.size() - 1; ++i) {
                    int from = findNodeIndex(route[i]);
                    int to = findNodeIndex(route[i + 1]);
                    pheromones[from][to] += Q / routeLength;
                    pheromones[to][from] += Q / routeLength; // Symetryczna aktualizacja
                }
            }
        }
    }

    // Ustaw najlepsze rozwi�zanie
    setTrucks(bestSolution);
    sumOfallRoutes = bestRouteLength;
}

int AlgorithmAntColonyOptimization::findNodeIndex(const ImVec2& coordinates) const {
    for (size_t i = 0; i < nodes.size(); ++i) {
        if (nodes[i].getCoordinates().x == coordinates.x && nodes[i].getCoordinates().y == coordinates.y) {
            return static_cast<int>(i);
        }
    }
    return -1; // Zwr�� -1, je�li w�ze� nie zosta� znaleziony
}

double AlgorithmAntColonyOptimization::calculateSolutionLength(const std::vector<Truck>& solution) const {
    double totalLength = 0.0;
    for (const auto& truck : solution) {
        totalLength += truck.getRouteLength();
    }
    return totalLength;
}

Node AlgorithmAntColonyOptimization::selectNextNode(
    const Node& currentNode,
    const std::vector<Node>& unvisitedNodes,
    const std::vector<std::vector<double>>& pheromones,
    double alpha,
    double beta
) const {
    std::vector<double> probabilities;
    double totalProbability = 0.0;

    int currentIndex = findNodeIndex(currentNode.getCoordinates());

    // Oblicz prawdopodobie�stwa dla ka�dego nieodwiedzonego w�z�a
    for (const auto& node : unvisitedNodes) {
        int nextIndex = findNodeIndex(node.getCoordinates());
        double pheromone = pheromones[currentIndex][nextIndex];
        double heuristic = 1.0 / calculateDistance(currentNode.getCoordinates(), node.getCoordinates());
        double probability = std::pow(pheromone, alpha) * std::pow(heuristic, beta);
        probabilities.push_back(probability);
        totalProbability += probability;
    }

    // Normalizacja prawdopodobie�stw
    for (auto& probability : probabilities) {
        probability /= totalProbability;
    }

    // Wyb�r nast�pnego w�z�a na podstawie prawdopodobie�stwa
    double randomValue = static_cast<double>(rand()) / RAND_MAX;
    double cumulativeProbability = 0.0;

    for (size_t i = 0; i < unvisitedNodes.size(); ++i) {
        cumulativeProbability += probabilities[i];
        if (randomValue <= cumulativeProbability) {
            return unvisitedNodes[i];
        }
    }

    // W razie problem�w zwr�� pierwszy w�ze� (powinno by� rzadkie)
    return unvisitedNodes.front();
}

Node AlgorithmAntColonyOptimization::findNodeByCoordinates(const ImVec2& coordinates) const {
    for (const auto& node : nodes) {
        if (node.getCoordinates().x == coordinates.x && node.getCoordinates().y == coordinates.y) {
            return node;
        }
    }
    throw std::runtime_error("Node with given coordinates not found");
}