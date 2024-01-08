/**
 *
 * @file    level_manager.h
 * @author  Ersan MEHMED
 * @date    Décembre 2023
 * @version 0.1
 * @brief   Gestion de niveau/editeur
 **/
#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H
#include "mingl/mingl.h"
#include "mingl/transition/transition_engine.h"

using CMatrice = std::vector<std::vector<unsigned short>>; // Alias grille


/**
 * @struct Level
 * @brief Possède des données pour le niveau
 */
struct Level
{
    unsigned int lvl_num;  // Numéro du niveau
    unsigned int required_points; // Nombre de points requis pour finir le nv
    bool is_fully_loaded;
    std::string path; //Fichier du niveau
    CMatrice mat; //Matrice représentant la grille de jeu
};
/**
 * @struct GameSave
 * @brief Possède des données pour la sauvegarde du jeu, pas implementé
 */
struct GameSave
{
    unsigned int current_level; // Niveau actuel
    unsigned long max_points; // Max points atteint
};


/**
 * @brief BonBon_T : Liste de tout les types de bonbons implementé
 */
enum class BonBon_T : unsigned short
{
    Ignore = 0,
    NoDraw = 1,
    ToReplace,
    START,
    NormalBleu,
    NormalVert,
    NormalRouge,
    NormalOrange,
    MAX
};


// Alias MLevels = Level
using MLevels = std::map<std::string,Level>;
// Alias pour les transitions
using nsTransition::TransitionEngine;

/**
 * @namespace level_manager
 * @brief Espace de nom pour la gestion fonction lié aux niveaux;
 */
namespace level_manager
{
    // Fonction pour dessiner en mode développeur
    void dev_mode_draw(MinGL& window, TransitionEngine& engine);
    // Fonction pour charger un niveau
    Level load_level(const char* level_path, bool full_load);
    // Fonction pour charger tous les niveaux
    MLevels load_levels(const char* levels_path);
}


#endif // LEVEL_MANAGER_H
