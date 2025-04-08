#pragma once

#include <random>
#include <chrono>
#include "imgui.h"
#include <utility>
#include <algorithm>

inline std::pair<ImVec4, ImVec4> normaleChessboardColors(float sigma) {
    
    static std::mt19937 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::normal_distribution<float> distribution(0.0f, sigma);
    
    // Couleurs de base
    ImVec4 baseDarkSquare(0.5f, 0.3f, 0.0f, 1.0f);
    ImVec4 baseLightSquare(0.9f, 0.8f, 0.6f, 1.0f);
    
    ImVec4 darkSquare = baseDarkSquare;
    ImVec4 lightSquare = baseLightSquare;
    

    darkSquare.x = std::clamp(baseDarkSquare.x + distribution(rng), 0.3f, 0.7f);   // R
    darkSquare.y = std::clamp(baseDarkSquare.y + distribution(rng), 0.1f, 0.5f);   // G
    darkSquare.z = std::clamp(baseDarkSquare.z + distribution(rng), 0.0f, 0.2f);   // B
    
    // Variation pour la couleur claire
    lightSquare.x = std::clamp(baseLightSquare.x + distribution(rng), 0.7f, 1.0f); // R
    lightSquare.y = std::clamp(baseLightSquare.y + distribution(rng), 0.6f, 0.9f); // G
    lightSquare.z = std::clamp(baseLightSquare.z + distribution(rng), 0.4f, 0.8f); // B
    
    darkSquare.w = 1.0f;
    lightSquare.w = 1.0f;
    
    return std::make_pair(lightSquare, darkSquare);
}
