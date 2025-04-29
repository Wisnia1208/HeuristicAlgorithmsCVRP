#include "Node.h"
#include "imgui.h" // Dodano include dla ImVec2

// Konstruktor domyœlny
Node::Node() : x(0), y(0), demand(0) {}

// Konstruktor z parametrami
Node::Node(int x, int y, int demand) : x(x), y(y), demand(demand) {}

// Gettery
ImVec2 Node::getCoordinates() const { return ImVec2(x, y); }
int Node::getDemand() const { return demand; }

// Settery
void Node::setDemand(int demand) { this->demand = demand; }

// Metody
void Node::deliver(Truck &truck) {
	/*if (truck.getLoad() > this->demand) {
		this->demand -= truck.deliver(this->getCoordinates(), this->demand);
	}*/
	if (truck.getLoad() > this->demand) {
		this->demand -= truck.deliver(this->getCoordinates(), this->demand);
	}
	else {
		this->demand -= truck.deliver(this->getCoordinates(), truck.getLoad());
	}
}

void Node::deliverWithSplit(Truck& truck) {
	if (truck.getLoad() > this->demand) {
		this->demand -= truck.deliver(this->getCoordinates(), this->demand);
	}
	else {
		this->demand -= truck.deliver(this->getCoordinates(), truck.getLoad());
	}
}

// Implement the equality operator if not implemented inline in the header  
bool Node::operator==(const Node& other) const {
	return x == other.x && y == other.y && demand == other.demand;
}