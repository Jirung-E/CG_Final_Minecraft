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
    // child���� �Ҹ��ڰ� ȣ��Ǹ� parent�� children��Ͽ��� ������ �����ǹǷ� 
    // ������ �°� ����� ù��°�� �����ϱ� ���� �̷���
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
        child.second->update(dt);          // ObjectManager������ �ڽĵ��� ������Ʈ�ϸ� �ȵ�
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
    // �ش�Ǵ� ������Ʈ�� ��� ã�Ƽ� ����
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
