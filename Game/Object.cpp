#include "Object.h"

using namespace std;


Object::Object(const string& id) :
    id { id },
    transform { },
    model { nullptr },
    parent { nullptr }, 
    render { true }, 
    material { Material::base } {

}

Object::~Object() {
    for(auto component : components) {
        delete component;
    }
    // child에서 소멸자가 호출되면 parent의 children목록에서 스스로 삭제되므로 
    // 갯수에 맞게 목록의 첫번째만 삭제하기 위해 이렇게
    size_t len = children.size();
    for(size_t i=0; i<len; ++i) {
		delete children.begin()->second;
	}
    if(model != nullptr) {
        delete model;
    }
    if(parent != nullptr) {
        parent->removeChild(id);
    }
}


void Object::update(float dt) {
    for(auto component : components) {
        component->update(dt);
    }
    for(auto child : children) {
        child.second->update(dt);          // ObjectManager에서는 자식들을 업데이트하면 안됨
    }
}


void Object::setParent(Object* parent) {
    if(this->parent != nullptr) {
        this->parent->removeChild(id);
    }
    this->parent = parent;
    if(parent != nullptr) {
        parent->children[id] = this;
    }
}

Object* Object::getParent() const {
    return parent;
}

void Object::addChild(Object* child) {
    child->setParent(this);
}

void Object::removeChild(const string& id) {
    children[id]->parent = nullptr;
    children.erase(id);
}

Object* Object::getChild(const string& id) {
    return children[id];
}


Matrix4 Object::absoluteTransformMatrix() const {
    if(parent == nullptr) {
        return transform.matrix();
    }
    return parent->absoluteTransformMatrix() * transform.matrix();
}

// ------------------------------------------------------------ //

void Object::removeComponent(Component* component) {
    components.erase(component);
}

void Object::removeComponent(const string& id) {
    // 해당되는 컴포넌트를 모두 찾아서 삭제
    for(auto it = components.begin(); it != components.end(); ) {
        if((*it)->id == id) {
            delete* it;
            it = components.erase(it);
        }
        else {
            ++it;
        }
    }
}
