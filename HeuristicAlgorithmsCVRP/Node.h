#pragma once

#include "imgui.h" // Dodano include dla ImVec2
#include "Truck.h"

class Node
{
private:
    int x;       // Wspó³rzêdna X
    int y;       // Wspó³rzêdna Y
    int demand;  // Zapotrzebowanie

public:
    // Konstruktor domyœlny
    Node();
    // Konstruktor z parametrami
    Node(int x, int y, int demand);
    // Gettery
	ImVec2 getCoordinates() const;
    int getDemand() const;
    // Settery
    void setDemand(int demand);
	// Metody
    void deliver(Truck& truck);
};
