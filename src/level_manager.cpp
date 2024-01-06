/**
 *
 * @file    level_manager.h
 * @author  Ersan MEHMED
 * @date    Décembre 2023
 * @version 0.1
 * @brief   Gestion de niveau/editeur
 **/

#include <filesystem>
#include <fstream>
#include <mingl/shape/circle.h>

#include "hdrs/level_manager.h"
#include "hdrs/globals.h"
#include "mingl/shape/rectangle.h"
#include "mingl/gui/text.h"
#include "hdrs/animations.h"

using std::chrono::microseconds, std::filesystem::path;

Level level_manager::load_level(const char* level_path, bool fully_load)
{
    Level lvl;

    std::ifstream lvl_file(level_path);
    std::cout << "loading " << level_path << std::endl;

    if (lvl_file.fail())
    {
        throw std::runtime_error("failed to open lvl, gg");
    }

    lvl.path = level_path;

    lvl_file >> lvl.lvl_num;

    size_t row, column, current_row = 0, current_column = 0;
    lvl_file >> row;
    lvl_file >> column;

    lvl_file >> lvl.required_points;

    std::string line;

    if (!fully_load)
        lvl.is_fully_loaded = false;

    if (fully_load)
    {
        unsigned int bonbon;
        lvl.mat.resize(row);

        for (size_t i = 0; i < row; ++i)
        {
            lvl.mat[i].resize(column);
        }

        while (lvl_file >> bonbon)
        {
            if (current_column >= column)
            {
                current_column = 0;
                ++current_row;
                std::cout << std::endl;
            }
            std::cout << bonbon;
            lvl.mat[current_row][current_column] = bonbon;
            ++current_column;
        }

    }
    lvl_file.close();
    return lvl;
};


void show_loaded_levels(MLevels& levels)
{
    for (const auto& lvl : levels)
    {
        std::cout << lvl.second.path << std::endl;
    }
}


MLevels level_manager::load_levels(const char* levels_path)
{
    MLevels level_maps;

    path game_path = std::filesystem::current_path();
    game_path.append(levels_path);

    bool path_exists = std::filesystem::exists(game_path);

    if (!path_exists)
    {
        std::filesystem::create_directory(game_path);
        return level_maps;
    }

    for (const auto& entry : std::filesystem::directory_iterator(game_path))
    {
        level_maps[entry.path()] = load_level(entry.path().c_str(), false);
    }

    return level_maps;
};


// game functions
bool detectionExplositionUneBombeHorizontale(CMatrice&);
bool detectionExplositionUneBombeVerticale(CMatrice&); // forward declaration
void game_move(CMatrice& mat, int& new_row, int& new_col);


