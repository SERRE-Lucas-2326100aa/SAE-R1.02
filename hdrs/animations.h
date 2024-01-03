#ifndef ANIMATIONS_H
#define ANIMATIONS_H
#include <string>


namespace animations
{
    extern float fast_float_lerp(std::string& id, bool state, float min, float max, float speed);
    extern int fast_int_lerp(std::string& id, bool state, float min, float max, float speed);
}

#endif // ANIMATIONS_H
