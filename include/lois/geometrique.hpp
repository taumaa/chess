#pragma once

#include <random>
#include <cmath>

class Geometrique {
public:
    explicit Geometrique(double p) : m_p(p) {
        if (p <= 0 || p > 1) {
            throw std::invalid_argument("p must be in (0,1]");
        }
    }

    int generate() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 1.0);
        return static_cast<int>(std::ceil(std::log(1.0 - dis(gen)) / std::log(1.0 - m_p)));
    }

private:
    double m_p; // probability of success
}; 