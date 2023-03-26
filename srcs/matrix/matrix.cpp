#include "matrix.hpp"

mat4_t getIdentity() {
  return {{
      {1, 0, 0, 0},
      {0, 1, 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1},
    }};
}

mat4_t getScaleMatrix(Vec3& scale) {
  mat4_t m = getIdentity();

  m.m[0][0] = scale.x;
  m.m[1][1] = scale.y;
  m.m[2][2] = scale.z;

  return m;
}