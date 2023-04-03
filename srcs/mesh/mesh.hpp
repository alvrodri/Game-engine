#pragma once

#include <vector>
#include "../vector/vector.hpp"

typedef struct face_s {
    int indexes[3];
    uint32_t color;
} Face;

typedef struct mesh_s {
    std::vector<Vec3>   vertices;
    std::vector<Face>   faces;
    Vec3                rotation = {0, 0, 0};
    Vec3                scale = {1, 1, 1};
    Vec3                translation = {0, 0, 0};
} Mesh;

typedef struct triangle_s {
    Vec2 points[3];
    uint32_t color;
    float avgZ;
} Triangle;

struct {
  bool operator()(Triangle  a, Triangle b) const {
    return a.avgZ > b.avgZ;
  }
} TriangleSort;