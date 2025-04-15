#include "AlgorithmRandomClients.h"
#include <cmath>

AlgorithmRandomClients::AlgorithmRandomClients() {}

void AlgorithmRandomClients::set(const std::vector<Node> nodes, const std::vector<Truck> trucks) {
	this->nodes = nodes;
	this->trucks = trucks;
}

double AlgorithmRandomClients::getSumOfRoutes() const {
	return sumOfallRoutes;
}

void AlgorithmRandomClients::solve() {
	for (auto& truck : trucks) {
		truck.addToRoute(nodes[depotIndex].getCoordinates());
	}
	sumOfallRoutes = 0;
	for (auto& node : nodes) {
		while (node.getDemand() > 0) {
			int randomTruckIndex = rand() % trucks.size();
			node.deliver(trucks[randomTruckIndex]);
		}
	}
	for (auto& truck : trucks) {
		truck.addToRoute(nodes[depotIndex].getCoordinates());
	}
	for (auto& truck : trucks) {
		float routeLength = 0;
		for (size_t i = 0; i < truck.getRoute().size() - 1; ++i) {
			routeLength += calculateDistance(truck.getRoute()[i], truck.getRoute()[i + 1]);
		}
		sumOfallRoutes += routeLength;
	}
}

std::vector<Truck> AlgorithmRandomClients::getTrucks() const {
	return trucks;
}

float AlgorithmRandomClients::calculateDistance(const ImVec2 firstPoint, const ImVec2 secondPoint) const {
	return std::sqrt(std::pow(firstPoint.x - secondPoint.x, 2) +
		std::pow(firstPoint.y - secondPoint.y, 2));
}