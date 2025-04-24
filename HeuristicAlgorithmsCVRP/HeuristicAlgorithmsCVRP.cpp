#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Experiment.h"
#include "Algorithm.h"
#include "AlgorithmRandomClients.h"
#include "AlgorithmGreedy.h"
#include "AlgorithmClarkeWright.h"
#include "AlgorithmSimulatedAnnealing.h"
#include "AlgorithmTabuSearch.h"

#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib> // std::rand, std::srand
#include <ctime>   // std::time
#include <vector>  // std::vector
#include <cmath>   // std::sqrt, std::pow
#include <string>
#include <filesystem> // Wymagane dla std::filesystem

#define GRID_SIZE 75.0f // Rozmiar pojedynczego kwadratu w pikselach
#define GRID_COUNT 10 // Liczba kwadratów w wierszu/kolumnie

float scalePoint(const float point) {
	return point * GRID_SIZE / GRID_COUNT;
}

void DrawGridWithLabels(ImDrawList* draw_list, const ImVec2& cursorScreenPos, float grid_size, int grid_count) {
	ImU32 grid_color = IM_COL32(200, 200, 200, 255); // Kolor siatki

	// Rysowanie siatki
	for (int i = 0; i <= grid_count; i++) {
		// Linie poziome
		float y = cursorScreenPos.y + i * grid_size;
		draw_list->AddLine(ImVec2(cursorScreenPos.x, y), ImVec2(cursorScreenPos.x + grid_count * grid_size, y), grid_color);

		// Linie pionowe
		float x = cursorScreenPos.x + i * grid_size;
		draw_list->AddLine(ImVec2(x, cursorScreenPos.y), ImVec2(x, cursorScreenPos.y + grid_count * grid_size), grid_color);
	}

	// Dodanie opisu osi X
	for (int i = 0; i <= grid_count; i++) {
		float x = cursorScreenPos.x + i * grid_size;
		std::string label = std::to_string(i * 10); // Wartości od 0 do 100

		// Wyśrodkowanie tekstu względem siatki
		ImVec2 text_size = ImGui::CalcTextSize(label.c_str());
		draw_list->AddText(ImVec2(x - text_size.x / 2, cursorScreenPos.y + grid_count * grid_size + 5), IM_COL32(255, 255, 255, 255), label.c_str());
	}

	// Dodanie opisu osi Y
	for (int i = 0; i <= grid_count; i++) {
		float y = cursorScreenPos.y + (grid_count - i) * grid_size;
		std::string label = std::to_string(i * 10); // Wartości od 0 do 100

		// Wyśrodkowanie tekstu względem siatki i wyrównanie do prawej
		ImVec2 text_size = ImGui::CalcTextSize(label.c_str());
		draw_list->AddText(ImVec2(cursorScreenPos.x - text_size.x - 5, y - text_size.y / 2), IM_COL32(255, 255, 255, 255), label.c_str());
	}

	// Dodanie opisu "Axis X" poniżej osi X, wyśrodkowanego względem siatki
	ImVec2 axis_x_label_pos = ImVec2(cursorScreenPos.x + grid_count * grid_size / 2 - ImGui::CalcTextSize("Axis X").x / 2, cursorScreenPos.y + grid_count * grid_size + 20);
	draw_list->AddText(axis_x_label_pos, IM_COL32(255, 255, 255, 255), "Axis X");

	// Dodanie opisu "Axis Y" obok osi Y, przesuniętego bardziej w lewo
	ImVec2 axis_y_label_pos = ImVec2(cursorScreenPos.x - 75, cursorScreenPos.y + grid_count * grid_size / 2 - ImGui::CalcTextSize("Axis Y").y / 2);
	draw_list->AddText(axis_y_label_pos, IM_COL32(255, 255, 255, 255), "Axis Y");
}

