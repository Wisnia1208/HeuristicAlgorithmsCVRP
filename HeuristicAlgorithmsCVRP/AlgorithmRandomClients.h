#pragma once
#include "Algorithm.h"
#include <GLFW/glfw3.h>
class AlgorithmRandomClients :
    public Algorithm
{
public:
	AlgorithmRandomClients();
	double getSumOfRoutes() const;
	void set(const std::vector<Node> nodes, const std::vector<Truck> trucks);
	void solve();
	std::vector<Truck> getTrucks() const;
	float calculateDistance(const ImVec2 firstPoint, const ImVec2 secondPoint) const;
};

