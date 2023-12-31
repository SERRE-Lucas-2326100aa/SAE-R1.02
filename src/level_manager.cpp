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
using std::chrono::microseconds;
void level_manager::dev_mode_draw(MinGL& window, TransitionEngine& engine, microseconds delta_time)
{
    if (!glob_blob::is_dev)
        return;

    static bool loaded_first = false;

    // premier initialization, s'execute qu'une fois.
    if (!loaded_first)
    {
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


    //main_title.setPosition({(window_size.getX()/2) - main_title.computeWidth()/2, 40});




    window << main_title;
};


