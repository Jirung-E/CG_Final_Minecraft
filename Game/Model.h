#pragma once

#include "../Math/Math.h"

#include <vector>
#include <string>


class Vertex {
public:
    int point;
    int normal;
    int texture_coord;

public:
    Vertex(int point, int normal, int texture_coord);
    Vertex(int point, int normal);
    Vertex(int n=0);
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
    std::vector<Vector3> uvs;
    std::vector<Vector2> texture_coords;
    std::vector<IndexedPolygon> polygons;
    std::vector<int> texture_id;

public:
    Model();

public:
    void move(const Vector3& vector);       // 벡터 방향으로 버텍스 밀기(피벗변경)
};
