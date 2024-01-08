#ifndef GLOBALS_H
#define GLOBALS_H
#include "mingl/mingl.h"
#include "mingl/shape/rectangle.h"
#include "utils.h"
#include "level_manager.h"
#include <map>



/**
 * @brief GameState : Liste de tout les types d'états de Jeu
 */
enum class GameState
{
    MAIN_MENU = 0,
    IN_LEVEL,
    IN_EDITOR
};

/**
 * @struct cursor_state
 * @brief Possède des données pour l'évènement curseur
 */
struct cursor_state
{
    int x, y; // Position du curseur
    int last_click_x, last_click_y; // Dernière pos du clic du curseur
    int click_count; // Nombre de clics
    bool is_clicking; // Clic en cours
};

/**
 * @namespace glob_blob
 * @brief Espace de nom pour la gestion de variable globale, utilisé dans différents fichier src;
 */
namespace glob_blob
{

    //config bools
    extern bool is_dev;


    //ui configs
    extern nsGraphics::Vec2D win_size; // Taille de la fenêtre
    extern cursor_state cursor; // État du curseur

    extern GameState menu_state; // État du menu


    inline int cell_size = 32; // Taille d'une cellule
    inline int margin = 2; // Marge entre les cellules
    inline float dpi = 1.5f; // Densité de pixels
    inline int total_cell_size = cell_size + margin; // Taille totale d'une cellule avec la marge
    extern bool is_swapping, is_swap_horizontal, is_swap_vertical;// Booléens swap cellules



    // in game info
    extern unsigned int current_points; // Points actuels
    extern int first_selected_row, first_selected_column; // Pos de la première cellule sélectionnée
    extern int last_selected_row, last_selected_column; // Pos de la deuxieme cellule sélectionnée


    extern std::map<std::string, std::reference_wrapper<nsShape::Rectangle>> rect_map;


    extern std::map<std::string, Button> buttons; // Conteneurs des boutons
    extern std::chrono::microseconds delta_time; // Temps écoulé entre le dernier évenement

    extern std::string current_level; // Niveau actuel
    extern MLevels levels; // Gestionnaire de niveaux
    extern MinGL& main_window; // Fenêtre principale
}


#endif // GLOBALS_H
