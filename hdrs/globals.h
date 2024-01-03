#ifndef GLOBALS_H
#define GLOBALS_H
#include "mingl/mingl.h"
#include "utils.h"
#include "level_manager.h"
#include <map>


enum class GameState
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
    extern cursor_state last_cursor_state;
    extern GameState menu_state;
    extern std::map<std::string, Button> buttons;

    extern std::chrono::microseconds delta_time;
    static unsigned int points = 0;
    static unsigned int lives = 3;

    extern std::string current_level;
    extern CMatrice current_game;
    extern MLevels levels;
    extern MinGL& main_window;
    //extern std::map<std::string, Button> buttons;

}


#endif // GLOBALS_H
