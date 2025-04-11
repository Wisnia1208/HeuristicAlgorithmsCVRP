#include "Experiment.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <regex> // Dodano include dla std::regex

// Konstruktor domy�lny
Experiment::Experiment() : dimension(0), capacity(0), depot(-1), optimalValue(0.0) {}

// Metoda do wczytywania danych z pliku
bool Experiment::loadFromFile(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Cannot open file: " << filePath << std::endl;
        return false;
    }

    nodes.clear();  // Wyczy�� wektor w�z��w przed wczytaniem nowych danych
    trucks.clear(); // Wyczy�� wektor ci�ar�wek przed wczytaniem nowych danych

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key;
        iss >> key;

        if (key == "NAME") {
            iss.ignore(3); // Ignorowanie " : "
            std::getline(iss, name);
        }
        else if (key == "COMMENT") {
            iss.ignore(3);
            std::getline(iss, comment);

            // Wyodr�bnienie optymalnej warto�ci z komentarza za pomoc� regex
            std::regex optimalValueRegex(R"(Optimal value: (\d+(\.\d+)?))");
            std::smatch match;
            if (std::regex_search(comment, match, optimalValueRegex)) {
                optimalValue = std::stod(match[1].str());
            }

            // Wyodr�bnienie liczby ci�ar�wek z komentarza za pomoc� regex
            std::regex truckRegex(R"(No of trucks: (\d+))");
            if (std::regex_search(comment, match, truckRegex)) {
                int numberOfTrucks = std::stoi(match[1].str());
                trucks.resize(numberOfTrucks); // Rezerwacja miejsca na ci�ar�wki (inicjalizacja p�niej)
            }
        }
        else if (key == "TYPE") {
            iss.ignore(3);
            iss >> type;
        }
        else if (key == "DIMENSION") {
            iss.ignore(3);
            iss >> dimension;
        }
        else if (key == "EDGE_WEIGHT_TYPE") {
            iss.ignore(3);
            iss >> edgeWeightType;
        }
        else if (key == "CAPACITY") {
            iss.ignore(3);
            iss >> capacity;
        }
        else if (key == "NODE_COORD_SECTION") {
            for (int i = 0; i < dimension; ++i) {
                int id, x, y;
                file >> id >> x >> y;
                nodes.emplace_back(x, y, 0); // Dodanie w�z�a z zapotrzebowaniem 0 (zaktualizowane p�niej)
            }
        }
        else if (key == "DEMAND_SECTION") {
            for (int i = 0; i < dimension; ++i) {
                int id, demand;
                file >> id >> demand;
                nodes[id - 1].setDemand(demand); // Aktualizacja zapotrzebowania dla w�z�a
            }
        }
        else if (key == "DEPOT_SECTION") {
            int depotId;
            file >> depotId;
            depot = depotId - 1; // Indeks depozytu (zak�adamy indeksowanie od 0)
            file.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignorowanie reszty sekcji
        }
    }

    // Inicjalizacja ci�ar�wek po wczytaniu wszystkich w�z��w
    if (!nodes.empty() && depot >= 0 && depot < nodes.size()) {
        for (auto& truck : trucks) {
            truck = Truck(nodes[depot].getCoordinates(), capacity); // Ustawienie pozycji startowej i pojemno�ci
        }
    }
    else {
        std::cerr << "Error: Nodes or depot not properly initialized. Cannot initialize trucks." << std::endl;
        return false;
    }

    file.close();
    return true;
}

// Gettery
const std::string& Experiment::getName() const { return name; }
const std::string& Experiment::getComment() const { return comment; }
const std::string& Experiment::getType() const { return type; }
int Experiment::getDimension() const { return dimension; }
const std::string& Experiment::getEdgeWeightType() const { return edgeWeightType; }
int Experiment::getCapacity() const { return capacity; }
const std::vector<Node>& Experiment::getNodes() const { return nodes; }
int Experiment::getDepot() const { return depot; }
const std::vector<Truck>& Experiment::getTrucks() const { return trucks; }
double Experiment::getOptimalValue() const { return optimalValue; } // Getter dla optymalnej warto�ci
