#pragma once

#include "Node.h"
#include "Truck.h"

class Algorithm
{
protected:
	std::vector<Node> nodes;
	std::vector<Truck> trucks;
	int depotIndex;
};

