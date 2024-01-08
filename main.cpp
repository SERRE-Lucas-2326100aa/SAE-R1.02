#include <iostream>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <cstring>


#define FPS_LIMIT 60

using namespace std;

#include "MinGL2_IUT_AIX/include/mingl/mingl.h"

#include "hdrs/globals.h"



/**
* Global variables shared across multiple cpp files
**/


bool glob_blob::is_dev = false;
GameState glob_blob::menu_state;
std::map<std::string, Button> glob_blob::buttons;

std::map<std::string, std::reference_wrapper<nsShape::Rectangle>> glob_blob::rect_map;
cursor_state glob_blob::cursor {0,0,-1,-1,0,false};

bool glob_blob::is_swapping = false;
bool glob_blob::is_swap_horizontal = false;
bool glob_blob::is_swap_vertical = false;

int glob_blob::first_selected_row = -1;
int glob_blob::first_selected_column = -1;
int glob_blob::last_selected_row = -1;
int glob_blob::last_selected_column = -1;

std::string glob_blob::current_level;
MLevels glob_blob::levels;
chrono::microseconds glob_blob::delta_time;
unsigned int glob_blob::current_points = 0;

const unsigned short KAIgnorer = static_cast<unsigned short>(BonBon_T::Ignore);
const unsigned short KAPasDessinerEtIgnorer = static_cast<unsigned short>(BonBon_T::NoDraw);

//initialisation de la grille de jeu

// affichage de la matrice sans les numéros de lignes / colonnes en haut / à gauche
// affichage de la matrice sans les numéros de lignes / colonnes en haut / à gauche, mais avec un fond de couleur
//pour signifier que la case est a KAIgnorer

// affichage de la matrice avec les numéros de lignes / colonnes en haut / à gauche et avec un fond de couleur
//pour signifier que la case est a KAIgnorer
// on remplira cela plus tard, juste la définition de la fonction
void explositionUneBombeHorizontale(CMatrice & mat, const size_t & numLigne,
                                    const size_t & numColonne, const size_t & combien);
void explositionUneBombeVerticale(CMatrice& mat, const size_t& numLigne,const size_t& numCol, const size_t& cmb);

void generer_bonbons(CMatrice& mat);


void generer_bonbons(CMatrice& mat)
{
    for (size_t ligne = 0; ligne < mat.size(); ++ligne)
    {
        for (size_t col = 0; col < mat[0].size(); ++col)
        {
            BonBon_T type_bonbon = static_cast<BonBon_T>(mat[ligne][col]);
            if (type_bonbon == BonBon_T::ToReplace)
            {
                unsigned short random = static_cast<unsigned short>(BonBon_T::START)+1 + rand() %
                                        (static_cast<unsigned short>(BonBon_T::MAX) - static_cast<unsigned short>(BonBon_T::START)-1);

                mat[ligne][col] = random;
            }
        }
    }
}


void show_mat(CMatrice& mat)
{
    for (size_t i = 0; i < mat.size(); ++i)
    {
        for (size_t j = 0; j < mat[0].size(); ++j)
        {
            cout << mat[i][j] << " ";
        }

        cout << endl;
    }

    cout << "-------------------------------" << endl;
}



explosion detectionExplositionUneBombeVerticale(CMatrice& mat)
{
    explosion exp{false, 0,0,0};
    size_t combienALaSuite = 1;
    //on parcours la matrice case / case
    // si on tombe sur la valeur KAIgnorer, on passe a la case suivante
    // sinon on compte combien de fois on a la même valeur
    for (size_t numCol = 0; numCol < mat[0].size(); ++numCol)
    {
        for (size_t numLigne = 0; numLigne < mat.size(); ++numLigne)
        {
            unsigned short _case = mat[numLigne][numCol];

            BonBon_T bonbon = static_cast<BonBon_T>(_case);

            if (bonbon == BonBon_T::Ignore || bonbon == BonBon_T::NoDraw)
            {
                continue;
            }

            combienALaSuite = 1;

            for (size_t k = numLigne+1; k < mat.size() && mat[numLigne][numCol] == mat[k][numCol]; ++k)
                ++combienALaSuite;

            if (combienALaSuite >= 3){
                exp.did_explode = true;
                exp.explosion_num = combienALaSuite;
                exp.start_col = numCol;
                exp.start_row = numLigne;
                glob_blob::current_points += 0.5*combienALaSuite;
                //cout << "on a une suite en position colonne = " << numCol
                //<< "; ligne = " << numLigne
                //<< "; sur " << combienALaSuite << " cases" << endl;
                //cout << string (20, '-') << endl << "matrice avant suppresion" << endl;
                //afficheMatriceV1(mat);
                explositionUneBombeVerticale (mat, numLigne, numCol, combienALaSuite);
                generer_bonbons(mat);
                //show_mat(mat);
                //cout << string (20, '-') << endl << "matrice après suppresion" << endl;
                //afficheMatriceV1(mat);
            }
        }
    }
    return exp;
};

