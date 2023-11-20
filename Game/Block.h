#pragma once

#include "Box.h"
#include "Component/Collider.h"


class Block : public Box {
private:
    float width;
    float height;
    float depth;

public:
    AABB* hitbox;

public:
    Block(float width, float height, float depth);
    Block();

public:
    float top() const;
    float bottom() const;
    float left() const;
    float right() const;
    float front() const;
    float back() const;
};