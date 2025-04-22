#pragma once

#include <cmath> // Dla std::sqrt
#include <vector>

struct Vec2D {
    float x, y;

    // Konstruktor domy�lny
    constexpr Vec2D() : x(0.0f), y(0.0f) {}

    // Konstruktor z parametrami
    constexpr Vec2D(float _x, float _y) : x(_x), y(_y) {}

    // Operator por�wnania
    bool operator==(const Vec2D& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Vec2D& other) const {
        return !(*this == other);
    }

    // Dodawanie wektor�w
    Vec2D operator+(const Vec2D& other) const {
        return Vec2D(x + other.x, y + other.y);
    }

    // Odejmowanie wektor�w
    Vec2D operator-(const Vec2D& other) const {
        return Vec2D(x - other.x, y - other.y);
    }

    // Skalowanie
    Vec2D operator*(float scalar) const {
        return Vec2D(x * scalar, y * scalar);
    }

    // Obliczanie d�ugo�ci wektora
    float length() const {
        return std::sqrt(x * x + y * y);
    }

    // Zamiana warto�ci
    void swap(Vec2D& other) noexcept {
        std::swap(x, other.x);
        std::swap(y, other.y);
    }
};
