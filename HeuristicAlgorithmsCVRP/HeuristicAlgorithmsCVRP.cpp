#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib> // std::rand, std::srand
#include <ctime>   // std::time
#include <vector>  // std::vector
#include <cmath>   // std::sqrt, std::pow
#include <string>

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

void DrawPoints(ImDrawList* draw_list, const ImVec2& cursorScreenPos, const std::vector<ImVec2>& points) {
	ImU32 point_color = IM_COL32(255, 0, 0, 255); // Kolor punktów (czerwony)
	float point_radius = 5.0f; // Promień punktu
	for (const auto& point : points) {
		draw_list->AddCircleFilled(
			ImVec2(cursorScreenPos.x + scalePoint(point.x),
				cursorScreenPos.y + GRID_SIZE * GRID_COUNT - scalePoint(point.y)),
			point_radius,
			point_color
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

	// Inicjalizacja generatora losowego
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	// Wektor przechowujący współrzędne punktów
	std::vector<ImVec2> points;
	float grid_size = GRID_SIZE; // Rozmiar pojedynczego kwadratu w pikselach
	int grid_count = GRID_COUNT; // Liczba kwadratów w wierszu/kolumnie

	// Punkty do rysowania linii
	ImVec2 closest_point1, closest_point2;
	bool has_closest_points = false;

	auto randomize_points = [&]() {
		points.clear();
		has_closest_points = false; // Resetuj najbliższe punkty
		for (int i = 0; i < 20; i++) {
			float random_x = std::rand() % 101; // Generowanie liczby z przedziału [0, 100]
			float random_y = std::rand() % 101; // Generowanie liczby z przedziału [0, 100]
			points.emplace_back(random_x, random_y);
		}
		};

	// Funkcja do obliczania odległości między dwoma punktami
	auto calculate_distance = [](const ImVec2& a, const ImVec2& b) {
		return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
		};


	// Funkcja do znajdowania dwóch najbliższych punktów
	auto find_closest_points = [&]() {
		if (points.size() < 2) return; // Nie można znaleźć najbliższych punktów, jeśli jest mniej niż 2 punkty
		float min_distance = std::numeric_limits<float>::max();
		for (size_t i = 0; i < points.size(); i++) {
			for (size_t j = i + 1; j < points.size(); j++) {
				float distance = calculate_distance(points[i], points[j]);
				if (distance < min_distance) {
					min_distance = distance;
					closest_point1 = points[i];
					closest_point2 = points[j];
				}
			}
		}
		has_closest_points = true;
		};

	// Generowanie punktów po raz pierwszy
	randomize_points();


	// Główna pętla
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		// Start ImGui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Okno GUI
		ImGui::Begin("Panel Sterowania", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Text("Hello from ImGui + GLFW!");

		// Dodanie przycisku do randomizacji punktów
		if (ImGui::Button("Randomizuj punkty")) {
			randomize_points();
			//for (const auto& point : points) {
			//	std::cout << "Point: (" << point.x << ", " << point.y << ")" << std::endl; // Debugowanie			
			//}
		}

		// Dodanie przycisku do znajdowania najbliższych punktów
		if (ImGui::Button("Znajdź najbliższe punkty")) {
			find_closest_points();
		}

		// Dodanie pola tekstowego do wprowadzania nazwy pliku
		static char filename[128] = ""; // Bufor na nazwę pliku
		ImGui::InputText("Nazwa pliku", filename, IM_ARRAYSIZE(filename));

		// Dodanie przycisku "Load"
		if (ImGui::Button("Load")) {
			// Na razie przycisk nie wykonuje żadnej akcji
			std::cout << "Kliknięto przycisk Load. Wprowadzona nazwa pliku: " << filename << std::endl;
		}

		ImGui::End();

		ImGui::SetNextWindowSize(ImVec2(850, 825));
		// Nowe okno z grafem
		ImGui::Begin("Graf 100x100", nullptr, ImGuiWindowFlags_NoResize);
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		ImVec2 cursorScreenPos = ImGui::GetCursorScreenPos(); // Pozycja początkowa
		cursorScreenPos.x += 75; // Przesunięcie w prawo
        
		DrawGridWithLabels(draw_list, cursorScreenPos, grid_size, grid_count); // Rysowanie siatki
		
		DrawPoints(draw_list, cursorScreenPos, points); // Rysowanie punktów na siatce
		
		// W głównej pętli:
		if (has_closest_points) {
			ImU32 line_color = IM_COL32(0, 255, 0, 255); // Kolor linii (zielony)
			DrawLineBetweenPoints(draw_list, cursorScreenPos, closest_point1, closest_point2, line_color);
		}

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
