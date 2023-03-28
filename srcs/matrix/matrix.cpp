#include "matrix.hpp"

mat4_t getIdentity() {
  return {{
      {1, 0, 0, 0},
      {0, 1, 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1},
    }};
}

mat4_t getScaleMatrix(Vec3& vec) {
  mat4_t m = getIdentity();

  m.m[0][0] = vec.x;
  m.m[1][1] = vec.y;
  m.m[2][2] = vec.z;

  return m;
}

mat4_t getTranslateMatrix(Vec3 &vec) {
  mat4_t m = getIdentity();

  m.m[0][3] = vec.x;
  m.m[1][3] = vec.y;
  m.m[2][3] = vec.z;

  return m;
}