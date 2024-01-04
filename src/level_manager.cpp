/**
 *
 * @file    level_manager.h
 * @author  Ersan MEHMED
 * @date    Décembre 2023
 * @version 0.1
 * @brief   Gestion de niveau/editeur
 **/
#include "hdrs/level_manager.h"
#include "hdrs/globals.h"
#include "hdrs/utils.h"


#include "mingl/shape/rectangle.h"
#include "mingl/shape/line.h"
#include "mingl/gui/text.h"
#include <filesystem>
#include <fstream>
#include <mingl/shape/circle.h>
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


void level_manager::dev_mode_draw(MinGL& window, TransitionEngine& engine)
{
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
        Level lvl = (*glob_blob::levels.find(glob_blob::current_level)).second;

        //std::cout << lvl.lvl_num << std::endl;

        // draw board
        //std::cout << lvl.mat.size() << std::endl;

        CMatrice& mat = lvl.mat; // alias

        int cell_size = 32;
        int margin = 2;
        float dpi = 1.5f;
        int total_cell_size = cell_size + margin;

        unsigned int num_rows = mat.size();
        unsigned int num_cols = mat[0].size();

        nsGraphics::Vec2D board_top_left = {(window_size.getX() /2.f) - (4 * total_cell_size), (window_size.getY()/4.f)};
        nsGraphics::Vec2D board_bottom_right = {board_top_left.getX() + dpi*(num_cols * total_cell_size), board_top_left.getY() + dpi*(num_rows *total_cell_size)};

        nsShape::Rectangle recttt(board_top_left, board_bottom_right, nsGraphics::KBlack);

        window << recttt;


        for (unsigned int row = 0; row < num_rows; ++row)
        {
            for (unsigned int col = 0; col < num_cols; ++col)
            {
                unsigned short nb = mat[row][col];

                BonBon_T bonbon_type = static_cast<BonBon_T>(nb);

                int x = board_top_left.getX() + margin * 4 + dpi*(col*total_cell_size);
                int y = board_top_left.getY() + margin * 4 + dpi*(row*total_cell_size);

                int x_end = x + cell_size;
                int y_end = y + cell_size;


                nsShape::Rectangle rect({x,y},{x_end,y_end}, nsGraphics::KRed);

                if (glob_blob::first_selected_column == col && glob_blob::first_selected_row == row)
                {
                    nsShape::Circle cc({x,y}, 5, nsGraphics::KYellow);
                    window  << cc;
                }

                switch(nb)
                {
                case 1:
                    rect.setFillColor(nsGraphics::KGreen);
                    break;
                case 2:
                    rect.setFillColor(nsGraphics::KBlue);
                    break;
                case 3:
                    rect.setFillColor(nsGraphics::KRed);
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


