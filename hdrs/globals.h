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
    int click_count;
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


    inline int cell_size = 32;
    inline int margin = 2;
    inline float dpi = 1.5f;
    inline int total_cell_size = cell_size + margin;

    extern bool is_swapping, is_swap_horizontal, is_swap_vertical;

    extern std::map<std::string, Button> buttons;

    extern std::chrono::microseconds delta_time;

    // in game info
    extern unsigned int current_points;
    extern unsigned int moves;
    extern int first_selected_row, first_selected_column;
    extern int last_selected_row, last_selected_column;


    extern std::string current_level;
    extern CMatrice current_game;
    extern MLevels levels;
    extern MinGL& main_window;
    //extern std::map<std::string, Button> buttons;

}


#endif // GLOBALS_H
