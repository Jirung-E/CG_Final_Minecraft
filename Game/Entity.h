#pragma once

#include "Object.h"
#include "Component/Collider.h"


class Entity : public Object {
public:
    // ������� �̵���
    Vector2 move_direction;
    float move_speed;   // blocks/sec
    AABB* hitbox;
    Transform previus_transform;

public:
    Entity(const std::string& id);
    virtual ~Entity();

public:
    virtual void update(float dt) override;
};
