#include "Algorithm.h"
#include <cmath>
#include <algorithm>

void Algorithm::set(const std::vector<Node> nodes, const std::vector<Truck> trucks) {
	this->nodes = nodes;
	this->trucks = trucks;
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

    // Pêtla g³ówna - dopóki znajdujemy lepsze rozwi¹zanie
    while (improvement) {
        improvement = false;

        // Iteracja po ka¿dej ciê¿arówce
        for (auto& truck : trucks) {
            int routeSize = truck.getRoute().size();

            // Sprawdzenie, czy trasa ma wystarczaj¹c¹ liczbê punktów do analizy
            if (routeSize < 4) { // Minimum 4 punkty, aby zamiana dwóch krawêdzi mia³a sens
                continue;
            }

            // Iteracja po wszystkich mo¿liwych parach krawêdzi
            for (int i = 1; i < routeSize - 2; i++) {
                for (int j = i + 1; j < routeSize - 1; j++) {
                    // Oblicz d³ugoœæ trasy przed zamian¹
                    double oldDistance = truck.getRouteLength();

                    // Zamiana dwóch krawêdzi
                    truck.swapRoute(i, j);

                    // Oblicz d³ugoœæ trasy po zamianie
                    double newDistance = truck.getRouteLength();

                    // Jeœli nowa trasa jest lepsza, zachowaj zmiany
                    if (newDistance < oldDistance) {
                        sumOfallRoutes -= oldDistance;
                        sumOfallRoutes += newDistance;
                        improvement = true;
                    }
                    else {
                        // Przywróæ oryginaln¹ trasê, jeœli zmiana nie jest korzystna
                        truck.swapRoute(i, j);
                    }
                }
            }
        }
    }
}