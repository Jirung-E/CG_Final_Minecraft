#pragma once

#include "Component.h"
#include "../Object.h"


class ComponentImpl abstract : public Component {
public:
    Object* object;

public:
    ComponentImpl(Object* object, const std::string& id);

public:
    virtual void update(float dt) override abstract;
};
