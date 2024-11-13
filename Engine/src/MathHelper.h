//
// Created by nicol on 10/28/2024.
//

#ifndef MATHHELPER_H
#define MATHHELPER_H
#include "glm/vec2.hpp"


class MathHelper
{
public:
    MathHelper() = default;
    ~MathHelper() = default;

    static float RadToDeg(float rad);
    static float DegToRad(float deg);
    static float Lerp(float a, float b, float t);
    static float Clamp(float value, float min, float max);
    static float Sin(float angle);
    static float Cos(float angle);
    static float Sqrt(float value);
    static float Tan(float angle);
    static float Cross2D(const glm::vec2 &a, const glm::vec2 &b);
    static float Dot2D(const glm::vec2 &a, const glm::vec2 &b);
    static float Pow(float base, float exponent);
    static float Floor(float value);
    static float Atan2(float y, float x);
    static float PI;
};



#endif //MATHHELPER_H
