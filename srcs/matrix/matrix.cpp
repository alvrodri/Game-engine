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

mat4_t getXRotationMatrix(float a) {
  mat4_t m = getIdentity();

  m.m[1][1] = cos(a);
  m.m[1][2] = -sin(a);
  m.m[2][1] = sin(a);
  m.m[2][2] = cos(a);

  return m;
}

mat4_t getYRotationMatrix(float a) {
  mat4_t m = getIdentity();

  m.m[0][0] = cos(a);
  m.m[0][2] = sin(a);
  m.m[2][0] = -sin(a);
  m.m[2][2] = cos(a);

  return m;
}

mat4_t getZRotationMatrix(float a) {
  mat4_t m = getIdentity();

  m.m[0][0] = cos(a);
  m.m[0][1] = -sin(a);
  m.m[1][0] = sin(a);
  m.m[1][1] = cos(a);

  return m;
}