void DrawPoints(ImDrawList* draw_list, const ImVec2& cursorScreenPos, const Experiment experiment) {
    ImU32 point_color = IM_COL32(255, 0, 0, 255); // Kolor punktów (czerwony)
    ImU32 depot_color = IM_COL32(0, 255, 0, 255); // Kolor punktu depozytu (zielony)
    float point_radius = 5.0f; // Promień punktu
    auto& nodes = experiment.getNodes();
    int depotIndex = experiment.getDepot(); // Indeks depozytu

    for (int i = 0; i < nodes.size(); ++i) {
        const auto& point = nodes[i];
        ImU32 color = (i == depotIndex) ? depot_color : point_color; // Wybór koloru
        draw_list->AddCircleFilled(
            ImVec2(cursorScreenPos.x + scalePoint(point.getCoordinates().x),
                cursorScreenPos.y + GRID_SIZE * GRID_COUNT - scalePoint(point.getCoordinates().y)),
            point_radius,
            color
        );
    }
}

void DrawLineBetweenPoints(ImDrawList* draw_list, const ImVec2& offset, const ImVec2& point1, const ImVec2& point2, ImU32 color, float thickness = 2.0f) {
	// Przeskalowanie punktów z zakresu [0, 100] do rozmiaru siatki
	ImVec2 scaled_point1(scalePoint(point1.x), GRID_SIZE * GRID_COUNT - scalePoint(point1.y));
	ImVec2 scaled_point2(scalePoint(point2.x), GRID_SIZE * GRID_COUNT - scalePoint(point2.y));

	// Rysowanie linii między przeskalowanymi punktami
	draw_list->AddLine(ImVec2(offset.x + scaled_point1.x, offset.y + scaled_point1.y),
		ImVec2(offset.x + scaled_point2.x, offset.y + scaled_point2.y),
		color, thickness);
}

auto getColorForTruck = [](int index) -> ImU32 {
	static const ImU32 colors[] = {
		IM_COL32(255, 0, 0, 255),    // Czerwony
		IM_COL32(0, 255, 0, 255),    // Zielony
		IM_COL32(0, 0, 255, 255),    // Niebieski
		IM_COL32(255, 255, 0, 255),  // Żółty
		IM_COL32(255, 0, 255, 255),  // Fioletowy
		IM_COL32(0, 255, 255, 255),  // Turkusowy
		IM_COL32(128, 0, 0, 255),    // Bordowy
		IM_COL32(0, 128, 0, 255),    // Ciemnozielony
		IM_COL32(0, 0, 128, 255),    // Granatowy
		IM_COL32(128, 128, 0, 255),  // Oliwkowy
		IM_COL32(128, 0, 128, 255),  // Purpurowy
		IM_COL32(0, 128, 128, 255),  // Morski
		IM_COL32(192, 192, 192, 255),// Srebrny
		IM_COL32(128, 128, 128, 255),// Szary
		IM_COL32(255, 165, 0, 255),  // Pomarańczowy
		IM_COL32(255, 20, 147, 255), // Różowy
		IM_COL32(75, 0, 130, 255),   // Indygo
		IM_COL32(173, 255, 47, 255), // Zielonożółty
		IM_COL32(139, 69, 19, 255),  // Brązowy
		IM_COL32(70, 130, 180, 255)  // Stalowy niebieski
	};
	return colors[index % (sizeof(colors) / sizeof(colors[0]))];
	};

// Funkcja do wczytywania listy plików .vrp z folderu
std::vector<std::string> getVRPFiles(const std::string& folderPath) {
	std::vector<std::string> files;
	for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
		if (entry.is_regular_file() && entry.path().extension() == ".vrp") {
			files.push_back(entry.path().filename().string());
		}
	}
	return files;
}

