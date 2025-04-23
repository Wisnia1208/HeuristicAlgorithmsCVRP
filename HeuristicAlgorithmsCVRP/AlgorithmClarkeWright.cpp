#include "AlgorithmClarkeWright.h"
#include <algorithm>

AlgorithmClarkeWright::AlgorithmClarkeWright() {}

void AlgorithmClarkeWright::solve() {
	std::vector<std::vector<Node>> routes;
	auto savingsMatrix = calculateSavingsMatrix();
	std::vector<std::tuple<int, int, double>> savingsList;
	for (auto& node : nodes) {
		if (node.getDemand() == 0) continue;
		std::vector<Node> route;
		route.push_back(node);
		routes.push_back(route);
	}

	for (int i = 0; i < nodes.size(); i++) {
		for (int j = 0; j < nodes.size(); j++) {
			if (i != j and i != depotIndex and j != depotIndex) {
				double savings = savingsMatrix[depotIndex][i] + savingsMatrix[depotIndex][j] - savingsMatrix[i][j];
				savingsList.push_back(std::make_tuple(i, j, savings));
			}
		}
	}

	std::sort(savingsList.begin(), savingsList.end(), [](const auto& a, const auto& b) {
		return std::get<2>(a) > std::get<2>(b);
	});

	while (routes.size() > trucks.size()) {
		for (const auto& [i, j, savings] : savingsList) {
			std::vector<Node>* route1 = nullptr;
			std::vector<Node>* route2 = nullptr;

			// Znajdz trasy r_i i r_j zawierajace i i j odpowiednio
			for (auto& route : routes) {
				if (route.back() == nodes[i]) { // i jest na końcu trasy r_i
					route1 = &route;
				}
				if (route.front() == nodes[j]) { // j jest na początku trasy r_j
					route2 = &route;
				}
			}

			// Sprawdz warunki polaczenia tras
			if (route1 && route2 && route1 != route2) { // Trasy muszą byc rozne
				int demand1 = calculateRouteDemand(*route1);
				int demand2 = calculateRouteDemand(*route2);

				if (demand1 + demand2 <= trucks[0].getLoad()) { // Suma ładunków ≤ Q
					// Połącz trasy: r_i = r_i + r_j
					route1->insert(route1->end(), route2->begin(), route2->end());

					// Zaktualizuj ładunek: load[r_i] += load[r_j]
					// (Nie musimy tego robić jawnie, bo `calculateRouteDemand` działa dynamicznie)

					// Usuń trasę r_j
					routes.erase(std::remove(routes.begin(), routes.end(), *route2), routes.end());

					// Przerwij pętlę, aby ponownie przetworzyć listę oszczędności
					break;
				}
			}
		}
	}

	for (size_t i = 0; i < trucks.size(); ++i) {
		if (i < routes.size()) {
			// Przekształć trasę z `routes` na `std::vector<ImVec2>`
			std::vector<ImVec2> truckRoute;

			// Dodaj depozyt na początek trasy
			truckRoute.push_back(nodes[depotIndex].getCoordinates());

			// Dodaj punkty trasy
			for (const auto& node : routes[i]) {
				truckRoute.push_back(node.getCoordinates());
			}

			// Dodaj depozyt na koniec trasy
			truckRoute.push_back(nodes[depotIndex].getCoordinates());

			// Ustaw trasę w ciężarówce
			trucks[i].setRoute(truckRoute);
		}
		else {
			// Jeśli liczba tras jest mniejsza niż liczba ciężarówek, ustaw pustą trasę
			trucks[i].setRoute({});
		}
	}

	// Obliczenie sumy wszystkich tras
	sumOfallRoutes = 0;
	for (auto& truck : trucks) {
		truck.setLoad(truck.getLoad() - calculateRouteDemand(truck.getRoute()));
		float routeLength = 0;
		for (size_t i = 0; i < truck.getRoute().size() - 1; ++i) {
			routeLength += calculateDistance(truck.getRoute()[i], truck.getRoute()[i + 1]);
		}
		sumOfallRoutes += routeLength;
	}
}

int AlgorithmClarkeWright::calculateRouteDemand(const std::vector<Node> route) {
	int demand = 0;
	for (const auto& node : route) {
		demand += node.getDemand();
	}
	return demand;
}

int AlgorithmClarkeWright::calculateRouteDemand(const std::vector<ImVec2> route) {
	int demand = 0;
	for (const auto& point : route) {
		demand += getNodeDemand(point.x, point.y);
	}
	return demand;
}

double AlgorithmClarkeWright::calculateRouteLength(const std::vector<Node> route) {
	double length = 0.0;
	for (size_t i = 0; i < route.size() - 1; ++i) {
		length += calculateDistance(route[i].getCoordinates(), route[i + 1].getCoordinates());
	}
	return length;
}

std::vector<std::vector<double>> AlgorithmClarkeWright::calculateSavingsMatrix() {
	std::vector<std::vector<double>> savingsMatrix(nodes.size(), std::vector<double>(nodes.size(), 0.0));
	for (size_t i = 0; i < nodes.size(); ++i) {
		for (size_t j = 0; j < nodes.size(); ++j) {
			if (i != j) {
				savingsMatrix[i][j] = calculateDistance(nodes[i].getCoordinates(), nodes[j].getCoordinates());
			}
		}
	}
	return savingsMatrix;
}
