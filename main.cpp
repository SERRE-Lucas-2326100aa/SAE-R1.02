#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <thread>
#include <cstring>


#define FPS_LIMIT 60

using namespace std;

#include "MinGL2_IUT_AIX/include/mingl/mingl.h"

#include "hdrs/globals.h"


void clearScreen () {
    cout << "\033[H\033[2J";
}

const unsigned KReset   (0);
const unsigned KRouge   (31);
const unsigned KVert    (32);
const unsigned KJaune   (33);
const unsigned KBleu    (34);
const unsigned KCyan    (36);

void couleur (const unsigned & coul) {
    cout << "\033[" << coul <<"m";
}


//***********************************************************************************/
//***********************    R1.01 – Prog#10 Exercice 2   ***************************/
//***********************************************************************************/
/*
 *
*/

typedef unsigned short contenueDUneCase;
typedef vector <contenueDUneCase> CVLigne; // un type représentant une ligne de la grille
typedef vector <CVLigne> CMatrice; // un type représentant la grille

const contenueDUneCase KAIgnorer = 0;
const contenueDUneCase KPlusGrandNombreDansLaMatrice = 4;

//initialisation de la grille de jeu
void initMat (CMatrice & mat, const size_t & nbLignes = 10,
             const size_t & nbColonnes = 10,
              const unsigned & nbMax= KPlusGrandNombreDansLaMatrice){
    mat.resize(nbLignes);

    for (auto& ligne : mat)
    {
        ligne.resize(nbColonnes);
        for (size_t i = 0; i < ligne.size(); ++i)
        {
            ligne[i] = (rand() % nbMax)+1;
        }
    }
}

// affichage de la matrice sans les numéros de lignes / colonnes en haut / à gauche
void  afficheMatriceV0 (const CMatrice & Mat) {
    cout << string (41, '-') << endl;
    for (auto& ligne : Mat)
    {
        for (size_t col = 0; col < ligne.size(); ++col)
        {
            if (col == 0)
            {
                couleur(KReset);
                cout << "|";
            }
            switch (ligne[col])
            {
                case 0:
                    break;
                case 1:
                {
                    couleur(KReset);
                    couleur(KRouge);
                    cout << " " << ligne[col] << " ";
                    couleur(KReset);
                    cout << "|";
                    break;
                }
                case 2:
                {
                    couleur(KReset);
                    couleur(KBleu);
                    cout << " " << ligne[col] << " ";
                    couleur(KReset);
                    cout << "|";
                    break;
                }
                case 3:
                {
                    couleur(KReset);
                    couleur(KJaune);
                    cout << " " << ligne[col] << " ";
                    couleur(KReset);
                    cout << "|";
                    break;
                }
                case 4:
                {
                    couleur(KReset);
                    couleur(KVert);
                    cout << " " << ligne[col] << " ";
                    couleur(KReset);
                    cout << "|";
                    break;
                }
            }
        }
        cout << endl;
    }
    cout << string (41, '-') << endl;
}

// affichage de la matrice sans les numéros de lignes / colonnes en haut / à gauche, mais avec un fond de couleur
//pour signifier que la case est a KAIgnorer
void  afficheMatriceV1 (const CMatrice & Mat) {
    cout << string (41, '-') << endl;
    for (auto& ligne : Mat)
    {
        for (size_t col = 0; col < ligne.size(); ++col)
        {
            if (col == 0)
            {
                couleur(KReset);
                cout << "|";
            }
            switch (ligne[col])
            {
                case 0:
                    couleur(KReset);
                    couleur(KCyan+10);
                    couleur(KCyan);
                    cout << " " << ligne[col] << " ";
                    couleur(KReset);
                    cout << "|";
                    break;
                case 1:
                {
                    couleur(KReset);
                    couleur(KRouge);
                    cout << " " << ligne[col] << " ";
                    couleur(KReset);
                    cout << "|";
                    break;
                }
                case 2:
                {
                    couleur(KReset);
                    couleur(KBleu);
                    cout << " " << ligne[col] << " ";
                    couleur(KReset);
                    cout << "|";
                    break;
                }
                case 3:
                {
                    couleur(KReset);
                    couleur(KJaune);
                    cout << " " << ligne[col] << " ";
                    couleur(KReset);
                    cout << "|";
                    break;
                }
                case 4:
                {
                    couleur(KReset);
                    couleur(KVert);
                    cout << " " << ligne[col] << " ";
                    couleur(KReset);
                    cout << "|";
                    break;
                }
            }
        }
        cout << endl;
    }
    cout << string (41, '-') << endl;
}


