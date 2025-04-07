#pragma once

#include <random>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>

// Loi binomiale : Génération de pseudonymes avec un nombre variable de qualificatifs
std::string binomialNicknameGenerator(int n, double p) {
    // n = nombre maximum de qualificatifs possibles
    // p = probabilité de sélection de chaque qualificatif (entre 0 et 1)
    
    static std::mt19937 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::binomial_distribution<int> distribution(n, p);
    int numTitles = distribution(rng);
    
    // Liste de qualificatifs possibles
    std::vector<std::string> titles = {
        "Maître", "Grand", "Stratège", "Tacticien", "Virtuose", 
        "Génie", "Champion", "Prodige", "Visionnaire", "Calculateur",
        "Légendaire", "Royal", "Immortel", "Invincible", "Mystique"
    };
    
    // Mélanger la liste pour une sélection aléatoire
    std::shuffle(titles.begin(), titles.end(), rng);
    
    // Générer le pseudonyme avec le nombre approprié de qualificatifs
    std::string nickname = "";
    for (int i = 0; i < numTitles && i < titles.size(); ++i) {
        nickname += titles[i] + " ";
    }
    
    // Ajouter un nom de base si aucun qualificatif n'a été sélectionné
    if (nickname.empty()) {
        nickname = "Joueur";
    }
    
    return nickname;
}
