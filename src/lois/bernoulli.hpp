#pragma once


#include <random>
#include <chrono>

// Loi de Bernoulli : Attribution aléatoire d'un titre honorifique (présence/absence)
bool bernoulliTitleAttribution(double p) {
    // p = probabilité d'avoir un titre (entre 0 et 1)
    static std::mt19937 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::bernoulli_distribution distribution(p);
    return distribution(rng);
}