void level_manager::dev_mode_draw(MinGL& window, TransitionEngine& engine)
{
    (void)(engine); // unsued engine warning

    if (!glob_blob::is_dev)
        return;

    static bool loaded_first = false;

    // premier initialization, s'execute qu'une fois.
    if (!loaded_first)
    {
        show_loaded_levels(glob_blob::levels);
        //load_level("/home/def/Desktop/SAE-R1.02/build/levels/nivo_un_jecrois.txt",true);
        // transition de fond
        loaded_first = true;
    }


    nsGraphics::Vec2D window_position = window.getWindowPosition();
    nsGraphics::Vec2D window_size = window.getWindowSize();
    nsShape::Rectangle rect = {{0,0},
                               {window_size.getX(),
                                window_size.getY()}, nsGraphics::KWhite};

    window << rect;

    nsGui::Text main_title({window_size.getX()/2,40},"Number Crush",nsGraphics::KBlack,
                           nsGui::GlutFont::BITMAP_9_BY_15, nsGui::Text::ALIGNH_CENTER);

    switch (glob_blob::menu_state)
    {
    case GameState::MAIN_MENU:
    {
        Button btn({200,100}, 200, 100, "Niveau 1", nsGraphics::KBlack);

        btn.on_click = [](){
            std::cout << "yeah changing this shit" << std::endl;

            glob_blob::current_level = "/home/def/Desktop/SAE-R1.02/build/levels/nivo_un_jecrois.txt";
            glob_blob::levels[glob_blob::current_level] = load_level(glob_blob::current_level.c_str(),true);

            glob_blob::menu_state = GameState::IN_LEVEL;
            glutSetCursor(GLUT_CURSOR_INHERIT);
        };

        glob_blob::buttons.insert({"lvl1", btn});
        window << btn;
        break;
    }
    case GameState::IN_LEVEL:
    {
        Level& lvl = (*glob_blob::levels.find(glob_blob::current_level)).second;

        //std::cout << lvl.lvl_num << std::endl;

        // draw board
        //std::cout << lvl.mat.size() << std::endl;

        CMatrice& mat = lvl.mat; // alias

        int num_rows = mat.size();
        int num_cols = mat[0].size();

        nsGraphics::Vec2D board_top_left = {(window_size.getX() /2.f) - (4 * glob_blob::total_cell_size), (window_size.getY()/4.f)};
        nsGraphics::Vec2D board_bottom_right = {board_top_left.getX() + glob_blob::dpi*(num_cols * glob_blob::total_cell_size), board_top_left.getY() + glob_blob::dpi*(num_rows * glob_blob::total_cell_size)};

        nsShape::Rectangle recttt(board_top_left, board_bottom_right, nsGraphics::KBlack);

        window << recttt;

        detectionExplositionUneBombeVerticale(mat);
        detectionExplositionUneBombeHorizontale(mat);


        for (int row = 0; row < num_rows; ++row)
        {
            for (int col = 0; col < num_cols; ++col)
            {
                unsigned short nb = mat[row][col];

                //BonBon_T bonbon_type = static_cast<BonBon_T>(nb);

                // if second click

                int x = board_top_left.getX() + glob_blob::margin * 4 + glob_blob::dpi*(col*glob_blob::total_cell_size);
                int y = board_top_left.getY() + glob_blob::margin * 4 + glob_blob::dpi*(row*glob_blob::total_cell_size);

                int x_end = x + glob_blob::cell_size;
                int y_end = y + glob_blob::cell_size;

                if (glob_blob::first_selected_column == col && glob_blob::first_selected_row == row)
                {
                    nsShape::Circle cc({x,y}, 5, nsGraphics::KYellow);
                    window  << cc;
                }

                // mouvements
                static std::string id_move_x = "h_swap";
                static std::string id_move_y = "v_swap";

                static bool reset_anim = false;

                if (!glob_blob::is_swapping && reset_anim)
                {
                    // reset animations
                    float st = animations::fast_float_lerp(id_move_x, false, 0.f, 1.f, 1.f);
                    animations::fast_float_lerp(id_move_y, false, 0.f, 1.f, 1.f);

                    if (st == 1.f)
                        reset_anim = false;
                }

                if (glob_blob::is_swap_horizontal)
                {
                    float anim_stage = animations::fast_float_lerp(id_move_x, glob_blob::is_swapping, 0.f, 1.f, 0.08f);

                    if (row == glob_blob::first_selected_row && col == glob_blob::first_selected_column)
                    {
                        int new_x = board_top_left.getX() + glob_blob::margin * 4 + glob_blob::dpi*(glob_blob::last_selected_column*glob_blob::total_cell_size);
                        int new_end_x = new_x + glob_blob::cell_size;

                        x = animations::lerp(x, new_x, anim_stage);
                        x_end = animations::lerp(x_end, new_end_x, anim_stage);
                    }
                    if (row == glob_blob::last_selected_row && col == glob_blob::last_selected_column)
                    {
                        int new_x = board_top_left.getX() + glob_blob::margin * 4 + glob_blob::dpi*(glob_blob::first_selected_column*glob_blob::total_cell_size);
                        int new_end_x = new_x + glob_blob::cell_size;

                        x = animations::lerp(x, new_x, anim_stage);
                        x_end = animations::lerp(x_end, new_end_x, anim_stage);
                    }

                    if (anim_stage > 0.98f)
                    {
                        game_move(mat, glob_blob::last_selected_row, glob_blob::last_selected_column);
                        reset_anim = true;
                        glob_blob::is_swapping = false;
                        glob_blob::is_swap_horizontal = false;
                    }
                }

                if (glob_blob::is_swap_vertical)
                {
                    float anim_stage = animations::fast_float_lerp(id_move_y, glob_blob::is_swapping, 0.f, 1.f, 0.08f);

                    if (row == glob_blob::first_selected_row && col == glob_blob::first_selected_column)
                    {
                        int new_y = board_top_left.getY() + glob_blob::margin * 4 + glob_blob::dpi*(glob_blob::last_selected_row*glob_blob::total_cell_size);
                        int new_end_y = new_y + glob_blob::cell_size;

                        y = animations::lerp(y, new_y, anim_stage);
                        y_end = animations::lerp(y_end, new_end_y, anim_stage);
                    }
                    if (row == glob_blob::last_selected_row && col == glob_blob::last_selected_column)
                    {
                        int new_y = board_top_left.getY() + glob_blob::margin * 4 + glob_blob::dpi*(glob_blob::first_selected_row*glob_blob::total_cell_size);
                        int new_end_y = new_y + glob_blob::cell_size;

                        y = animations::lerp(y, new_y, anim_stage);
                        y_end = animations::lerp(y_end, new_end_y, anim_stage);
                    }

                    if (anim_stage >= 0.98f)
                    {
                        game_move(mat, glob_blob::last_selected_row, glob_blob::last_selected_column);
                        reset_anim = true;
                        glob_blob::is_swapping = false;
                        glob_blob::is_swap_vertical = false;
                    }
                }


                nsShape::Rectangle rect({x,y},{x_end,y_end}, nsGraphics::KTransparent);

                switch(nb)
                {
                case 0:
                    rect.setFillColor(nsGraphics::KTransparent);
                    break;
                case 1:
                    rect.setFillColor(nsGraphics::KGreen);
                    break;
                case 2:
                    rect.setFillColor(nsGraphics::KBlue);
                    break;
                case 3:
                    rect.setFillColor(nsGraphics::KRed);
                    break;
                case 4:
                    rect.setFillColor(nsGraphics::KYellow);
                    break;
                }

                window << rect;

            }
        }



        break;
    }
    case GameState::IN_EDITOR:
        break;
    };

    window << main_title;


};


