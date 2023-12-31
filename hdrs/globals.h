#ifndef GLOBALS_H
#define GLOBALS_H
#include "mingl/mingl.h"
#include "utils.h"
#include "level_manager.h"

namespace glob_blob
{

    //config bools
    extern bool is_dev;
    static bool is_game_init = false;


    //ui configs
    extern nsGraphics::Vec2D win_size;


    static unsigned int current_level = 0;
    static unsigned int points = 0;
    static unsigned int lives = 3;


    extern CMatrice current_game;
    extern CLevels levels;
    extern MinGL& main_window;

}


#endif // GLOBALS_H
