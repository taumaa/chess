#pragma once

#include <random>
#include <cmath>
#include "exponentielle.hpp"

class Gamma {
public:
    Gamma(double k, double theta = 1.0) : m_k(k), m_theta(theta) {
        if (k <= 0 || theta <= 0) {
            throw std::invalid_argument("k and theta must be positive");
        }
    }

    double generate() {
        if (m_k == 1.0) {
            Exponentielle exp(1.0 / m_theta);
            return exp.generate();
        }

        double a = m_k;
        double b = 1.0;
        double c = a + 1.0 / std::sqrt(2.0 * a - 1.0);
        double d = 1.0 + std::log(4.5);

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 1.0);

        while (true) {
            double u1 = dis(gen);
            double u2 = dis(gen);
            double v = std::log(u1 / (1.0 - u1)) / c;
            double x = a * std::exp(v);
            double z = u1 * u1 * u2;
            double r = b + a * v - x;
            double s = r + d - 4.5 * z;

            if (s + 1.0 + std::log(4.5) >= 5.0 * z) {
                return x * m_theta;
            }

            double t = std::log(z);
            if (s >= t) {
                return x * m_theta;
            }
        }
    }

private:
    double m_k;     // shape parameter
    double m_theta; // scale parameter
}; 