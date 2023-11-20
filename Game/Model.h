#pragma once

#include "../Math/Math.h"

#include <vector>


class Vertex {
public:
    int point;
    int normal;

public:
    Vertex(int point, int normal);
    Vertex(int n);
};

class IndexedPolygon {
public:
    Vertex v1;
    Vertex v2;
    Vertex v3;
};


class Model {
public:
    std::vector<Vector3> points;
    std::vector<Vector3> normals;
    std::vector<IndexedPolygon> polygons;
};
