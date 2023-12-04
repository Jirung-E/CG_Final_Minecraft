#include "Box.h"

#include "../Game/Model.h"

using namespace std;


Box::Box(const string& id, const Vector3& pivot, float wx, float wy, float wz) : Object { id } {
    wx = wx/2.0f;
    wy = wy/2.0f;
    wz = wz/2.0f;

    Model* pm = new Model { Model::box };
    //pm->points.reserve(8);
    //pm->points.push_back({ -pivot.x + -wx, -pivot.y + -wy, -pivot.z + -wz });
    //pm->points.push_back({ -pivot.x + wx,  -pivot.y + -wy, -pivot.z + -wz });
    //pm->points.push_back({ -pivot.x + wx,  -pivot.y + wy,  -pivot.z + -wz });
    //pm->points.push_back({ -pivot.x + -wx, -pivot.y + wy,  -pivot.z + -wz });
    //pm->points.push_back({ -pivot.x + -wx, -pivot.y + -wy, -pivot.z + wz });
    //pm->points.push_back({ -pivot.x + wx,  -pivot.y + -wy, -pivot.z + wz });
    //pm->points.push_back({ -pivot.x + wx,  -pivot.y + wy,  -pivot.z + wz });
    //pm->points.push_back({ -pivot.x + -wx, -pivot.y + wy,  -pivot.z + wz });

    //pm->normals.reserve(6);
    //pm->normals.push_back({ 0, 0, -1 });
    //pm->normals.push_back({ 0, 0, 1 });
    //pm->normals.push_back({ 0, -1, 0 });
    //pm->normals.push_back({ 0, 1, 0 });
    //pm->normals.push_back({ -1, 0, 0 });
    //pm->normals.push_back({ 1, 0, 0 });

    //pm->texture_coords.reserve(6);
    //pm->texture_coords.push_back({ 0, 0 });
    //pm->texture_coords.push_back({ 1, 0 });
    //pm->texture_coords.push_back({ 1, 1 });
    //pm->texture_coords.push_back({ 0, 1 });
    //pm->texture_coords.push_back({ 0, 0 });
    //pm->texture_coords.push_back({ 1, 0 });

    //pm->polygons.reserve(12);
    //pm->polygons.push_back({ {0, 0}, {2, 0}, {1, 0} });
    //pm->polygons.push_back({ {0, 0}, {3, 0}, {2, 0} });
    //pm->polygons.push_back({ {4, 1}, {5, 1}, {6, 1} });
    //pm->polygons.push_back({ {4, 1}, {6, 1}, {7, 1} });
    //pm->polygons.push_back({ {0, 2}, {1, 2}, {5, 2} });
    //pm->polygons.push_back({ {0, 2}, {5, 2}, {4, 2} });
    //pm->polygons.push_back({ {1, 5}, {2, 5}, {6, 5} });
    //pm->polygons.push_back({ {1, 5}, {6, 5}, {5, 5} });
    //pm->polygons.push_back({ {2, 3}, {3, 3}, {7, 3} });
    //pm->polygons.push_back({ {2, 3}, {7, 3}, {6, 3} });
    //pm->polygons.push_back({ {3, 4}, {0, 4}, {4, 4} });
    //pm->polygons.push_back({ {3, 4}, {4, 4}, {7, 4} });

    model = pm;
}

Box::Box(const string& id, float wx, float wy, float wz) : Box { id, { 0, 0, 0 }, wx, wy, wz } {

}

Box::Box(const string& id) : Box { id, { 0, 0, 0 }, 1, 1, 1 } {

}
