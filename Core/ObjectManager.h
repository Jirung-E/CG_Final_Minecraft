#pragma once

#include <string>
#include <unordered_map>

#include "../Game/Object.h"


class ObjectManager {
    friend class Game;

protected:
    std::unordered_map<std::string, Object*> objects;

public:
    void add(const std::string& name, Object* object);
    void remove(const std::string& name);       // delete는 같이 하지 않음
    Object* get(const std::string& name) const;
    void clear();
    void deleteAll();
    size_t size() const;

    void update(float dt);

    auto begin() const { return objects.begin(); }
    auto end() const { return objects.end(); }
};
