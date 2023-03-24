#pragma once

#include <vector>
#include "../vector/vector.hpp"

typedef struct {
    int a, b, c;
} Face;

typedef struct mesh_s {
    std::vector<Vec3>   vertices;
    std::vector<Face>   faces;
    Vec3                rotation = Vec3(0, 0, 0);
} Mesh;

typedef struct {
    Vec2 points[3];
} Triangle;