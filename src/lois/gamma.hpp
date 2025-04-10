#pragma once

#include <random>
#include <cmath>
#include "exponentielle.hpp"

/**
 * Generates a random number following a gamma distribution with shape parameter k and scale parameter theta
 * @param k the shape parameter (must be positive)
 * @param theta the scale parameter (must be positive)
 * @return a random number from the gamma distribution
 * @throws std::invalid_argument if k or theta is non-positive
 */
double gamma(double k, double theta = 1.0) {
    if (k <= 0 || theta <= 0) {
        throw std::invalid_argument("k and theta must be positive");
    }

    if (k == 1.0) {
        return exponentielle(1.0 / theta);
    }

    double a = k;
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
            return x * theta;
        }

        double t = std::log(z);
        if (s >= t) {
            return x * theta;
        }
    }
}