#pragma once

#include "../vector/vector.hpp"

typedef struct mat4_s {
  float m[4][4];
} mat4_t;

mat4_t getIdentity();

mat4_t getScaleMatrix(Vec3& vec);
mat4_t getTranslateMatrix(Vec3& vec);
mat4_t getXRotationMatrix(float a);
mat4_t getYRotationMatrix(float a);
mat4_t getZRotationMatrix(float a);