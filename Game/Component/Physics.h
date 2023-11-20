#pragma once

#include "ComponentImpl.h"


class Physics final : public ComponentImpl {
public:
    Vector3 velocity;
    Vector3 acceleration;
    Vector3 gravity;
    Vector3 force;
    float mass;

public:
    Physics(Object* object, const std::string& id);

public:
    virtual void update(float dt) override;
};
