#include "AlgorithmGreedy.h"

AlgorithmGreedy::AlgorithmGreedy() {}

void AlgorithmGreedy::solve() {
    // Dodanie depozytu jako pocz�tkowego punktu trasy dla ka�dej ci�ar�wki
    for (auto& truck : trucks) {
        truck.addToRoute(nodes[depotIndex].getCoordinates());
    }

    // P�tla g��wna - dop�ki istniej� w�z�y z zapotrzebowaniem
    while (true) {
        bool deliveryMade = false;

        // Iteracja po ka�dej ci�ar�wce
        for (auto& truck : trucks) {
            if (truck.getLoad() <= 0) {
                continue; // Je�li ci�ar�wka nie ma �adunku, pomi� j�
            }

            // Znajd� najbli�szy w�ze� z zapotrzebowaniem
            int closestNodeIndex = -1;
            double minDistance = std::numeric_limits<double>::max();

            for (size_t i = 0; i < nodes.size(); ++i) {
                if (nodes[i].getDemand() > 0) { // Sprawd�, czy w�ze� ma zapotrzebowanie
                    double distance = calculateDistance(truck.getRoute().back(), nodes[i].getCoordinates());
                    if (distance < minDistance) {
                        minDistance = distance;
                        closestNodeIndex = static_cast<int>(i);
                    }
                }
            }

            // Je�li znaleziono najbli�szy w�ze�, wykonaj dostaw�
            if (closestNodeIndex != -1) {
                nodes[closestNodeIndex].deliver(truck); // Dostarcz towar do w�z�a
                deliveryMade = true;
            }
        }

        // Je�li �adna dostawa nie zosta�a wykonana, zako�cz p�tl�
        if (!deliveryMade) {
            break;
        }
    }

    // Dodanie depozytu jako ko�cowego punktu trasy dla ka�dej ci�ar�wki
    for (auto& truck : trucks) {
        truck.addToRoute(nodes[depotIndex].getCoordinates());
    }

    // Obliczenie sumy wszystkich tras
    sumOfallRoutes = 0;
    for (auto& truck : trucks) {
        float routeLength = 0;
        for (size_t i = 0; i < truck.getRoute().size() - 1; ++i) {
            routeLength += calculateDistance(truck.getRoute()[i], truck.getRoute()[i + 1]);
        }
        sumOfallRoutes += routeLength;
    }
}
