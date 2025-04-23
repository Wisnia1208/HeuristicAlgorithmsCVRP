#pragma once

#include "Node.h"
#include "Truck.h"

class Algorithm
{
protected:
	std::vector<Node> nodes;
	std::vector<Truck> trucks;
	double sumOfallRoutes;
	int depotIndex;
public:
	double getSumOfRoutes() const;
	void set(const std::vector<Node> nodes, const std::vector<Truck> trucks, int depotIndex);
	void setTrucks(const std::vector<Truck> trucks);
	std::vector<Truck> getTrucks() const;
	double calculateDistance(const ImVec2 firstPoint, const ImVec2 secondPoint) const;
	void twoOpt();
	int getNodeDemand(int x, int y);
	int getNodeDemand(ImVec2 node);
};

