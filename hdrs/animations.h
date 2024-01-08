#ifndef ANIMATIONS_H
#define ANIMATIONS_H
#include "mingl/graphics/rgbacolor.h"
#include <string>
#include <cmath>



/**
 * @namespace animations
 * @brief Espace de nom pour la gestion d'animation
 */
namespace animations
{
    // Fonction externe float
    extern float fast_float_lerp(std::string& id, bool state, float min, float max, float speed);
    // Fonction externe int
    extern int fast_int_lerp(std::string& id, bool state, int min, int max, float speed);

    // Définition d'un modèle de fonction pour l'interpolation linéaire, lissage de mouvement
    template <typename T>
    T lerp(T a, T b, float t)
    {
        return a + (b - a) * t;
    }

    // Déclaration d'une fonction externe pour l'interpolation linéaire de couleurs RGBA
    extern nsGraphics::RGBAcolor lerp_color(nsGraphics::RGBAcolor start, nsGraphics::RGBAcolor end, float stage);

    // Définition des fonctions d'assouplissement (easing) de l'étape (stage) (voir https://easings.net/)

    // Fonction d'assouplissement "ease_in_sine" pour les flottants
    inline float ease_in_sine(float t)
    {
        return 1 - cos((t * M_PI) / 2.f);
    };

    // Modèle de fonction d'assouplissement "ease_in_cubic" pour n'importe quel type N
    template <typename N>
    N ease_in_cubic(N t)
    {
        return t * t * t;
    };

    // Modèle de fonction d'assouplissement "ease_out_sine" pour n'importe quel type N
    template <typename N>
    N ease_out_sine(N t)
    {
        return sin((t * M_PI) / 2.f);
    };

    // Modèle de fonction d'assouplissement "ease_out_cubic" pour n'importe quel type N
    template <typename N>
    N ease_out_cubic(N t)
    {
        return 1 - pow(1 - t, 3);
    };
}

#endif // ANIMATIONS_H
