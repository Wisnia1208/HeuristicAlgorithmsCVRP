#pragma once

#include <string>
#include <vector>
#include "Node.h"
#include "Truck.h"

class Experiment
{
private:
    std::string name;                     // Nazwa instancji
    std::string comment;                  // Komentarz
    std::string type;                     // Typ problemu (np. CVRP)
    int dimension;                        // Liczba w�z��w
    std::string edgeWeightType;           // Typ wag (np. EUC_2D)
    int capacity;                         // Pojemno�� pojazdu
    std::vector<Node> nodes;              // W�z�y (koordynaty i zapotrzebowanie)
    int depot;                            // Indeks depozytu
    std::vector<Truck> trucks;            // Wektor ci�ar�wek
    double optimalValue;                  // Optymalna warto��

public:
    // Konstruktor domy�lny
    Experiment();

    // Metoda do wczytywania danych z pliku
    bool loadFromFile(const std::string& filePath);

    // Gettery
    const std::string& getName() const;
    const std::string& getComment() const;
    const std::string& getType() const;
    int getDimension() const;
    const std::string& getEdgeWeightType() const;
    int getCapacity() const;
    const std::vector<Node>& getNodes() const;
    int getDepot() const;
    const std::vector<Truck>& getTrucks() const;
    double getOptimalValue() const;
	Node getNodeByCoordinates(int x, int y) const;
};
