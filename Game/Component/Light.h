#pragma once

#include "ComponentImpl.h"


class Light : public ComponentImpl {
public:
    Vector3 position;
    float ambient;
    
public:
    Light(Object* object, const std::string& id);

public:
    virtual void update(float dt);
};
