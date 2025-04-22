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
    int dimension;                        // Liczba wêz³ów
    std::string edgeWeightType;           // Typ wag (np. EUC_2D)
    int capacity;                         // Pojemnoœæ pojazdu
    std::vector<Node> nodes;              // Wêz³y (koordynaty i zapotrzebowanie)
    int depot;                            // Indeks depozytu
    std::vector<Truck> trucks;            // Wektor ciê¿arówek
    double optimalValue;                  // Optymalna wartoœæ

public:
    // Konstruktor domyœlny
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
