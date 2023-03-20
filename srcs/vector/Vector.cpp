#include "Vector.hpp"

Vector2::Vector2() {
    this->x = 0.0f;
    this->y = 0.0f;
}

Vector2::Vector2(float x, float y) {
    this->x = x;
    this->y = y;
}

Vector2::Vector2(const Vector2& other) {
    this->x = other.x;
    this->y = other.y;
}

Vector2::~Vector2() {
    
}

Vector2& Vector2::operator=(const Vector2& other) {
    this->x = other.x;
    this->y = other.y;
    return *this;
}

Vector3::Vector3() {
    this->x = 0.0f;
    this->y = 0.0f;
    this->z = 0.0f;
}

Vector3::Vector3(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector3::Vector3(const Vector3& other) {
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
}

Vector3::~Vector3() {

}

Vector3& Vector3::operator=(const Vector3& other) {
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
    return *this;
}

void Vector3::rotateX(float angle) {
    float y = this->y;

    this->y = this->y * cos(angle) - this->z * sin(angle);
    this->z = this->z * cos(angle) + y * sin(angle);
}

void Vector3::rotateY(float angle) {
    float x = this->x;

    this->x = this->x * cos(angle) - this->z * sin(angle);
    this->z = this->z * cos(angle) + x * sin(angle);
}

void Vector3::rotateZ(float angle) {
    float x = this->x;

    this->x = this->x * cos(angle) - this->y * sin(angle);
    this->y = this->y * cos(angle) + x * sin(angle);
}