#ifndef UTILS_H
#define UTILS_H
#include "components/button.h"

/**
 * @struct explosion
 * @brief Possède des données pour l'explosion d'une bombe
 */
struct explosion
{
    bool did_explode; // booléen si on a detecté une explosion
    unsigned int start_row,start_col; // si une explosion, on a la cellule
    unsigned int explosion_num; // le nombre d'explosion à la suite
};

#endif // UTILS_H
