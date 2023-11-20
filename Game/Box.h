#pragma once

#include "Object.h"


class Box : public Object {
public:
    Box(const std::string& id, const Vector3& pivot, float wx, float wy, float wz);
    Box(const std::string& id, float wx, float wy, float wz);
    Box(const std::string& id);
};
