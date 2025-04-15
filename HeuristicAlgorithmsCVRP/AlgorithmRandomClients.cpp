#include "AlgorithmRandomClients.h"
#include <cmath>

AlgorithmRandomClients::AlgorithmRandomClients() {}

void AlgorithmRandomClients::solve() {
	for (auto& truck : trucks) {
		truck.addToRoute(nodes[depotIndex].getCoordinates());
	}

    while (true) {
        std::vector<Node*> nodesWithDemand;
        for (auto& node : nodes) {
            if (node.getDemand() > 0) {
                nodesWithDemand.push_back(&node);
            }
        }

        if (nodesWithDemand.empty()) {
            break;
        }

        int randomTruckIndex = rand() % trucks.size();
        Truck& selectedTruck = trucks[randomTruckIndex];

        int randomNodeIndex = rand() % nodesWithDemand.size();
        Node* selectedNode = nodesWithDemand[randomNodeIndex];

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