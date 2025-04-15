#include "AlgorithmRandomClients.h"
#include <cmath>

AlgorithmRandomClients::AlgorithmRandomClients() {}

void AlgorithmRandomClients::solve() {
	for (auto& truck : trucks) {
		truck.addToRoute(nodes[depotIndex].getCoordinates());
	}

    while (true) {
        // Filtruj w�z�y z niezerowym zapotrzebowaniem
        std::vector<Node*> nodesWithDemand;
        for (auto& node : nodes) {
            if (node.getDemand() > 0) {
                nodesWithDemand.push_back(&node);
            }
        }

        // Je�li nie ma wi�cej w�z��w z zapotrzebowaniem, zako�cz p�tl�
        if (nodesWithDemand.empty()) {
            break;
        }

        // Wybierz losow� ci�ar�wk�
        int randomTruckIndex = rand() % trucks.size();
        Truck& selectedTruck = trucks[randomTruckIndex];

        // Wybierz losowy w�ze� z niezerowym zapotrzebowaniem
        int randomNodeIndex = rand() % nodesWithDemand.size();
        Node* selectedNode = nodesWithDemand[randomNodeIndex];

        // Dostarcz towar do wybranego w�z�a
        selectedNode->deliver(selectedTruck);
    }

	for (auto& truck : trucks) {
		truck.addToRoute(nodes[depotIndex].getCoordinates());
	}

	sumOfallRoutes = 0;
	for (auto& truck : trucks) {
		float routeLength = 0;
		for (size_t i = 0; i < truck.getRoute().size() - 1; ++i) {
			routeLength += calculateDistance(truck.getRoute()[i], truck.getRoute()[i + 1]);
		}
		sumOfallRoutes += routeLength;
	}
}