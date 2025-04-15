#include "Truck.h"
#include "imgui.h" // Dodano include dla ImVec2
#include <vector>
#include <cmath>

// Konstruktor domyœlny
Truck::Truck() : x(0), y(0), load(0) {}
// Konstruktor z parametrami
Truck::Truck(ImVec2 coordinates, int load) : x(coordinates.x), y(coordinates.y), load(load) {}
// Gettery
ImVec2 Truck::getCoordinates() const { return ImVec2(x, y); }
int Truck::getLoad() const { return load; }
const std::vector<ImVec2>& Truck::getRoute() const {
	return route;
}
// Settery
void Truck::setLoad(int load) { this->load = load; }
void Truck::setRoute(const std::vector<ImVec2>& route) {
	this->route = route;
}
// Metody
void Truck::addToRoute(const ImVec2& point) {
	route.push_back(point);
}
int Truck::deliver(ImVec2 deliverTo, int amountDelivered) {
	load -= amountDelivered;
	addToRoute(deliverTo);
	return amountDelivered;
}
void Truck::swapRoute(int i, int j) {
	if (i < 0 || j < 0 || i >= route.size() || j >= route.size()) {
		return;
	}
	std::swap(route[i], route[j]);
}
double Truck::getRouteLength() const {
	double length = 0.0;
	for (int i = 0; i < route.size() - 1; i++) {
		length += std::sqrt(std::pow(route[i + 1].x - route[i].x, 2) + std::pow(route[i + 1].y - route[i].y, 2));
	}
	return length;
}