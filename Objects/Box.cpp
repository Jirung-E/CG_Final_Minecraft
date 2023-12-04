#include "Box.h"

#include "../Game/Model.h"

using namespace std;


Box::Box(const string& id, const Vector3& pivot, float wx, float wy, float wz) : Object { id } {
    wx = (wx-1)/2.0f;
    wy = (wy-1);
    wz = (wz-1)/2.0f;

    Model* pm = new Model { Model::box };
    pm->points[0] += Vector3 { -pivot.x + -wx, -pivot.y,      -pivot.z + wz  };
    pm->points[1] += Vector3 { -pivot.x + -wx, -pivot.y,      -pivot.z + -wz };
    pm->points[2] += Vector3 { -pivot.x + wx,  -pivot.y,      -pivot.z + -wz };
    pm->points[3] += Vector3 { -pivot.x + wx,  -pivot.y,      -pivot.z + wz  };
    pm->points[4] += Vector3 { -pivot.x + -wx, -pivot.y + wy, -pivot.z + wz  };
    pm->points[5] += Vector3 { -pivot.x + wx,  -pivot.y + wy, -pivot.z + wz  };
    pm->points[6] += Vector3 { -pivot.x + wx,  -pivot.y + wy, -pivot.z + -wz };
    pm->points[7] += Vector3 { -pivot.x + -wx, -pivot.y + wy, -pivot.z + -wz };

    model = pm;
}

Box::Box(const string& id, float wx, float wy, float wz) : Box { id, { 0, 0, 0 }, wx, wy, wz } {

}

Box::Box(const string& id) : Box { id, { 0, 0, 0 }, 1, 1, 1 } {

}
