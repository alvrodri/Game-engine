#pragma once

#include "../vector/vector.hpp"

typedef struct mat4_s {
  float m[4][4];

  void print() {
    for (int x = 0; x < 4; x++) {
      for (int y = 0; y < 4; y++) {
        std::cout << m[x][y] << " ";
      }
      std::cout << std::endl;
    }
  };
} mat4_t;

mat4_t getIdentity();

mat4_t getScaleMatrix(Vec3& scale);
mat4_t getTranslateMatrix(Vec3& scale);