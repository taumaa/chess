#pragma once

#include <random>
#include <chrono>

// Loi uniforme : Génération d'un nombre aléatoire dans un intervalle [a, b]
template<typename T>
T uniforme(T a, T b) {
    // a = borne inférieure, b = borne supérieure
    static std::mt19937 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<T> distribution(a, b);
    return distribution(rng);
}

// Spécialisation pour les entiers
template<>
int uniforme<int>(int a, int b) {
    static std::mt19937 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> distribution(a, b);
    return distribution(rng);
} 