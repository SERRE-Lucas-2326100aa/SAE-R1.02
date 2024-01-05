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


typedef unsigned short contenueDUneCase;
//typedef vector <contenueDUneCase> CVLigne; // un type représentant une ligne de la grille
//typedef vector <CVLigne> CMatrice; // un type représentant la grille

const contenueDUneCase KAIgnorer = 0;
const contenueDUneCase KPlusGrandNombreDansLaMatrice = 4;

//initialisation de la grille de jeu

// affichage de la matrice sans les numéros de lignes / colonnes en haut / à gauche
// affichage de la matrice sans les numéros de lignes / colonnes en haut / à gauche, mais avec un fond de couleur
//pour signifier que la case est a KAIgnorer

// affichage de la matrice avec les numéros de lignes / colonnes en haut / à gauche et avec un fond de couleur
//pour signifier que la case est a KAIgnorer
//***********************************************************************************/
//***********************    R1.01 – Prog#10 Exercice 2   ***************************/
//***********************************************************************************/

// on remplira cela plus tard, juste la définition de la fonction
void explositionUneBombeHorizontale (CMatrice & mat, const size_t & numLigne,
                                    const size_t & numColonne, const size_t & combien);
void explositionUneBombeVerticale (CMatrice& mat, const size_t& numLigne,const size_t& numCol, const size_t& cmb);

bool detectionExplositionUneBombeVerticale(CMatrice& mat)
{
   bool auMoinsUneExplosion (false);
   size_t combienALaSuite (1);
   //on parcours la matrice case / case
   // si on tombe sur la valeur KAIgnorer, on passe a la case suivante
   // sinon on compte combien de fois on a la même valeur
   for (size_t numCol = 0; numCol < mat[0].size(); ++numCol)
   {
    for (size_t numLigne = 0; numLigne < mat.size(); ++numLigne)
    {
        unsigned int _case = mat[numLigne][numCol];
        if (_case == KAIgnorer)
        {
            continue;
        }
        combienALaSuite = 1;

        for (size_t k = numLigne+1; k < mat.size() && mat[numLigne][numCol] == mat[k][numCol]; ++k)
            ++combienALaSuite;

        if (combienALaSuite >= 3){
                    auMoinsUneExplosion = true;
                    cout << "on a une suite en position colonne = " << numCol
                        << "; ligne = " << numLigne
                        << "; sur " << combienALaSuite << " cases" << endl;
                    cout << string (20, '-') << endl << "matrice avant suppresion" << endl;
                    //afficheMatriceV1(mat);
                    explositionUneBombeVerticale (mat, numLigne, numCol, combienALaSuite);
                    cout << string (20, '-') << endl << "matrice après suppresion" << endl;
                    //afficheMatriceV1(mat);
        }
    }
    }
   return auMoinsUneExplosion;
};

bool detectionExplositionUneBombeHorizontale (CMatrice & mat){
    bool auMoinsUneExplosion (false);
    //on parcours la matrice case / case
    // si on tombe sur la valeur KAIgnorer, on passe a la case suivante
    // sinon on compte combien de fois on a la même valeur
    for (size_t numLigne = 0; numLigne < mat.size(); ++numLigne)
    {
        for (size_t numCol = 0; numCol < mat[numLigne].size(); ++numCol)
        {
            unsigned int _case = mat[numLigne][numCol];
            if (_case == KAIgnorer)
            {
                continue;
            }
            size_t combienALaSuite (1);

            for (size_t k = numCol+1; k < mat[numLigne].size() && mat[numLigne][numCol] == mat[numLigne][k]; ++k)
                ++combienALaSuite;

            if (combienALaSuite >= 3){
                        auMoinsUneExplosion = true;
                        cout << "on a une suite en position numLigne = " << numLigne
                             << "; colonne = " << numCol
                             << "; sur  " << combienALaSuite << " cases" << endl;
                        cout << string (20, '-') << endl << "matrice avant suppresion" << endl;
                        //afficheMatriceV1(mat);
                        explositionUneBombeHorizontale (mat, numLigne, numCol, combienALaSuite);
                        cout << string (20, '-') << endl << "matrice après suppresion" << endl;
                        //afficheMatriceV1(mat);
            }
        }
    }
    return auMoinsUneExplosion;
    //si on a aun moins 3 chiffres identiques a la suite
};

