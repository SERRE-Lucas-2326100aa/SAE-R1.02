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
#include "utils.h"


struct BonBon; // declaration avancé
using CMatrice = std::vector<std::vector<unsigned short>>; // grille

struct Level
{
    unsigned int lvl_num;
    unsigned int required_points;
    bool locked;
    bool is_fully_loaded;
    std::string path;
    CMatrice mat;
};

struct GameSave
{
    unsigned int current_level;
    unsigned long max_points;
};


//https://zoneasoluces.fr/le-guide-des-bonbons-de-candy-crush-saga/
enum class BonBon_T : unsigned short
{
    Ignore = 0,
    NoDraw,
    START,
    NormalBleu,
    NormalVert,
    NormalRouge,
    NormalOrange,
    MAX
};

struct BonBon
{
    bool is_selected;
    BonBon_T type;

    friend std::ostream& operator<<(std::ostream& os, const BonBon& b)
    {
        os << static_cast<unsigned int>(b.type);
        return os;
    };
};

using MLevels = std::map<std::string,Level>;
using nsTransition::TransitionEngine;

namespace level_manager
{
    void dev_mode_draw(MinGL& window, TransitionEngine& engine);
    Level load_level(const char* level_path, bool full_load);
    MLevels load_levels(const char* levels_path);
    void start_game();
}


#endif // LEVEL_MANAGER_H
