#include "hdrs/animations.h"
#include "hdrs/globals.h"
#include <map>

template <typename T>
T animations::lerp(T a, T b, float t)
{
    return a + (b - a) * t;
}

float animations::fast_float_lerp(std::string& id, bool state, float min, float max, float speed)
{
    static std::map<std::string, float> float_map;

    using f_map_it = std::map<std::string, float>::iterator;

    f_map_it it = float_map.find(id);

    if (it == float_map.end())
    {
        float_map.insert({id, min});
        it = float_map.find(id);
    };

    const float rate_speed = speed * glob_blob::delta_time.count();

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

    it->second = std::clamp(it->second, min, max);

    return it->second;
}


int animations::fast_int_lerp(std::string& id, bool state, int min, int max, float speed)
{
    static std::map<std::string, int> float_map;

    using f_map_it = std::map<std::string, int>::iterator;

    f_map_it it = float_map.find(id);

    if (it == float_map.end())
    {
        float_map.insert({id, min});
        it = float_map.find(id);
    };

    const float rate_speed = speed * glob_blob::delta_time.count();

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

    it->second = std::clamp(it->second, min, max);

    return it->second;
}
