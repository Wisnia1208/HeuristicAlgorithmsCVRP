#include "Truck.h"
#include "imgui.h" // Dodano include dla ImVec2
#include <vector>

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
// Metody
void Truck::addToRoute(const ImVec2& point) {
	route.push_back(point);
}
int Truck::deliver(ImVec2 deliverTo, int amountDelivered) {
	load -= amountDelivered;
	addToRoute(deliverTo);
	return amountDelivered;
}