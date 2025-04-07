#pragma once


#include <random>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>

// Loi de Poisson : Détermination du nombre de caractères spéciaux dans un pseudonyme
std::string poissonSpecialCharacters(double lambda, const std::string& baseName) {
    // lambda = nombre moyen de caractères spéciaux attendus
    static std::mt19937 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::poisson_distribution<int> distribution(lambda);
    int numSpecialChars = distribution(rng);
    
    // Liste de caractères spéciaux ASCII pour éviter les problèmes d'encodage
    std::vector<std::string> specialChars = {
        "*", "+", "~", "^", "!", "#", "$", "%", "&", "@", 
        "[", "]", "{", "}", "<", ">", "=", "-", "|", "/"
    };
    
    // Mélanger la liste pour une sélection aléatoire
    std::shuffle(specialChars.begin(), specialChars.end(), rng);
    
    // Générer le pseudonyme avec le nombre approprié de caractères spéciaux
    std::string decoratedName = baseName;
    
    // Ajouter les caractères spéciaux
    for (int i = 0; i < numSpecialChars && i < specialChars.size(); ++i) {
        // Décider aléatoirement si le caractère va avant ou après le nom
        std::bernoulli_distribution positionDist(0.5);
        if (positionDist(rng)) {
            decoratedName = specialChars[i] + decoratedName;
        } else {
            decoratedName = decoratedName + specialChars[i];
        }
    }
    
    return decoratedName;
}
