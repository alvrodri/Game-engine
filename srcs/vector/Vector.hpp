#pragma once

#include <math.h>

class Vec2 {
    public:
    Vec2();
    Vec2(float x, float y);
    Vec2(const Vec2& other);

    ~Vec2();

    Vec2& operator=(const Vec2& other);

    const float length() const;

    Vec2 operator+(const Vec2& other);
    Vec2 operator-(const Vec2& other);
    Vec2 operator*(const float factor);
    Vec2 operator/(const float factor);

    float x, y;
};

class Vec3 {
    public:
    Vec3();
    Vec3(float x, float y, float z);
    Vec3(const Vec3& other);

    ~Vec3();

    Vec3& operator=(const Vec3& other);

    const float length() const;

    Vec3 operator+(const Vec3& other);
    Vec3 operator-(const Vec3& other);
    Vec3 operator*(const float factor);
    Vec3 operator/(const float factor);

    Vec3 crossProduct(const Vec3& other);
    float dotProduct(const Vec3& other);

    void rotateX(float angle);
    void rotateY(float angle);
    void rotateZ(float angle);

    void normalize();

    float x, y, z;
};