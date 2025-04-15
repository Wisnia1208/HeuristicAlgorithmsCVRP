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

void Algorithm::kOpt(const int k) { //nie rozumiem kOpta
	if (k < 2) {
		return;
	}
	for(auto& truck : trucks) {
		Truck tempTruck = truck;
		int routeSize = truck.getRoute().size();
		for (int i = 0; i < routeSize - k; i++) {
			for (int j = i + 1; j < routeSize; j++) {
				tempTruck.swapRoute(i, j);
				double newRouteLength = tempTruck.getRouteLength();
				if (newRouteLength < truck.getRouteLength()) {
					sumOfallRoutes -= truck.getRouteLength();
					truck = tempTruck;
					sumOfallRoutes += newRouteLength;
				}
				else {
					tempTruck.swapRoute(i, j);
				}
			}
		}
	}
}