//fait descendre toutes les cases d'une unité suite à une explosition
void explositionUneBombeHorizontale (CMatrice & mat, const size_t & numLigne,
                                    const size_t & numColonne, const size_t & combien)
{

    for (size_t nbcol = numColonne; nbcol < numColonne + combien; ++nbcol){
        for (size_t nbligne = numLigne; nbligne>0; --nbligne){
            mat [nbligne][nbcol] = mat[nbligne-1][nbcol];
        }
        mat [0][nbcol] =  KAIgnorer;
    }
}

void explositionUneBombeVerticale (CMatrice& mat, const size_t& numLigne,const size_t& numColonne, const size_t& combien)
{
    for (size_t k = numLigne; k < numLigne + combien; ++k)
    {
       mat[k][numColonne] = KAIgnorer;
    }

    for (size_t n = 0; n < combien; ++n)
    {
        for (size_t k = numLigne+(combien-1); k > 0; --k)
        {
            swap(mat[k][numColonne], mat[k-1][numColonne]);
        }
    }
}

/*

void faitUnMouvement (CMatrice & mat, const char & deplacment, const size_t & numLigne,
                     const size_t & numCol) {

    int offset_x = 0, offset_y = 0;
    switch (tolower(deplacment)) {
    case 'a':
        offset_x = -1;
        offset_y = 1;
        break;
    case 'z':
        offset_y = 1;
        break;
    case 'e':
        offset_x = 1;
        offset_y = 1;
        break;
    case 'q':
        offset_x = -1;
        break;
    case 'd':
        offset_x = 1;
        break;
    case 'x':
        offset_x = -1;
        offset_y = -1;
        break;
    case 'c':
        offset_y = -1;
        break;
    case 'v':
        offset_y = -1;
        offset_x = 1;
        break;
   //TODO : completer les autres mouvements
    default:
        cout<<"Tu choisis A ou Z ou E  ou Q ou D ou X ou C ou V"<<endl;
        break;
    }
    //faire la permutaion entre les 2 cases
    swap(mat[numLigne][numCol],mat[numLigne+offset_y][numCol+offset_x]);
}


int ppalExo03 (){
    CMatrice mat;
    initMat(mat);
    // affichage de la matrice sans les numéros de lignes / colonnes en haut / à gauche
    afficheMatriceV2 (mat);

    while (detectionExplositionUneBombeHorizontale (mat) || detectionExplositionUneBombeVerticale(mat))
    {
    }
    //swap(mat[7][1], mat[7][2]);
    //afficheMatriceV2 (mat);
    return 0;
}

int ppalExo04 (){

    CMatrice mat;
    initMat(mat);
    // affichage de la matrice sans les numéros de lignes / colonnes en haut / à gauche
    detectionExplositionUneBombeHorizontale (mat);
    afficheMatriceV2 (mat);
    //condition de victoire a trouver
    while (true) {
        cout << "Fait un mouvement ";
        cout << "numero de ligne : ";
        size_t numLigne;
        cin >> numLigne;
        cout << "numero de colonne : ";
        size_t numCol;
        cin >> numCol;
        cout << "Sens du deplacement : (A|Z|E|Q|D|V|X|C) : " << endl;
        char deplacement;
        cin >> deplacement;
        faitUnMouvement (mat, deplacement, numLigne, numCol);
        detectionExplositionUneBombeHorizontale (mat);
        afficheMatriceV2 (mat);
    }
    return 0;
}



 * Global variables shared across multiple cpp files
 **/


bool glob_blob::is_dev = false;
GameState glob_blob::menu_state;
std::map<std::string, Button> glob_blob::buttons;
cursor_state glob_blob::cursor {0,0,-1,-1,0,false};

int glob_blob::first_selected_row = -1;
int glob_blob::first_selected_column = -1;
std::string glob_blob::current_level;
MLevels glob_blob::levels;
chrono::microseconds glob_blob::delta_time;


