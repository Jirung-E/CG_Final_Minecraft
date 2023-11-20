#pragma once


#include "Object.h"


class Entity abstract : public Object {
public:
    // ������� �̵���
    Vector2 move_direction;
    float move_speed;   // blocks/sec

public:
    Entity(const std::string& id);
    virtual ~Entity();

public:
    virtual void update(float dt) override;
};
