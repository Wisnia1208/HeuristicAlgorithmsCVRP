#pragma once

#include "imgui.h" // Dodano include dla ImVec2
#include <vector>

class Truck
{
    private:
        int x;       // Wsp�rz�dna X
        int y;       // Wsp�rz�dna Y
        int load;   // Towar
		std::vector <ImVec2> route; // Droga

	public:
		// Konstruktor domy�lny
		Truck();
		// Konstruktor z parametrami
		Truck(ImVec2 coordinates, int load);
		// Gettery
		ImVec2 getCoordinates() const;
		int getLoad() const;
		// Settery
		void setLoad(int load);
		// Metody
		void addToRoute(const ImVec2& point);
		const std::vector<ImVec2>& getRoute() const;
		int deliver(ImVec2 deliverTo, int amountDelivered);
};
