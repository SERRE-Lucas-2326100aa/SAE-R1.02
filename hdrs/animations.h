#ifndef ANIMATIONS_H
#define ANIMATIONS_H
#include "mingl/graphics/rgbacolor.h"
#include <string>
#include <cmath>

namespace animations
{
    extern float fast_float_lerp(std::string& id, bool state, float min, float max, float speed);
    extern int fast_int_lerp(std::string& id, bool state, int min, int max, float speed);


    template <typename T>
    T lerp(T a, T b, float t)
    {
        return a + (b - a) * t;
    }

    extern nsGraphics::RGBAcolor lerp_color(nsGraphics::RGBAcolor start, nsGraphics::RGBAcolor end, float stage);

    // stage easings functions (https://easings.net/)

    inline float ease_in_sine(float t)
    {
        return 1 - cos((t * M_PI) /2.f);
    };

    template <typename N>
    N ease_in_cubic(N t)
    {
        return t*t*t;
    };

    template <typename N>
    N ease_out_sine(N t)
    {
        return sin((t * M_PI) /2.f);
    };

    template <typename N>
    N ease_out_cubic(N t)
    {
        return 1 - pow(1 - t,3);
    };
}

#endif // ANIMATIONS_H
