#pragma once

#include "../Game/Component/Collider.h"
#include "Box.h"


class Block : public Box {
public:
    AABB* hitbox;

public:
    Block(const std::string& id);
};