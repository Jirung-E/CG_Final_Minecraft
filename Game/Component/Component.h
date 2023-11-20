#pragma once

#include <string>


// To create a new component class, inherit ComponentImpl instead of this.
class Component abstract {
public:
    const std::string id;

public:
    Component(const std::string& id);

public:
    virtual void update(float dt) abstract;
};
