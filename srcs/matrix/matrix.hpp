#pragma once

#include "../vector/vector.hpp"

typedef struct mat4_s {
  float m[4][4];
} mat4_t;

mat4_t getIdentity();