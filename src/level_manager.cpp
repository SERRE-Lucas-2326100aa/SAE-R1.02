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


int get_scale_position(const int& start_x, const int& pos)
{
    return start_x + glob_blob::margin * 4 + glob_blob::dpi*(pos*glob_blob::total_cell_size);
};


// game functions
explosion detectionExplositionUneBombeHorizontale(CMatrice&);
explosion detectionExplositionUneBombeVerticale(CMatrice&); // forward declaration
void explositionUneBombeHorizontale(CMatrice & mat, const size_t & numLigne,
                                    const size_t & numColonne, const size_t & combien);
void explositionUneBombeVerticale(CMatrice& mat, const size_t& numLigne,const size_t& numCol, const size_t& cmb);

void game_move(CMatrice& mat, int& new_row, int& new_col);
void show_mat(CMatrice& mat);
void generer_bonbons(CMatrice& mat);


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
        // todo 4 bouttons pour jouer aux niveaux, creer un niveau, editer un niveau ou quitter

        static unsigned int cpt = 0;

        for (auto it = glob_blob::levels.begin(); ; ++it)
        {
            if (it == glob_blob::levels.end())
            {
                cpt = 0;
                break;
            }

            int line_x = 200*(cpt+1);
            int line_y = 100;

            if (cpt > 0 && cpt % 4 == 0)
            {
                line_y += 105;
            }

            Button btne({line_x, line_y}, 200,100, "Niveau " + std::to_string(it->second.lvl_num), nsGraphics::KBlack);

            btne.on_click = [it](){
                std::cout << " uwu " << it->first << std::endl;

                glob_blob::current_level = it->first;

                if (glob_blob::current_level.empty())
                    return;

                glob_blob::levels[glob_blob::current_level] = load_level(glob_blob::current_level.c_str(), true);

                glob_blob::menu_state = GameState::IN_LEVEL;

                glutSetCursor(GLUT_CURSOR_INHERIT);
            };

            glob_blob::buttons.insert({"lvl"+std::to_string(it->second.lvl_num), btne});
            window << btne;
            ++cpt;
        }

        break;
    }
    case GameState::IN_LEVEL:
    {
        Level& lvl = (*glob_blob::levels.find(glob_blob::current_level)).second; // le niveau est bien chargé normalement à partir de ce point là sinon ça crash :)

        CMatrice& mat = lvl.mat; // alias

        size_t num_rows = mat.size();
        size_t num_cols = mat[0].size();

        nsGraphics::Vec2D board_top_left = {(window_size.getX() /2.f) - (4 * glob_blob::total_cell_size), (window_size.getY()/4.f)};
        nsGraphics::Vec2D board_bottom_right = {board_top_left.getX() + glob_blob::dpi*(num_cols * glob_blob::total_cell_size), board_top_left.getY() + glob_blob::dpi*(num_rows * glob_blob::total_cell_size)};

        nsShape::Rectangle board(board_top_left, board_bottom_right, nsGraphics::KBlack);

        window << board;


        // affichage du nombre de points

        nsGraphics::Vec2D point_top_left = {40,100};
        nsGraphics::Vec2D point_bot_right = {300,180};

        nsShape::Rectangle points(point_top_left, point_bot_right, nsGraphics::KBlack);

        nsGui::Text pp({point_top_left.getX()+100,point_top_left.getY()+ 40},"Points: " + std::to_string(glob_blob::current_points) + " / " + std::to_string(lvl.required_points),nsGraphics::KYellow,
                               nsGui::GlutFont::BITMAP_9_BY_15, nsGui::Text::ALIGNH_CENTER);

        window << points;
        window << pp;

        // bouton retour au menu

        Button btn({40,200}, 200, 100, "Retour Menu", nsGraphics::KBlack);

        btn.on_click = [&](){
            lvl.mat[0].resize(0);
            lvl.mat.resize(0);
            //glob_blob::levels[glob_blob::current_level] = load_level(glob_blob::current_level.c_str(),true);

            glob_blob::menu_state = GameState::MAIN_MENU;
            glob_blob::current_points = 0;

            glutSetCursor(GLUT_CURSOR_INHERIT);
            return;
        };

        glob_blob::buttons.insert({"back_to_menu", btn});

        window << btn;


        //win text

        nsGui::Text txt({window_size.getX()/2.f,window_size.getY()/2.f},"VOUS AVEZ GAGNE",nsGraphics::KYellow,
            nsGui::GlutFont::BITMAP_9_BY_15, nsGui::Text::ALIGNH_CENTER);



        if (glob_blob::current_points >= lvl.required_points)
        {
            window << txt;
            return;
        }

        explosion explosion_v = detectionExplositionUneBombeVerticale(mat);
        explosion explosion_h = detectionExplositionUneBombeHorizontale(mat);

        for (size_t row = 0; row < num_rows; ++row)
        {
            for (size_t col = 0; col < num_cols; ++col)
            {
                unsigned short nb_bonbon = mat[row][col];

                // if second click

                int x = get_scale_position(board_top_left.getX(),col);
                int y = get_scale_position(board_top_left.getY(),row);

                int x_end = x + glob_blob::cell_size;
                int y_end = y + glob_blob::cell_size;

                if (glob_blob::first_selected_column == col && glob_blob::first_selected_row == row)
                {
                    nsShape::Circle cc({x,y}, 5, nsGraphics::KYellow);
                    window  << cc;
                }

                nsGraphics::RGBAcolor rect_color = nsGraphics::KTransparent;
                nsGraphics::RGBAcolor original_color;

                BonBon_T t_bonbon = static_cast<BonBon_T>(nb_bonbon);

                switch(t_bonbon)
                {
                case BonBon_T::NoDraw:
                case BonBon_T::Ignore:
                    rect_color = nsGraphics::KTransparent;
                    original_color = nsGraphics::KTransparent;
                    break;
                case BonBon_T::NormalVert:
                    rect_color = nsGraphics::KGreen;
                    original_color = nsGraphics::KGreen;
                    break;
                case BonBon_T::NormalBleu:
                    rect_color = nsGraphics::KBlue;
                    original_color = nsGraphics::KBlue;
                    break;
                case BonBon_T::NormalRouge:
                    rect_color = nsGraphics::KRed;
                    original_color = nsGraphics::KRed;
                    break;
                case BonBon_T::NormalOrange:
                    rect_color = nsGraphics::KYellow;
                    original_color = nsGraphics::KYellow;
                    break;
                }

                // mouvements
                static std::string id_move_x = "h_swap";
                static std::string id_move_y = "v_swap";
                static std::string h_swap_id = "h_swap_whole";
                static std::string dissapear_id_h = "dissappear_anim";

                static bool reset_anim = false;
                static bool reset_anim_whole = false;

                if (!glob_blob::is_swapping && reset_anim)
                {
                    // reset animations
                    float st = animations::fast_float_lerp(id_move_x, false, 0.f, 1.f, 1.f);
                    animations::fast_float_lerp(id_move_y, false, 0.f, 1.f, 1.f);

                    if (st >= 1.f)
                        reset_anim = false;
                }

                if (reset_anim_whole)
                {
                    float xt = animations::fast_float_lerp(h_swap_id,false,0.f, 1.f,100.f);
                    float wt = animations::fast_float_lerp(dissapear_id_h,false,0.f, 0.5f,100.f);

                    if (xt == 0.f && wt == 0.f)
                    {
                        reset_anim_whole = false;
                    }
                }

                // animation du cube en mouvement horizontale
                if (glob_blob::is_swap_horizontal)
                {
                    float anim_stage = animations::fast_float_lerp(id_move_x, glob_blob::is_swapping, 0.f, 1.f, 0.08f);

                    bool is_first_selected = row == glob_blob::first_selected_row && col == glob_blob::first_selected_column;
                    bool is_last_selected = row == glob_blob::last_selected_row && col == glob_blob::last_selected_column;

                    if (is_first_selected || is_last_selected) {
                        int new_x = is_first_selected ? get_scale_position(board_top_left.getX(), glob_blob::last_selected_column) :
                                        get_scale_position(board_top_left.getX(), glob_blob::first_selected_column);
                        int new_end_x = new_x + glob_blob::cell_size;

                        x = animations::lerp(x, new_x, anim_stage);
                        x_end = animations::lerp(x_end, new_end_x, anim_stage);
                    }

                    if (anim_stage >= 0.9f) {
                        game_move(mat, glob_blob::last_selected_row, glob_blob::last_selected_column);
                        reset_anim = true;
                        glob_blob::is_swapping = false;
                        glob_blob::is_swap_horizontal = false;
                    }
                }
                // animation du cube en mouvement vertical
                if (glob_blob::is_swap_vertical)
                {
                    float anim_stage = animations::fast_float_lerp(id_move_y, glob_blob::is_swapping, 0.f, 1.f, 0.08f);

                    bool is_first_selected = row == glob_blob::first_selected_row && col == glob_blob::first_selected_column;
                    bool is_last_selected = row == glob_blob::last_selected_row && col == glob_blob::last_selected_column;

                    if (is_first_selected || is_last_selected) {
                        int new_y = is_first_selected ? get_scale_position(board_top_left.getY(), glob_blob::last_selected_row) :
                                        get_scale_position(board_top_left.getY(), glob_blob::first_selected_row);
                        int new_end_y = new_y + glob_blob::cell_size;

                        y = animations::lerp(y, new_y, anim_stage);
                        y_end = animations::lerp(y_end, new_end_y, anim_stage);
                    }

                    if (anim_stage >= 0.9f) {
                        game_move(mat, glob_blob::last_selected_row, glob_blob::last_selected_column);
                        reset_anim = true;
                        glob_blob::is_swapping = false;
                        glob_blob::is_swap_vertical = false;
                    }
                }

                float anim_stage = animations::fast_float_lerp(h_swap_id,explosion_h.did_explode,0.f, 1.f,0.04f);
                float d_stage = animations::fast_float_lerp(dissapear_id_h,explosion_h.did_explode,0.f, 0.5f,0.08f);

                // horizontal pushing
                if (explosion_h.did_explode)
                {
                    // push the lines
                    for (size_t nbcol = explosion_h.start_col; nbcol < explosion_h.start_col + explosion_h.explosion_num; ++nbcol)
                    {
                        for (size_t nbligne = explosion_h.start_row; nbligne>0; --nbligne)
                        {
                            if (nbcol == col && row == nbligne-1)
                            {

                                int new_y = get_scale_position(board_top_left.getY(), nbligne);
                                int new_end_y = new_y + glob_blob::cell_size;

                                y = animations::lerp(y, new_y, anim_stage);
                                y_end = animations::lerp(y_end, new_end_y, anim_stage);
                                rect_color = original_color;
                            }
                            else if (nbcol == col && row == explosion_h.start_row && anim_stage <= 0.4)
                            {
                                rect_color = animations::lerp_color(rect_color, nsGraphics::KTransparent, d_stage);
                            };
                        }
                    }

                    if (anim_stage >= 1.f && !reset_anim_whole)
                    {
                        reset_anim_whole = true;
                        explositionUneBombeHorizontale(mat,explosion_h.start_row,explosion_h.start_col, explosion_h.explosion_num);
                        generer_bonbons(mat);
                    }
                }


                nsShape::Rectangle rect({x,y},{x_end,y_end}, rect_color);


                rect.setFillColor(rect_color);

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