void game_move(CMatrice& mat, int& new_row, int& new_col)
{
    if (new_row < 0 || new_col < 0)
        return;

    swap(mat[glob_blob::first_selected_row][glob_blob::first_selected_column], mat[new_row][new_col]);
    std::cout << "swapped" << std::endl;
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
                    nsGraphics::Vec2D window_size = window.getWindowSize();
                    Level& lvl = (*glob_blob::levels.find(glob_blob::current_level)).second;

                    int num_rows = (glob_blob::levels[glob_blob::current_level]).mat.size();
                    int num_cols = (glob_blob::levels[glob_blob::current_level]).mat[0].size();

                    nsGraphics::Vec2D board_top_left = {(window_size.getX() /2.f) - (4 * glob_blob::total_cell_size), (window_size.getY()/4.f)};
                    nsGraphics::Vec2D board_bottom_right = {board_top_left.getX() + glob_blob::dpi*(num_cols * glob_blob::total_cell_size), board_top_left.getY() + glob_blob::dpi*(num_rows * glob_blob::total_cell_size)};


                    auto reset_crusor_clicks = []()
                    {
                        glob_blob::cursor.click_count = 0;
                        glob_blob::first_selected_column = -1;
                        glob_blob::first_selected_row = -1;
                    };
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
                        std::cout << " after " << glob_blob::cursor.click_count << std::endl;

                        int clicked_col = (glob_blob::cursor.x- glob_blob::margin*4 - board_top_left.getX()) / (glob_blob::dpi * glob_blob::total_cell_size);
                        int clicked_row = (glob_blob::cursor.y- glob_blob::margin*4 - board_top_left.getY()) / (glob_blob::dpi * glob_blob::total_cell_size);



                        if (glob_blob::cursor.click_count == 2)
                        //&& (std::abs(glob_blob::first_selected_column - clicked_col) <= 1 && std::abs(glob_blob::first_selected_row - clicked_row) <= 1))
                        {
                            if ((std::abs(glob_blob::first_selected_column - clicked_col) <= 1 && std::abs(glob_blob::first_selected_row - clicked_row) <= 1) &&
                                (glob_blob::first_selected_column == clicked_col || glob_blob::first_selected_row == clicked_row) && // on compte pas le clique en diagonale
                                (glob_blob::first_selected_column != clicked_col || glob_blob::first_selected_row != clicked_row)) // on compte pas si c'estt le même, on deselectionne
                            {
                                std::cout << "not diagonal and good, swapping: " <<
                                    glob_blob::first_selected_column << "= " << clicked_col << " " <<
                                    glob_blob::first_selected_row << "= " << clicked_row << std::endl;
                                game_move(lvl.mat, clicked_row, clicked_col);


                            }
                            reset_crusor_clicks();
                        };


                        // le premier click
                        if (glob_blob::cursor.click_count == 1)
                        {
                            int col = (glob_blob::cursor.x- glob_blob::margin*4 - board_top_left.getX()) / (glob_blob::dpi * glob_blob::total_cell_size);
                            int row = (glob_blob::cursor.y- glob_blob::margin*4 - board_top_left.getY()) / (glob_blob::dpi * glob_blob::total_cell_size);
                            glob_blob::first_selected_column = col;
                            glob_blob::first_selected_row = row;
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
    if (argc == 2 && strcmp(argv[1], "dev") == 0)
    {
        std::cout << argv[0] << std::endl;
        glob_blob::is_dev = true;
    }

    nsGraphics::Vec2D win_size = {1080,720};
    nsGraphics::Vec2D win_pos = {128,128};

    //MinGL window("Number Crush", win_size, {128,128}, nsGraphics::KBlack);
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

        /*
         * Ici, écrivez votre logique d'affichage et de gestion des évènements
         */
        mouse_events(window);

        level_manager::dev_mode_draw(window,tr_engine);

        //tr_engine.update(delta_time);

        // On finit la frame en cours
        window.finishFrame();

        // On vide la queue d'évènements
        //window.getEventManager().clearEvents();

        // On attend un peu pour limiter le framerate et soulager le CPU
        this_thread::sleep_for(chrono::milliseconds(1000 / FPS_LIMIT) - chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - frame_start));

        // On récupère le temps de frame
        glob_blob::delta_time = chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - frame_start);
    }
}
