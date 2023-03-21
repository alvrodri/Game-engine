#pragma once

#include <vector>
#include "../vector/Vector.hpp"

typedef struct {
    int a, b, c;
} face_t;

typedef struct {
    std::vector<Vector3>    vertices;
    std::vector<face_t>     faces;
    Vector3                 rotation = Vector3(0, 0, 0);
} mesh_t;