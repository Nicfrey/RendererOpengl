//
// Created by nicol on 10/28/2024.
//

#include "MathHelper.h"

#include "glm/trigonometric.hpp"

float MathHelper::PI = 3.14159265358979323846f;

float MathHelper::RadToDeg(float rad)
{
    return glm::degrees(rad);
}

float MathHelper::DegToRad(float deg)
{
    return glm::radians(deg);
}

float MathHelper::Lerp(float a, float b, float t)
{
    return a + (b - a) * t;
}

float MathHelper::Clamp(float value, float min, float max)
{
    if (value < min)
    {
        return min;
    }
    if (value > max)
    {
        return max;
    }
    return value;
}

float MathHelper::Sin(float angle)
{
    return std::sin(angle);
}

float MathHelper::Cos(float angle)
{
    return std::cos(angle);
}

float MathHelper::Sqrt(float value)
{
    return std::sqrt(value);
}

float MathHelper::Tan(float angle)
{
    return std::tan(angle);
}

float MathHelper::Cross2D(const glm::vec2 &a, const glm::vec2 &b)
{
    return a.x * b.y - a.y * b.x;
}

float MathHelper::Dot2D(const glm::vec2 &a, const glm::vec2 &b)
{
    return a.x * b.x + a.y * b.y;
}

float MathHelper::Pow(float base, float exponent)
{
    return std::pow(base, exponent);
}

float MathHelper::Floor(float value)
{
    return std::floor(value);
}

float MathHelper::Atan2(float y, float x)
{
    return std::atan2(y, x);
}
