#pragma once

#include <random>
#include <cmath>

/**
 * Generates a random number following an exponential distribution with parameter lambda
 * @param lambda the rate parameter (must be positive)
 * @return a random number from the exponential distribution
 * @throws std::invalid_argument if lambda is non-positive
 */
double exponentielle(double lambda = 1.0) {
    if (lambda <= 0) {
        throw std::invalid_argument("Lambda must be positive");
    }
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    return -std::log(1.0 - dis(gen)) / lambda;
} 