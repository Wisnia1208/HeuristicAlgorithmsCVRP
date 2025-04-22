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

	// Sprawdzenie, czy trasa ma wystarczaj¹c¹ liczbê punktów
	if (route.size() < 2) {
		return length; // Zwróæ 0, jeœli trasa jest pusta lub zawiera tylko jeden punkt
	}

	// Obliczanie d³ugoœci trasy
	for (size_t i = 0; i < route.size() - 1; i++) {
		length += std::sqrt(std::pow(route[i + 1].x - route[i].x, 2) +
			std::pow(route[i + 1].y - route[i].y, 2));
	}

	return length;
}

void Truck::convertImVec2ToVec2() {
	routePoints.clear(); // Wyczyœæ wektor przed dodaniem nowych punktów
	for (auto& point : route) {
		Vec2D vec2d(point.x, point.y);
		routePoints.push_back(vec2d);
	}
}

void Truck::convertVec2ToImVec2() {
	route.clear(); // Wyczyœæ wektor przed dodaniem nowych punktów
	for (auto& point : routePoints) {
		ImVec2 imvec2(point.x, point.y);
		route.push_back(imvec2);
	}
}