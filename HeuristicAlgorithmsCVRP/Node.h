#pragma once

#include "imgui.h" // Dodano include dla ImVec2
#include "Truck.h"

class Node
{
private:
    int x;       // Wsp�rz�dna X
    int y;       // Wsp�rz�dna Y
    int demand;  // Zapotrzebowanie

public:
    // Konstruktor domy�lny
    Node();
    // Konstruktor z parametrami
    Node(int x, int y, int demand);
    // Add the equality operator to the Node class  
    bool operator==(const Node& other) const;
    // Gettery
	ImVec2 getCoordinates() const;
    int getDemand() const;
    // Settery
    void setDemand(int demand);
	// Metody
    void deliver(Truck& truck);
	void deliverWithSplit(Truck& truck);
};
