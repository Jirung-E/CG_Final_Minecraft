#pragma once

#include <string>
#include <unordered_map>

#include "../Game/Object.h"


class ObjectManager {
    friend class Game;

private:
    std::unordered_map<std::string, Object*> objects;
    
public:
    void add(const std::string& name, Object* object);
    void remove(const std::string& name);       // delete�� ���� ���� ����
    Object* get(const std::string& name) const;
    void clear();
    void deleteAll();

    void update(float dt);

    auto begin() const { return objects.begin(); }
    auto end() const { return objects.end(); }
};
