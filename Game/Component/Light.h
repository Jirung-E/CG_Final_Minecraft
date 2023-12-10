#pragma once

#include "ComponentImpl.h"


class Light : public ComponentImpl {
public:
    Vector3 position;
    ColorRGB color;
    float ambient;
    float c1;
    float c2;
    
public:
    Light(Object* object, const std::string& id);

public:
    virtual void update(float dt);
};
