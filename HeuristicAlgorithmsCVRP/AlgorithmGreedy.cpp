#include "AlgorithmGreedy.h"

AlgorithmGreedy::AlgorithmGreedy() {}

void AlgorithmGreedy::solve() {
    // Dodanie depozytu jako pocz¹tkowego punktu trasy dla ka¿dej ciê¿arówki
    for (auto& truck : trucks) {
        truck.addToRoute(nodes[depotIndex].getCoordinates());
    }

    // Pêtla g³ówna - dopóki istniej¹ wêz³y z zapotrzebowaniem
    while (true) {
        bool deliveryMade = false;

        // Iteracja po ka¿dej ciê¿arówce
        for (auto& truck : trucks) {
            if (truck.getLoad() <= 0) {
                continue; // Jeœli ciê¿arówka nie ma ³adunku, pomiñ j¹
            }

            // ZnajdŸ najbli¿szy wêze³ z zapotrzebowaniem
            int closestNodeIndex = -1;
            double minDistance = std::numeric_limits<double>::max();

            for (size_t i = 0; i < nodes.size(); ++i) {
                if (nodes[i].getDemand() > 0) { // SprawdŸ, czy wêze³ ma zapotrzebowanie
                    double distance = calculateDistance(truck.getRoute().back(), nodes[i].getCoordinates());
                    if (distance < minDistance) {
                        minDistance = distance;
                        closestNodeIndex = static_cast<int>(i);
                    }
                }
            }

            // Jeœli znaleziono najbli¿szy wêze³, wykonaj dostawê
            if (closestNodeIndex != -1) {
                nodes[closestNodeIndex].deliver(truck); // Dostarcz towar do wêz³a
                deliveryMade = true;
            }
        }

        // Jeœli ¿adna dostawa nie zosta³a wykonana, zakoñcz pêtlê
        if (!deliveryMade) {
            break;
        }
    }

    // Dodanie depozytu jako koñcowego punktu trasy dla ka¿dej ciê¿arówki
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