// affichage de la matrice avec les numéros de lignes / colonnes en haut / à gauche et avec un fond de couleur
//pour signifier que la case est a KAIgnorer
void  afficheMatriceV2 (const CMatrice & mat) {
    cout << "    0   1   2   3   4   5  6   7   8    9" << endl;
    cout << string (43, '-') << endl;
    for (size_t ligne = 0; ligne < mat.size(); ++ligne)
    {
        for (size_t col = 0; col < mat[ligne].size(); ++col)
        {
            if (col == 0)
            {
                couleur(KReset);
                cout << ligne << " |";
            }
            switch (mat[ligne][col])
            {
                case 0:
                    couleur(KReset);
                    couleur(KCyan+10);
                    couleur(KCyan);
                    cout << " " << mat[ligne][col] << " ";
                    couleur(KReset);
                    cout << "|";
                    break;
                case 1:
                {
                    couleur(KReset);
                    couleur(KRouge);
                    cout << " " << mat[ligne][col] << " ";
                    couleur(KReset);
                    cout << "|";
                    break;
                }
                case 2:
                {
                    couleur(KReset);
                    couleur(KBleu);
                    cout << " " << mat[ligne][col] << " ";
                    couleur(KReset);
                    cout << "|";
                    break;
                }
                case 3:
                {
                    couleur(KReset);
                    couleur(KJaune);
                    cout << " " << mat[ligne][col] << " ";
                    couleur(KReset);
                    cout << "|";
                    break;
                }
                case 4:
                {
                    couleur(KReset);
                    couleur(KVert);
                    cout << " " << mat[ligne][col] << " ";
                    couleur(KReset);
                    cout << "|";
                    break;
                }
            }
        }
        cout << endl;
    }
    cout << string (43, '-') << endl;
}


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
                    afficheMatriceV1(mat);
                    explositionUneBombeVerticale (mat, numLigne, numCol, combienALaSuite);
                    cout << string (20, '-') << endl << "matrice après suppresion" << endl;
                    afficheMatriceV1(mat);
        }
    }
    }
   return auMoinsUneExplosion;
}

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
                        afficheMatriceV1(mat);
                        explositionUneBombeHorizontale (mat, numLigne, numCol, combienALaSuite);
                        cout << string (20, '-') << endl << "matrice après suppresion" << endl;
                        afficheMatriceV1(mat);
            }
        }
    }
    return auMoinsUneExplosion;
    //si on a aun moins 3 chiffres identiques a la suite
}

//***********************************************************************************/
//***********************    R1.01 – Prog#10 Exercice 3   ***************************/
//***********************************************************************************/


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



/*
 * Global variables shared across multiple cpp files
 **/


bool glob_blob::is_dev = false;
cursor_state glob_blob::cursor {0,0,0,0,false};
VLevels glob_blob::levels {};


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
            glob_blob::cursor.last_click_x = actualEvent.eventData.clickData.x;
            glob_blob::cursor.last_click_y = actualEvent.eventData.clickData.y;
            glob_blob::cursor.is_clicking = !actualEvent.eventData.clickData.state;
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

    glob_blob::levels = level_manager::load_levels("levels");


    nsTransition::TransitionEngine tr_engine;

    // Variable qui tient le temps de frame
    chrono::microseconds delta_time = chrono::microseconds::zero();

    while (window.isOpen())
    {
        chrono::time_point<chrono::steady_clock> frame_start = chrono::steady_clock::now();

        window.clearScreen();

        /*
         * Ici, écrivez votre logique d'affichage et de gestion des évènements
         */
        mouse_events(window);

        level_manager::dev_mode_draw(window,tr_engine, delta_time);
        //level_manager::mouse_events(window);


        //tr_engine.update(delta_time);

        // On finit la frame en cours
        window.finishFrame();

        // On vide la queue d'évènements
        //window.getEventManager().clearEvents();

        // On attend un peu pour limiter le framerate et soulager le CPU
        this_thread::sleep_for(chrono::milliseconds(1000 / FPS_LIMIT) - chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - frame_start));

        // On récupère le temps de frame
        delta_time = chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - frame_start);
    }
}
