#include "Algorithm.h"
#include <cmath>
#include <algorithm>

void Algorithm::set(const std::vector<Node> nodes, const std::vector<Truck> trucks, int depotIndex) {
	this->nodes = nodes;
	this->trucks = trucks;
	this->depotIndex = depotIndex;
}

double Algorithm::getSumOfRoutes() const {
	return sumOfallRoutes;
}

std::vector<Truck> Algorithm::getTrucks() const {
	return trucks;
}

void Algorithm::setTrucks(const std::vector<Truck> trucks) {
	this->trucks = trucks;
}

double Algorithm::calculateDistance(const ImVec2 firstPoint, const ImVec2 secondPoint) const {
	return std::sqrt(std::pow(firstPoint.x - secondPoint.x, 2) + std::pow(firstPoint.y - secondPoint.y, 2));
}

void Algorithm::twoOpt() {
    bool improvement = true;

    // P�tla g��wna - dop�ki znajdujemy lepsze rozwi�zanie
    while (improvement) {
        improvement = false;

        // Iteracja po ka�dej ci�ar�wce
        for (auto& truck : trucks) {
            int routeSize = truck.getRoute().size();

            // Sprawdzenie, czy trasa ma wystarczaj�c� liczb� punkt�w do analizy
            if (routeSize < 4) { // Minimum 4 punkty, aby zamiana dw�ch kraw�dzi mia�a sens
                continue;
            }

            // Iteracja po wszystkich mo�liwych parach kraw�dzi
            for (int i = 1; i < routeSize - 2; i++) {
                for (int j = i + 1; j < routeSize - 1; j++) {
                    // Oblicz d�ugo�� trasy przed zamian�
                    double oldDistance = truck.getRouteLength();

                    // Zamiana dw�ch kraw�dzi
                    truck.swapRoute(i, j);

                    // Oblicz d�ugo�� trasy po zamianie
                    double newDistance = truck.getRouteLength();

                    // Sprawd�, czy nowa trasa spe�nia ograniczenie �adunku
                    int totalDemand = 0;
                    for (const auto& point : truck.getRoute()) {
                        // Znajd� zapotrzebowanie punktu w `nodes`
                        auto it = std::find_if(nodes.begin(), nodes.end(), [&](const Node& node) {
                            return node.getCoordinates().x == point.x && node.getCoordinates().y == point.y;
                            });

                        if (it != nodes.end()) {
                            totalDemand += it->getDemand();
                        }
                    }

                    // Je�li nowa trasa jest lepsza i spe�nia ograniczenie �adunku, zachowaj zmiany
                    if (newDistance < oldDistance && totalDemand <= truck.getLoad()) {
                        sumOfallRoutes -= oldDistance;
                        sumOfallRoutes += newDistance;
                        improvement = true;
                    }
                    else {
                        // Przywr�� oryginaln� tras�, je�li zmiana nie jest korzystna lub przekracza �adunek
                        truck.swapRoute(i, j);
                    }
                }
            }
        }
    }
}
