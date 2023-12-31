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


struct level
{
    unsigned int lvl_num;
    unsigned int required_points;
    char* background_image_path;
    bool locked;
};


//https://zoneasoluces.fr/le-guide-des-bonbons-de-candy-crush-saga/
enum class BonBon_T : unsigned int
{
    Ignore = 0,
    NormalBleu,
    NormalVert,
    NormalRouge,
    NormalOrange,
    Bombe,
    MAX
};

struct BonBon
{
    bool is_frozen;
    BonBon_T type;
};

using CLevels = std::vector<level>;
using nsTransition::TransitionEngine;

namespace level_manager
{
    void mouse_events(MinGL& window);
    void dev_mode_draw(MinGL& window, TransitionEngine& engine, std::chrono::microseconds);
    CMatrice load_level(unsigned int level);
    CLevels load_levels(const char* game_path);
    void start_game();
}


#endif // LEVEL_MANAGER_H
