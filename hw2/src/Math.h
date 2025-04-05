//
// Created by William Zhao on 4/4/25.
//

#ifndef MATH_H
#define MATH_H

#include <cmath>

struct Vector3
{
    float x, y, z;
    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    Vector3 operator+(const Vector3& other) const
    {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }
    Vector3 operator-(const Vector3& other) const
    {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }
    Vector3 operator*(float scalar) const
    {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }
    Vector3 operator/(float scalar) const
    {
        return Vector3(x / scalar, y / scalar, z / scalar);
    }

    static Vector3 Dot(const Vector3& a, const Vector3& b)
    {
        return Vector3(a.x * b.x, a.y * b.y, a.z * b.z);
    }
    float Length() const
    {
        return std::sqrt(x * x + y * y + z * z);
    }
    float LengthSquared() const
    {
        return x * x + y * y + z * z;
    }
    void Normalize()
    {
        const float length = Length();
        if (length > 0)
        {
            x /= length;
            y /= length;
            z /= length;
        }
    }
};

#endif //MATH_H