explosion detectionExplositionUneBombeHorizontale (CMatrice & mat){
    explosion exp{false, 0,0,0};
    size_t combienALaSuite = 1;
    //on parcours la matrice case / case
    // si on tombe sur la valeur KAIgnorer, on passe a la case suivante
    // sinon on compte combien de fois on a la même valeur
    for (size_t numLigne = 0; numLigne < mat.size(); ++numLigne)
    {
        for (size_t numCol = 0; numCol < mat[numLigne].size(); ++numCol)
        {
            unsigned short _case = mat[numLigne][numCol];
            BonBon_T bonbon = static_cast<BonBon_T>(_case);

            if (bonbon == BonBon_T::Ignore || bonbon == BonBon_T::NoDraw)
            {
                continue;
            }

            combienALaSuite = 1;

            for (size_t k = numCol+1; k < mat[numLigne].size() && mat[numLigne][numCol] == mat[numLigne][k]; ++k)
                ++combienALaSuite;

            if (combienALaSuite >= 3){
                exp.did_explode = true;
                exp.explosion_num = combienALaSuite;
                exp.start_col = numCol;
                exp.start_row = numLigne;
                glob_blob::current_points +=  0.5*combienALaSuite;
                /*auMoinsUneExplosion = true;
                        cout << "on a une suite en position numLigne = " << numLigne
                             << "; colonne = " << numCol
                             << "; sur  " << combienALaSuite << " cases" << endl;
                        cout << string (20, '-') << endl << "matrice avant suppresion" << endl;*/
                //afficheMatriceV1(mat);
                //explositionUneBombeHorizontale (mat, numLigne, numCol, combienALaSuite);
                //generer_bonbons(mat);
                //show_mat(mat);
                //cout << string (20, '-') << endl << "matrice après suppresion" << endl;
                //afficheMatriceV1(mat);
            }
        }
    }
    return exp;
    //si on a aun moins 3 chiffres identiques a la suite
};



//fait descendre toutes les cases d'une unité suite à une explosition
void explositionUneBombeHorizontale (CMatrice & mat, const size_t & numLigne,
                                    const size_t & numColonne, const size_t & combien)
{

    for (size_t nbcol = numColonne; nbcol < numColonne + combien; ++nbcol){
        for (size_t nbligne = numLigne; nbligne>0; --nbligne){

            unsigned short _case = mat[nbligne][nbcol];
            BonBon_T bonbon = static_cast<BonBon_T>(_case);

            if (bonbon == BonBon_T::Ignore || bonbon == BonBon_T::NoDraw)
            {
                continue;
            }

            mat [nbligne][nbcol] = mat[nbligne-1][nbcol];

        }
        mat [0][nbcol] = static_cast<unsigned short>(BonBon_T::ToReplace);
    }
}

void explositionUneBombeVerticale (CMatrice& mat, const size_t& numLigne,const size_t& numColonne, const size_t& combien)
{
    for (size_t k = numLigne; k < numLigne + combien; ++k)
    {
        mat[k][numColonne] = static_cast<unsigned short>(BonBon_T::ToReplace);
    }

    for (size_t n = 0; n < combien; ++n)
    {
        for (size_t k = numLigne+(combien-1); k > 0; --k)
        {
            unsigned short _case = mat[k][numColonne];
            BonBon_T bonbon = static_cast<BonBon_T>(_case);

            if (bonbon == BonBon_T::Ignore || bonbon == BonBon_T::NoDraw)
            {
                break;
            }

            swap(_case, mat[k-1][numColonne]);
        }
    }
}


