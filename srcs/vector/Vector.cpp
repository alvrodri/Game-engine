#include "vector.hpp"

Vec2::Vec2() {
    this->x = 0.0f;
    this->y = 0.0f;
}

Vec2::Vec2(float x, float y) {
    this->x = x;
    this->y = y;
}

Vec2::Vec2(const Vec2& other) {
    this->x = other.x;
    this->y = other.y;
}

Vec2::~Vec2() {

}

Vec2& Vec2::operator=(const Vec2& other) {
    this->x = other.x;
    this->y = other.y;
    return *this;
}

const float Vec2::length() const {
    return sqrt(pow(this->x, 2) + pow(this->y, 2));
}

Vec2 Vec2::operator+(const Vec2& other) {
    Vec2 tmp(*this);

    tmp.x += other.x;
    tmp.y += other.y;

    return tmp;
}

Vec2 Vec2::operator-(const Vec2& other) {
    Vec2 tmp(*this);

    tmp.x -= other.x;
    tmp.y -= other.y;

    return tmp;
}

Vec2 Vec2::operator*(const float factor) {
    Vec2 tmp(*this);

    tmp.x *= factor;
    tmp.y *= factor;

    return tmp;
}

Vec2 Vec2::operator/(const float factor) {
    Vec2 tmp(*this);

    tmp.x /= factor;
    tmp.y /= factor;

    return tmp;
}

Vec3::Vec3() {
    this->x = 0.0f;
    this->y = 0.0f;
    this->z = 0.0f;
}

Vec3::Vec3(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

Vec3::Vec3(const Vec3& other) {
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
}

Vec3::~Vec3() {

}

Vec3& Vec3::operator=(const Vec3& other) {
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
    return *this;
}

void Vec3::rotateX(float angle) {
    float y = this->y;

    this->y = this->y * cos(angle) - this->z * sin(angle);
    this->z = this->z * cos(angle) + y * sin(angle);
}

void Vec3::rotateY(float angle) {
    float x = this->x;

    this->x = this->x * cos(angle) - this->z * sin(angle);
    this->z = this->z * cos(angle) + x * sin(angle);
}

void Vec3::rotateZ(float angle) {
    float x = this->x;

    this->x = this->x * cos(angle) - this->y * sin(angle);
    this->y = this->y * cos(angle) + x * sin(angle);
}

const float Vec3::length() const {
    return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}

Vec3 Vec3::operator+(const Vec3& other) {
    Vec3 tmp(*this);

    tmp.x += other.x;
    tmp.y += other.y;
    tmp.z += other.z;

    return tmp;
}

Vec3 Vec3::operator-(const Vec3& other) {
    Vec3 tmp(*this);

    tmp.x -= other.x;
    tmp.y -= other.y;
    tmp.z -= other.z;

    return tmp;
}

Vec3 Vec3::operator*(const float factor) {
    Vec3 tmp(*this);

    tmp.x *= factor;
    tmp.y *= factor;
    tmp.z *= factor;

    return tmp;
}

Vec3 Vec3::operator/(const float factor) {
    Vec3 tmp(*this);

    tmp.x /= factor;
    tmp.y /= factor;
    tmp.z /= factor;

    return tmp;
}

Vec3 Vec3::crossProduct(const Vec3& other) {
    return Vec3(
        this->y * other.z - this->z * other.y,
        this->z * other.x - this->x * other.z,
        this->x * other.y - this->y * other.x
    );
}

float Vec3::dotProduct(const Vec3& other) {
    return ((this->x * other.x) + (this->y * other.y) + (this->z * other.z));
}

void Vec3::normalize() {
    float length = this->length();

    this->x /= length;
    this->y /= length;
    this->z /= length;
}

void Vec3::scale(float m[4][4]) {
  this->x *= m[0][0];
  this->y *= m[1][1];
  this->z *= m[2][2];
}

Vec4::Vec4(float x, float y, float z, float w): x(x), y(y), z(z), w(w) {}

Vec4::Vec4(const Vec3 &vec) {
  this->x = vec.x;
  this->y = vec.y;
  this->z = vec.z;
  this->w = 1.0f;
}

Vec4 Vec4::operator*(float m[4][4]) {
  return Vec4(
      m[0][0] * this->x + m[0][1] * this->y + m[0][2] * this->z + m[0][3] * this->w,
      m[1][0] * this->x + m[1][1] * this->y + m[1][2] * this->z + m[1][3] * this->w,
      m[2][0] * this->x + m[2][1] * this->y + m[2][2] * this->z + m[2][3] * this->w,
      m[3][0] * this->x + m[3][1] * this->y + m[3][2] * this->z + m[3][3] * this->w
  );
}