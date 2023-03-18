#pragma once

class Vector2 {
    public:
    Vector2();
    Vector2(float x, float y);
    Vector2(const Vector2& other);

    ~Vector2();

    Vector2& operator=(const Vector2& other);

    private:
    float x, y;
};

class Vector3 {
    public:
    Vector3();
    Vector3(float x, float y, float z);
    Vector3(const Vector3& other);

    ~Vector3();

    Vector3& operator=(const Vector3& other);

    private:
    float x, y, z;
};