void reset_crusor_clicks()
{
    glob_blob::cursor.click_count = 0;
    glob_blob::first_selected_column = -1;
    glob_blob::first_selected_row = -1;
    glob_blob::last_selected_row = -1;
    glob_blob::last_selected_column = -1;
};

void game_move(CMatrice& mat, int& new_row, int& new_col)
{
    if (new_row < 0 || new_col < 0)
        return;

    swap(mat[glob_blob::first_selected_row][glob_blob::first_selected_column], mat[new_row][new_col]);

    //std::cout << "swapped" << std::endl;

    reset_crusor_clicks();
}


void mouse_events(MinGL& window)
{
    // On vérifie chaque évènement de la queue d'évènements
    while (window.getEventManager().hasEvent())
    {
        const nsEvent::Event_t actualEvent = window.getEventManager().pullEvent();
        // On regarde le type d'évènement
        switch (actualEvent.eventType)
        {
        case nsEvent::EventType_t::MouseMove:
            // Il s'agit d'un mouvement de souris
            glob_blob::cursor.x = actualEvent.eventData.moveData.x;
            glob_blob::cursor.y = actualEvent.eventData.moveData.y;
            break;

        case nsEvent::EventType_t::MouseClick:
            // Il s'agit d'un click de souris
            glob_blob::cursor.is_clicking = actualEvent.eventData.clickData.state == 0;

            // on vient de cliquer
            if (glob_blob::cursor.is_clicking == 0 && actualEvent.eventData.clickData.state == 1)
            {
                if (glob_blob::menu_state == GameState::MAIN_MENU)
                {
                    std::map<std::string, Button>::iterator it;
                    for (it = glob_blob::buttons.begin(); it != glob_blob::buttons.end(); ++it)
                    {
                        Button& btn = it->second;
                        if (btn.is_in(glob_blob::cursor.x, glob_blob::cursor.y))
                        {
                            btn.on_click();
                        }
                    }
                } else if (glob_blob::menu_state == GameState::IN_LEVEL)
                {
                    // handle main menu button click
                    std::map<std::string, Button>::iterator it = glob_blob::buttons.find("back_to_menu");
                    if (it != glob_blob::buttons.end())
                    {
                        Button& btn = it->second;
                        if (btn.is_in(glob_blob::cursor.x, glob_blob::cursor.y))
                        {
                            btn.on_click();
                        }
                    }

                    nsGraphics::Vec2D window_size = window.getWindowSize();
                    Level& lvl = (*glob_blob::levels.find(glob_blob::current_level)).second;

                    int num_rows = (glob_blob::levels[glob_blob::current_level]).mat.size();
                    int num_cols = (glob_blob::levels[glob_blob::current_level]).mat[0].size();

                    nsGraphics::Vec2D board_top_left = {(window_size.getX() /2.f) - (4 * glob_blob::total_cell_size), (window_size.getY()/4.f)};
                    nsGraphics::Vec2D board_bottom_right = {board_top_left.getX() + glob_blob::dpi*(num_cols * glob_blob::total_cell_size), board_top_left.getY() + glob_blob::dpi*(num_rows * glob_blob::total_cell_size)};

                    // if click inside the board
                    if (glob_blob::cursor.x >= board_top_left.getX() && glob_blob::cursor.x <= board_bottom_right.getX() &&
                        glob_blob::cursor.y >= board_top_left.getY() && glob_blob::cursor.y <= board_bottom_right.getY())
                    {
                        if (glob_blob::cursor.click_count <= 1 )
                            glob_blob::cursor.click_count++;
                        else
                        {
                            reset_crusor_clicks();
                        }
                        //std::cout << " after " << glob_blob::cursor.click_count << std::endl;

                        int clicked_col = (glob_blob::cursor.x- glob_blob::margin*4 - board_top_left.getX()) / (glob_blob::dpi * glob_blob::total_cell_size);
                        int clicked_row = (glob_blob::cursor.y- glob_blob::margin*4 - board_top_left.getY()) / (glob_blob::dpi * glob_blob::total_cell_size);

                        glob_blob::last_selected_column = clicked_col;
                        glob_blob::last_selected_row = clicked_row;

                        if (glob_blob::cursor.click_count == 2)
                        {
                            if ((std::abs(glob_blob::first_selected_column - clicked_col) <= 1 && std::abs(glob_blob::first_selected_row - clicked_row) <= 1) && // on regarde si la difference entre les 2 cases
                                (glob_blob::first_selected_column == clicked_col || glob_blob::first_selected_row == clicked_row) && // on compte pas le clique en diagonale
                                (glob_blob::first_selected_column != clicked_col || glob_blob::first_selected_row != clicked_row) && // on compte pas si c'estt le même, on deselectionne
                                (lvl.mat[clicked_row][clicked_col] != KAIgnorer  && lvl.mat[glob_blob::first_selected_row][glob_blob::first_selected_column] != KAIgnorer) &&  // on bouge pas les endroit à ignorer
                                (lvl.mat[clicked_row][clicked_col] != KAPasDessinerEtIgnorer  && lvl.mat[glob_blob::first_selected_row][glob_blob::first_selected_column] != KAPasDessinerEtIgnorer))
                            {
                                //std::cout << lvl.mat[clicked_row][clicked_col] << std::endl;

                                glob_blob::is_swapping = true;
                                glob_blob::is_swap_vertical = ((glob_blob::first_selected_row == clicked_row - 1 ||
                                                                 glob_blob::first_selected_row == clicked_row + 1) &&
                                                                 glob_blob::first_selected_column == clicked_col);

                                glob_blob::is_swap_horizontal = ((glob_blob::first_selected_column == clicked_col - 1 ||
                                                               glob_blob::first_selected_column == clicked_col + 1) &&
                                                              glob_blob::first_selected_row == clicked_row);

                                //std::cout << glob_blob::is_swap_horizontal << glob_blob::is_swap_vertical << std::endl;

                                //game_move(lvl.mat, clicked_row, clicked_col);
                            }
                            //reset_crusor_clicks();
                        };


                        // le premier click
                        if (glob_blob::cursor.click_count == 1)
                        {
                            glob_blob::first_selected_column = clicked_col;
                            glob_blob::first_selected_row = clicked_row;
                        }
                    }
                    else // en dehors du jeu
                    {
                        glob_blob::cursor.click_count = 0;
                        glob_blob::first_selected_column = -1;
                        glob_blob::first_selected_row = -1;
                    }
                }

                glob_blob::cursor.last_click_x = actualEvent.eventData.clickData.x;
                glob_blob::cursor.last_click_y = actualEvent.eventData.clickData.y;
            }
            break;

        default:
            // L'évènement ne nous intéresse pas
            break;
        }
    }
}



