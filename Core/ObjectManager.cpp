#include "ObjectManager.h"

using namespace std;


void ObjectManager::add(const string& name, Object* object) {
    objects[name] = object;
}

void ObjectManager::remove(const string& name) {
    objects.erase(name);
}

Object* ObjectManager::get(const string& name) const {
    return objects.at(name);
}

void ObjectManager::clear() {
    objects.clear();
}

void ObjectManager::deleteAll() {
    for(auto& e : objects) {
        delete e.second;
    }
    clear();
}


void ObjectManager::update(float dt) {
    for(auto& e : objects) {
        e.second->update(dt);
    }
}
