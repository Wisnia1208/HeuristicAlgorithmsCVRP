#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib> // std::rand, std::srand
#include <ctime>   // std::time
#include <vector>  // std::vector
#include <cmath>   // std::sqrt, std::pow

void DrawLineBetweenPoints(ImDrawList* draw_list, const ImVec2& offset, const ImVec2& point1, const ImVec2& point2, ImU32 color, float thickness = 2.0f) {
    draw_list->AddLine(ImVec2(offset.x + point1.x, offset.y + point1.y),
        ImVec2(offset.x + point2.x, offset.y + point2.y),
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
    GLFWwindow* window = glfwCreateWindow(800, 600, "ImGui + GLFW", nullptr, nullptr);
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
    float grid_size = 75.0f; // Rozmiar pojedynczego kwadratu w pikselach
    int grid_count = 10; // Liczba kwadratów w wierszu/kolumnie

    // Punkty do rysowania linii
    ImVec2 closest_point1, closest_point2;
    bool has_closest_points = false;

    // Funkcja do randomizacji punktów
    auto randomize_points = [&]() {
        points.clear();
        has_closest_points = false; // Resetuj najbliższe punkty
        for (int i = 0; i < 20; i++) {
            float random_x = std::rand() % (grid_count * static_cast<int>(grid_size));
            float random_y = std::rand() % (grid_count * static_cast<int>(grid_size));
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
        ImGui::Begin("Moje Okienko");
        ImGui::Text("Hello from ImGui + GLFW!");

        // Dodanie przycisku do randomizacji punktów
        if (ImGui::Button("Randomizuj punkty")) {
            randomize_points();
        }

        // Dodanie przycisku do znajdowania najbliższych punktów
        if (ImGui::Button("Znajdź najbliższe punkty")) {
            find_closest_points();
        }

        ImGui::End();

        // Nowe okno z grafem
        ImGui::Begin("Graf 100x100");
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        ImVec2 p = ImGui::GetCursorScreenPos(); // Pozycja początkowa
        ImU32 grid_color = IM_COL32(200, 200, 200, 255); // Kolor siatki

        // Rysowanie siatki
        for (int i = 0; i <= grid_count; i++) {
            // Linie poziome
            float y = p.y + i * grid_size;
            draw_list->AddLine(ImVec2(p.x, y), ImVec2(p.x + grid_count * grid_size, y), grid_color);

            // Linie pionowe
            float x = p.x + i * grid_size;
            draw_list->AddLine(ImVec2(x, p.y), ImVec2(x, p.y + grid_count * grid_size), grid_color);
        }

        // Rysowanie czerwonych punktów
        ImU32 point_color = IM_COL32(255, 0, 0, 255); // Kolor punktów (czerwony)
        float point_radius = 5.0f; // Promień punktu
        for (const auto& point : points) {
            draw_list->AddCircleFilled(ImVec2(p.x + point.x, p.y + point.y), point_radius, point_color);
        }

        // W głównej pętli:
        if (has_closest_points) {
            ImU32 line_color = IM_COL32(0, 255, 0, 255); // Kolor linii (zielony)
            DrawLineBetweenPoints(draw_list, p, closest_point1, closest_point2, line_color);
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