int main(int argc, char* argv[])
{
    (void)(argc); // warning unused
    (void)(argv);
    srand(time(nullptr));

    glob_blob::is_dev = true;

    nsGraphics::Vec2D win_size = {1080,720};
    nsGraphics::Vec2D win_pos = {128,128};

    MinGL window("Number Crush", win_size, win_pos, nsGraphics::KBlack);

    //glob_blob::main_window = window;

    window.initGlut();
    window.initGraphic();

    glutFullScreenToggle();

    glob_blob::levels = level_manager::load_levels("levels");

    glob_blob::menu_state = GameState::MAIN_MENU;

    nsTransition::TransitionEngine tr_engine;

    // Variable qui tient le temps de frame
    glob_blob::delta_time = chrono::microseconds::zero();

    while (window.isOpen())
    {
        chrono::time_point<chrono::steady_clock> frame_start = chrono::steady_clock::now();

        window.clearScreen();

        mouse_events(window);

        level_manager::dev_mode_draw(window,tr_engine);

        //tr_engine.update(delta_time);

        // On finit la frame en cours
        window.finishFrame();


        // On attend un peu pour limiter le framerate et soulager le CPU
        this_thread::sleep_for(chrono::milliseconds(1000 / FPS_LIMIT) - chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - frame_start));

        // On récupère le temps de frame
        glob_blob::delta_time = chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - frame_start);
    }
}
