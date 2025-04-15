#include "Algorithm.h"
#include <cmath>

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

float Algorithm::calculateDistance(const ImVec2 firstPoint, const ImVec2 secondPoint) const {
	return std::sqrt(std::pow(firstPoint.x - secondPoint.x, 2) +
		std::pow(firstPoint.y - secondPoint.y, 2));
}