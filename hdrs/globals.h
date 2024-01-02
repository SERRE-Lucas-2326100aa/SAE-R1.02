#ifndef GLOBALS_H
#define GLOBALS_H
#include "mingl/mingl.h"
#include "utils.h"
#include "level_manager.h"
#include <map>


enum GameState
{
    MAIN_MENU = 0,
    IN_LEVEL,
    IN_EDITOR
};

struct cursor_state
{
    int x,y;
    int last_click_x, last_click_y;
    bool is_clicking;
};

namespace glob_blob
{

    //config bools
    extern bool is_dev;
    static bool is_game_init = false;


    //ui configs
    extern nsGraphics::Vec2D win_size;
    extern cursor_state cursor;
    extern GameState menu_state;


    static unsigned int current_level = 0;
    static unsigned int points = 0;
    static unsigned int lives = 3;


    extern CMatrice current_game;
    extern VLevels levels;
    extern MinGL& main_window;
    //extern std::map<std::string, Button> buttons;

}


#endif // GLOBALS_H
