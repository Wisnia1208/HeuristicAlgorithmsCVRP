#pragma once

#include "imgui.h" // Dodano include dla ImVec2
#include <vector>
#include "Vec2D.h"

class Truck
{
private:
    int x;       // Wspó³rzêdna X
    int y;       // Wspó³rzêdna Y
    int load;    // Towar
    std::vector<ImVec2> route; // Droga
public:
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
};
