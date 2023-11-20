#pragma once

#include <unordered_map>
#include <unordered_set>
#include <string>

#include "Transform.h"
#include "Model.h"
#include "Component/Component.h"
#include "../Shader/Material.h"
#include "../Math/Math.h"


class Object {
public:
    const std::string id;

    Transform transform;
    Model* model;
    bool render;

    Material material;

//private:
    Object* parent;
    std::unordered_map<std::string, Object*> children;

private:
    std::unordered_set<Component*> components;

public:
    Object(const std::string& id);
    virtual ~Object();

public:
    virtual void update(float dt);

    void setParent(Object* parent);
    Object* getParent() const;
    void addChild(Object* child);
    void removeChild(const std::string& id);
    Object* getChild(const std::string& id);

    Matrix4 absoluteTransformMatrix() const;

    // ------------------------------------------------------------ //

    template <class T, typename... Args>
    void addComponent(const std::string& id, Args&&... args) {
        static_assert(std::is_base_of<Component, T>::value, "T must be a Component");
        components.insert(new T { this, id, std::forward<Args>(args)... });
    }
    template <class T, typename... Args>
    void addComponent(Args&&... args) {
        static_assert(std::is_base_of<Component, T>::value, "T must be a Component");
        components.insert(new T { this, "", std::forward<Args>(args)... });
    }

    template <class T>
    void removeComponent() {
        // TODO: 해당되는거 다 지워지는지 확인
        static_assert(std::is_base_of<Component, T>::value, "T must be a Component");
        for(auto component : components) {
            if(dynamic_cast<T*>(component)) {
                components.erase(component);
            }
        }
    }
    void removeComponent(Component* component);
    void removeComponent(const std::string& id);

    template <class T>
    T* getComponent(const std::string& id="") {
        static_assert(std::is_base_of<Component, T>::value, "T must be a Component");
        for(auto component : components) {
            if(dynamic_cast<T*>(component)) {
                if(id == "" || component->id == id) {
                    return dynamic_cast<T*>(component);
                }
            }
        }
        return nullptr;
    }

    template <class T>
    std::vector<T*> getComponents() {
        static_assert(std::is_base_of<Component, T>::value, "T must be a Component");
        std::vector<T*> result;
        for(auto component : components) {
            if(dynamic_cast<T*>(component)) {
                result.push_back(dynamic_cast<T*>(component));
            }
        }
        return result;
    }
};