#pragma once

#include <random>
#include <cmath>

class Exponentielle {
public:
    explicit Exponentielle(double lambda = 1.0) : m_lambda(lambda) {
        if (lambda <= 0) {
            throw std::invalid_argument("Lambda must be positive");
        }
    }

    double generate() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 1.0);
        return -std::log(1.0 - dis(gen)) / m_lambda;
    }

private:
    double m_lambda;
}; 