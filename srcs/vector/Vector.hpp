#pragma once

#include <math.h>

class Vector2 {
    public:
    Vector2();
    Vector2(float x, float y);
    Vector2(const Vector2& other);

    ~Vector2();

    Vector2& operator=(const Vector2& other);

    const float length() const;

    Vector2 operator+(const Vector2& other);
    Vector2 operator-(const Vector2& other);
    Vector2 operator*(const float factor);
    Vector2 operator/(const float factor);

    float x, y;
};

class Vector3 {
    public:
    Vector3();
    Vector3(float x, float y, float z);
    Vector3(const Vector3& other);

    ~Vector3();

    Vector3& operator=(const Vector3& other);

    const float length() const;

    Vector3 operator+(const Vector3& other);
    Vector3 operator-(const Vector3& other);
    Vector3 operator*(const float factor);
    Vector3 operator/(const float factor);

    Vector3 crossProduct(const Vector3& other);
    float dotProduct(const Vector3& other);

    void rotateX(float angle);
    void rotateY(float angle);
    void rotateZ(float angle);

    float x, y, z;
};