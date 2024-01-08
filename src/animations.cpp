#include "hdrs/animations.h"
#include "hdrs/globals.h"
#include <map>

// Init float
float animations::fast_float_lerp(std::string& id, bool state, float min, float max, float speed)
{

    //une map static qui est déclaré qu'une fois et qui stocke les valeurs précedantes
    static std::map<std::string, float> float_map;

    // Alias pour l'itérateur car pas d'auto ici :)
    using f_map_it = std::map<std::string, float>::iterator;

    // Recherche l'id dans la map
    f_map_it it = float_map.find(id);

    // Si l'id n'est pas trouvée, l'insert avec la valeur minimale
    if (it == float_map.end())
    {
        float_map.insert({id, min});
        it = float_map.find(id);
    };

    // on lisse le mouvement par rapport au temps de la frame précedante
    const float rate_speed = speed * (glob_blob::delta_time.count() / 1000000.f);

    // Met a jours la valeur en fonction de l'état
    if (state)
    {
        if (it->second < max)
            it->second += rate_speed;
    }
    else
    {
        if (it->second > min)
            it->second -= rate_speed;
    }

    // Limite la valeur entre le min et max
    it->second = std::clamp(it->second, min, max);

    return it->second;
}


int animations::fast_int_lerp(std::string& id, bool state, int min, int max, float speed)
{
    // Map qui stocke les valeurs précedentes
    static std::map<std::string, int> int_map;

    // Alias pour l'itérateur
    using f_map_it = std::map<std::string, int>::iterator;

    // Recherche l'id dans la map
    f_map_it it = int_map.find(id);

    // Si l'id n'est pas trouvée, l'insert avec la valeur minimale
    if (it == int_map.end())
    {
        int_map.insert({id, min});
        it = int_map.find(id);
    };

    const float rate_speed = speed * (glob_blob::delta_time.count() / 1000000.f);

    // Met a jours la valeur en fonction de l'état
    if (state)
    {
        if (it->second < max)
            it->second += rate_speed;
    }
    else
    {
        if (it->second > min)
            it->second -= rate_speed;
    }

    // Limite la valeur entre le min et max
    it->second = std::clamp(it->second, min, max);

    return it->second;
}

// Fonction pour les couleur
nsGraphics::RGBAcolor animations::lerp_color(nsGraphics::RGBAcolor start, nsGraphics::RGBAcolor end, float stage)
{
    // Crée une nouvelle couleur entre les couleurs de du début et de la fin
    nsGraphics::RGBAcolor col = {
        lerp(start.getRed(), end.getRed(), stage),
        lerp(start.getGreen(), end.getGreen(), stage),
        lerp(start.getBlue(), end.getBlue(), stage)
    };

    return col;
}
