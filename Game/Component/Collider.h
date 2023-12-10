#pragma once

#include "ComponentImpl.h"
#include "../../Math/Math.h"


class Collider : public ComponentImpl {
public:
    bool is_colliding;
    bool is_trigger;

public:
    Collider(Object* object, const std::string& id);

public:
    virtual void update(float dt) override;
};


class AxisAlignedBoxCollider : public Collider {
public:
    Vector3 size;
    Vector3 center;

public:
    AxisAlignedBoxCollider(Object* object, const std::string& id, const Vector3& size={ 1, 1, 1 }, const Vector3& center={ 0, 0, 0 });
};
using AABB = AxisAlignedBoxCollider;


bool collide(Collider* collider1, Collider* collider2);