int main() {
	// Inicjalizacja GLFW
	if (!glfwInit()) return -1;

	// Kontekst OpenGL
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Tworzenie okna
	GLFWwindow* window = glfwCreateWindow(1900, 1000, "Experiment GUI", nullptr, nullptr);
	if (!window) { glfwTerminate(); return -1; }
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // vsync

	// Inicjalizacja ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	// Obiekt klasy Experiment
	Experiment experiment;
	Algorithm algorithm;
	AlgorithmRandomClients algorithmRandomClients;
	AlgorithmGreedy algorithmGreedy;
	AlgorithmClarkeWright algorithmClarkeWright;
	AlgorithmSimulatedAnnealing algorithmSimulatedAnnealing;
	AlgorithmTabuSearch algorithmTabuSearch;
	bool drawTruckRoutes = false; // Flaga kontrolująca rysowanie tras
	//bool isFileLoaded = false; // Flaga informująca, czy plik został wczytany

	// Lista plików .vrp
	std::vector<std::string> vrpFiles = getVRPFiles("input_files");
	static int selectedFileIndex = -1; // Indeks wybranego pliku

	//std::vector<ImVec2> points;
	float grid_size = GRID_SIZE; // Rozmiar pojedynczego kwadratu w pikselach
	int grid_count = GRID_COUNT; // Liczba kwadratów w wierszu/kolumnie

	// Główna pętla
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		// Start ImGui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Okno GUI
		ImGui::Begin("Panel Sterowania", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

		// Drop-down box z listą plików
		if (ImGui::BeginCombo("Wybierz plik", selectedFileIndex >= 0 ? vrpFiles[selectedFileIndex].c_str() : "Brak wybranego pliku")) {
			for (int i = 0; i < vrpFiles.size(); ++i) {
				bool isSelected = (selectedFileIndex == i);
				if (ImGui::Selectable(vrpFiles[i].c_str(), isSelected)) {
					selectedFileIndex = i; // Ustaw wybrany plik
				}
				if (isSelected) {
					ImGui::SetItemDefaultFocus(); // Ustaw fokus na wybranym elemencie
				}
			}
			ImGui::EndCombo();
		}

		// Przycisk "Load"
		if (ImGui::Button("Load")) {
			if (selectedFileIndex >= 0) {
				std::string filePath = "input_files/" + vrpFiles[selectedFileIndex];
				if (experiment.loadFromFile(filePath)) {
					drawTruckRoutes = false; // Resetowanie flagi przed nowym wczytaniem

					// Zerowanie wszystkich zmiennych typu diff
					/*randomClientsDiff = 0.0f;
					randomClients2OptDiff = 0.0f;
					greedyDiff = 0.0f;
					greedy2OptDiff = 0.0f;
					clarkeWrightDiff = 0.0f;
					clarkeWright2OptDiff = 0.0f;*/
					//te wszystktkie trzeba by zadeklarować wczesniej żeby to działało
					//isFileLoaded = true;
					//std::cout << "File loaded succesfully: " << filePath << std::endl;
				}
				else {
					//isFileLoaded = false;
					//std::cerr << "File loading failed: " << filePath << std::endl;
				}
			}
			else {
				//std::cerr << "File was not selected" << std::endl;
			}
		}

		ImGui::SameLine();
		if (ImGui::Button("Run all experiments")) {
			//todo
		}

		ImGui::End();

        ImGui::Begin("Experiment Data", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

		// Wyświetlanie podstawowych informacji o eksperymencie
		ImGui::Text("Name: %s", experiment.getName().c_str());
		ImGui::Text("Comment: %s", experiment.getComment().c_str());
		ImGui::Text("Type: %s", experiment.getType().c_str());
		ImGui::Text("Dimension: %d", experiment.getDimension());
		ImGui::Text("Edge weight type: %s", experiment.getEdgeWeightType().c_str());
		ImGui::Text("Capacity: %d", experiment.getCapacity());
		ImGui::Text("Depot: %d", experiment.getDepot() + 1); // Indeks depozytu (1-based)
		ImGui::Text("No of Trucks: %d", experiment.getTrucks().size());
		ImGui::Text("Optimal Value: %.2f", experiment.getOptimalValue()); // Wyświetlanie optymalnej wartości

		ImGui::End();

		ImGui::SetNextWindowSize(ImVec2(950, 900));
		// Nowe okno z grafem
		ImGui::Begin("Graph 100x100", nullptr, ImGuiWindowFlags_NoResize);
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		ImVec2 cursorScreenPos = ImGui::GetCursorScreenPos(); // Pozycja początkowa
		cursorScreenPos.x += 75; // Przesunięcie w prawo
		cursorScreenPos.y += 75; // Przesunięcie w prawo
        
		DrawGridWithLabels(draw_list, cursorScreenPos, grid_size, grid_count); // Rysowanie siatki
		
		DrawPoints(draw_list, cursorScreenPos, experiment); // Rysowanie punktów na siatce

		if (drawTruckRoutes) {
			int truckIndex = 0; // Indeks ciężarówki
			for (const auto& truck : algorithm.getTrucks()) {
				const auto& route = truck.getRoute();
				ImU32 truckColor = getColorForTruck(truckIndex); // Pobierz kolor dla ciężarówki

				// Rysowanie ścieżek ciężarówki
				for (size_t i = 0; i < route.size() - 1; ++i) {
					DrawLineBetweenPoints(
						draw_list,
						cursorScreenPos,
						route[i],
						route[i + 1],
						truckColor, // Użyj koloru ciężarówki
						2.0f
					);
				}
				++truckIndex; // Zwiększ indeks ciężarówki
			}
		}


		ImGui::End();

		ImGui::Begin("Algorithms Panel", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

		// Algorytm RandomClients
		static double randomClientsDiff = 0.0f; // Przechowuje sumę ścieżek dla RandomClients
		if (ImGui::Button("Run##RandomClients")) {
			algorithmRandomClients.set(experiment.getNodes(), experiment.getTrucks(), experiment.getDepot());
			algorithmRandomClients.solve();
			randomClientsDiff = algorithmRandomClients.getSumOfRoutes() - experiment.getOptimalValue(); // Pobranie sumy ścieżek
			algorithm.setTrucks(algorithmRandomClients.getTrucks());
			drawTruckRoutes = true;
		}
		ImGui::SameLine();
		ImGui::Text("Algorithm Random Clients");
		ImGui::SameLine();
		ImGui::InputDouble("##RandomClientsDiff", &randomClientsDiff, 0.0, 0.0, "%.2f", ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();
		ImGui::Text("(difference from optimum)");

		// Algorytm RandomClients 2-opt
		static double randomClients2OptDiff = 0.0f; // Przechowuje sumę ścieżek dla RandomClients 2-opt
		if (ImGui::Button("Run##RandomClients2Opt")) {
			algorithmRandomClients.set(experiment.getNodes(), experiment.getTrucks(), experiment.getDepot());
			algorithmRandomClients.solve();
			algorithmRandomClients.twoOpt(); // Uruchomienie algorytmu 2-opt
			randomClients2OptDiff = algorithmRandomClients.getSumOfRoutes() - experiment.getOptimalValue(); // Pobranie sumy ścieżek
			algorithm.setTrucks(algorithmRandomClients.getTrucks());
			drawTruckRoutes = true;
		}
		ImGui::SameLine();
		ImGui::Text("Algorithm Random Clients 2-opt");
		ImGui::SameLine();
		ImGui::InputDouble("##RandomClients2OptDiff", &randomClients2OptDiff, 0.0, 0.0, "%.2f", ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();
		ImGui::Text("(difference from optimum)");

		// Algorytm Greedy
		static double greedyDiff = 0.0f; // Przechowuje sumę ścieżek dla Greedy
		if (ImGui::Button("Run##Greedy")) {
			algorithmGreedy.set(experiment.getNodes(), experiment.getTrucks(), experiment.getDepot());;
			algorithmGreedy.solve();
			greedyDiff = algorithmGreedy.getSumOfRoutes() - experiment.getOptimalValue(); // Pobranie sumy ścieżek
			algorithm.setTrucks(algorithmGreedy.getTrucks());
			drawTruckRoutes = true;
		}
		ImGui::SameLine();
		ImGui::Text("Algorithm Greedy");
		ImGui::SameLine();
		ImGui::InputDouble("##GreedyDiff", &greedyDiff, 0.0, 0.0, "%.2f", ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();
		ImGui::Text("(difference from optimum)");

		// Algorytm Greedy 2-opt
		static double greedy2OptDiff = 0.0f; // Przechowuje sumę ścieżek dla Greedy 2-opt
		if (ImGui::Button("Run##Greedy2Opt")) {
			algorithmGreedy.set(experiment.getNodes(), experiment.getTrucks(), experiment.getDepot());
			algorithmGreedy.solve();
			algorithmGreedy.twoOpt(); // Uruchomienie algorytmu 2-opt
			greedy2OptDiff = algorithmGreedy.getSumOfRoutes() - experiment.getOptimalValue(); // Pobranie sumy ścieżek
			algorithm.setTrucks(algorithmGreedy.getTrucks());
			drawTruckRoutes = true;
		}
		ImGui::SameLine();
		ImGui::Text("Algorithm Greedy 2-opt");
		ImGui::SameLine();
		ImGui::InputDouble("##Greedy2OptDiff", &greedy2OptDiff, 0.0, 0.0, "%.2f", ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();
		ImGui::Text("(difference from optimum)");

		// Algorytm Clarke-Wright
		static double clarkeWrightDiff = 0.0f; // Przechowuje sumę ścieżek dla Clarke-Wright
		if (ImGui::Button("Run##ClarkeWright")) {
			algorithmClarkeWright.set(experiment.getNodes(), experiment.getTrucks(), experiment.getDepot());
			algorithmClarkeWright.solve();
			clarkeWrightDiff = algorithmClarkeWright.getSumOfRoutes() - experiment.getOptimalValue(); // Pobranie sumy ścieżek
			algorithm.setTrucks(algorithmClarkeWright.getTrucks());
			drawTruckRoutes = true;
		}
		ImGui::SameLine();
		ImGui::Text("Algorithm Clarke-Wright");
		ImGui::SameLine();
		ImGui::InputDouble("##ClarkeWrightDiff", &clarkeWrightDiff, 0.0, 0.0, "%.2f", ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();
		ImGui::Text("(difference from optimum)");

		// Algorytm Clarke-Wright 2-opt
		static double clarkeWright2OptDiff = 0.0f; // Przechowuje sumę ścieżek dla Clarke-Wright 2-opt
		if (ImGui::Button("Run##ClarkeWright2Opt")) {
			algorithmClarkeWright.set(experiment.getNodes(), experiment.getTrucks(), experiment.getDepot());
			algorithmClarkeWright.solve();
			algorithmClarkeWright.twoOpt(); // Uruchomienie algorytmu 2-opt
			clarkeWright2OptDiff = algorithmClarkeWright.getSumOfRoutes() - experiment.getOptimalValue(); // Pobranie sumy ścieżek
			algorithm.setTrucks(algorithmClarkeWright.getTrucks());
			drawTruckRoutes = true;
		}
		ImGui::SameLine();
		ImGui::Text("Algorithm Clarke-Wright 2-opt");
		ImGui::SameLine();
		ImGui::InputDouble("##ClarkeWright2OptDiff", &clarkeWright2OptDiff, 0.0, 0.0, "%.2f", ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();
		ImGui::Text("(difference from optimum)");

		// Algorytm Simulated Annealing with Random Clients
		static double simulatedAnnealingRandomClientsDiff = 0.0f; // Przechowuje sumę ścieżek dla Simulated Annealing with Random Clients
		if (ImGui::Button("Run##SimulatedAnnealingRandomClients")) {
			algorithmSimulatedAnnealing.set(experiment.getNodes(), experiment.getTrucks(), experiment.getDepot());
			algorithmSimulatedAnnealing.setExperiment(experiment);
			algorithmSimulatedAnnealing.solveStartingWithRandomClientsAlgorithm();
			simulatedAnnealingRandomClientsDiff = algorithmSimulatedAnnealing.getSumOfRoutes() - experiment.getOptimalValue(); // Pobranie sumy ścieżek
			algorithm.setTrucks(algorithmSimulatedAnnealing.getTrucks());
			drawTruckRoutes = true;
		}
		ImGui::SameLine();
		ImGui::Text("Algorithm Simulated Annealing with Random Clients");
		ImGui::SameLine();
		ImGui::InputDouble("##SimulatedAnnealingRandomClientsDiff", &simulatedAnnealingRandomClientsDiff, 0.0, 0.0, "%.2f", ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();
		ImGui::Text("(difference from optimum)");

		// Algorytm Simulated Annealing with Greedy
		static double simulatedAnnealingGreedyDiff = 0.0f; // Przechowuje sumę ścieżek dla Simulated Annealing with Greedy
		if (ImGui::Button("Run##SimulatedAnnealingGreedy")) {
			algorithmSimulatedAnnealing.set(experiment.getNodes(), experiment.getTrucks(), experiment.getDepot());
			//algorithmSimulatedAnnealing.set(experiment.getNodes(), algorithmRandomClients.getTrucks(), experiment.getDepot());
			algorithmSimulatedAnnealing.setExperiment(experiment);
			algorithmSimulatedAnnealing.solveStartingWithGreedyAlgorithm();
			simulatedAnnealingGreedyDiff = algorithmSimulatedAnnealing.getSumOfRoutes() - experiment.getOptimalValue(); // Pobranie sumy ścieżek
			algorithm.setTrucks(algorithmSimulatedAnnealing.getTrucks());
			drawTruckRoutes = true;
		}
		ImGui::SameLine();
		ImGui::Text("Algorithm Simulated Annealing with Greedy");
		ImGui::SameLine();
		ImGui::InputDouble("##SimulatedAnnealingGreedyDiff", &simulatedAnnealingGreedyDiff, 0.0, 0.0, "%.2f", ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();
		ImGui::Text("(difference from optimum)");

		// Algorytm Simulated Annealing with Clarke-Wright
		static double simulatedAnnealingClarkeWrightDiff = 0.0f; // Przechowuje sumę ścieżek dla Simulated Annealing with Clarke-Wright
		if (ImGui::Button("Run##SimulatedAnnealingClarkeWright")) {
			algorithmSimulatedAnnealing.set(experiment.getNodes(), experiment.getTrucks(), experiment.getDepot());
			algorithmSimulatedAnnealing.setExperiment(experiment);
			algorithmSimulatedAnnealing.solveStartingWithClarkeWrightAlgorithm();
			simulatedAnnealingClarkeWrightDiff = algorithmSimulatedAnnealing.getSumOfRoutes() - experiment.getOptimalValue(); // Pobranie sumy ścieżek
			algorithm.setTrucks(algorithmSimulatedAnnealing.getTrucks());
			drawTruckRoutes = true;
		}
		ImGui::SameLine();
		ImGui::Text("Algorithm Simulated Annealing with Clarke-Wright");
		ImGui::SameLine();
		ImGui::InputDouble("##SimulatedAnnealingClarkeWrightDiff", &simulatedAnnealingClarkeWrightDiff, 0.0, 0.0, "%.2f", ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();
		ImGui::Text("(difference from optimum)");

		// Algorytm Tabu Search with Random Clients
		static double tabuSearchRandomClientsDiff = 0.0f; // Przechowuje sumę ścieżek dla Tabu Search with Random Clients
		if (ImGui::Button("Run##TabuSearchRandomClients")) {
			algorithmTabuSearch.set(experiment.getNodes(), experiment.getTrucks(), experiment.getDepot());
			algorithmTabuSearch.solveStartingWithRandomClientAlgorithm();
			tabuSearchRandomClientsDiff = algorithmTabuSearch.getSumOfRoutes() - experiment.getOptimalValue(); // Pobranie sumy ścieżek
			algorithm.setTrucks(algorithmTabuSearch.getTrucks());
			drawTruckRoutes = true;
		}
		ImGui::SameLine();
		ImGui::Text("Algorithm Tabu Search with Random Clients");
		ImGui::SameLine();
		ImGui::InputDouble("##TabuSearchRandomClientsDiff", &tabuSearchRandomClientsDiff, 0.0, 0.0, "%.2f", ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();
		ImGui::Text("(difference from optimum)");

		// Algorytm Tabu Search with Greedy
		static double tabuSearchGreedyDiff = 0.0f; // Przechowuje sumę ścieżek dla Tabu Search with Greedy
		if (ImGui::Button("Run##TabuSearchGreedy")) {
			algorithmTabuSearch.set(experiment.getNodes(), experiment.getTrucks(), experiment.getDepot());
			algorithmTabuSearch.solveStartingWithGreedyAlgorithm();
			tabuSearchGreedyDiff = algorithmTabuSearch.getSumOfRoutes() - experiment.getOptimalValue(); // Pobranie sumy ścieżek
			algorithm.setTrucks(algorithmTabuSearch.getTrucks());
			drawTruckRoutes = true;
		}
		ImGui::SameLine();
		ImGui::Text("Algorithm Tabu Search with Greedy");
		ImGui::SameLine();
		ImGui::InputDouble("##TabuSearchGreedyDiff", &tabuSearchGreedyDiff, 0.0, 0.0, "%.2f", ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();
		ImGui::Text("(difference from optimum)");

		// Algorytm Tabu Search with Clarke-Wright
		static double tabuSearchClarkeWrightDiff = 0.0f; // Przechowuje sumę ścieżek dla Tabu Search with Clarke-Wright
		if (ImGui::Button("Run##TabuSearchClarkeWright")) {
			algorithmTabuSearch.set(experiment.getNodes(), experiment.getTrucks(), experiment.getDepot());
			algorithmTabuSearch.solveStartingWithClarkeWrightAlgorithm();
			tabuSearchClarkeWrightDiff = algorithmTabuSearch.getSumOfRoutes() - experiment.getOptimalValue(); // Pobranie sumy ścieżek
			algorithm.setTrucks(algorithmTabuSearch.getTrucks());
			drawTruckRoutes = true;
		}
		ImGui::SameLine();
		ImGui::Text("Algorithm Tabu Search with Clarke-Wright");
		ImGui::SameLine();
		ImGui::InputDouble("##TabuSearchClarkeWrightDiff", &tabuSearchClarkeWrightDiff, 0.0, 0.0, "%.2f", ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();
		ImGui::Text("(difference from optimum)");

		// Dummy algorytm
		static double dummySum = 0.0f; // Przechowuje sumę ścieżek dla dummy algorytmu
		if (ImGui::Button("Run##Dummy")) {
			// Tutaj można dodać logikę dla dummy algorytmu
			dummySum = 0.0f; // Przykładowa wartość
		}
		ImGui::SameLine();
		ImGui::Text("Dummy Algorithm");
		ImGui::SameLine();
		ImGui::InputDouble("##dummysum", &dummySum, 0.0, 0.0, "%.2f", ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();
		ImGui::Text("(difference from optimum)");

		ImGui::End();

		// Renderowanie
		ImGui::Render();
		int w, h;
		glfwGetFramebufferSize(window, &w, &h);
		glViewport(0, 0, w, h);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
