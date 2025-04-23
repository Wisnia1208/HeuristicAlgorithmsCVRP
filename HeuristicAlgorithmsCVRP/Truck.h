#pragma once

#include "imgui.h" // Dodano include dla ImVec2
#include <vector>
#include "Vec2D.h"
#include <functional> // std::hash
#include "ImVec2Extensions.h"

// Definicja klasy Truck
class Truck {
private:
    int x;       // Wspó³rzêdna X
    int y;       // Wspó³rzêdna Y
    int load;    // Towar

public:
    std::vector<ImVec2> route; // Droga
    std::vector<Vec2D> routePoints; // Punkty na drodze

    // Konstruktor domyœlny
    Truck();

    // Konstruktor z parametrami
    Truck(ImVec2 coordinates, int load);

    // Gettery
    ImVec2 getCoordinates() const;
    int getLoad() const;

    // Settery
    void setLoad(int load);
    void setRoute(const std::vector<ImVec2>& route);

    // Metody
    void addToRoute(const ImVec2& point);
    const std::vector<ImVec2>& getRoute() const;
    int deliver(ImVec2 deliverTo, int amountDelivered);
    void swapRoute(int i, int j);
    double getRouteLength() const;
    void convertImVec2ToVec2();
    void convertVec2ToImVec2();
    void twoOpt();
	double calculateDistance(const ImVec2& firstPoint, const ImVec2& secondPoint) const;

    // Operator porównania
    bool operator==(const Truck& other) const {
        return route == other.route && load == other.load;
    }
};

// Definicja funkcji hash dla klasy Truck
namespace std {
    template <>
    struct hash<Truck> {
        size_t operator()(const Truck& truck) const {
            size_t hashValue = 0;
            for (const auto& point : truck.route) {
                hashValue ^= std::hash<float>()(point.x) ^ std::hash<float>()(point.y);
            }
            hashValue ^= std::hash<int>()(truck.getLoad());
            return hashValue;
        }
    };

    // Definicja funkcji hash dla std::vector<Truck>
    template <>
    struct hash<std::vector<Truck>> {
        size_t operator()(const std::vector<Truck>& trucks) const {
            size_t hashValue = 0;
            for (const auto& truck : trucks) {
                hashValue ^= std::hash<Truck>()(truck);
            }
            return hashValue;
        }
    };
}
