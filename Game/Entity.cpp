#include "Entity.h"

using namespace std;


Entity::Entity(const string& id) : Object { id },
    move_direction { 0, 0 },
    move_speed { 0 }, 
    previus_transform { transform } {
    addComponent<AABB>();
    hitbox = getComponent<AABB>();
}

Entity::~Entity() {

}


void Entity::update(float dt) {
    previus_transform = transform;

    Object::update(dt);

    move(dt);
}

void Entity::move(float dt) {
    Vector2 d = move_direction;
    if(d.x != 0) {      // 옆으로 이동시
        if(d.y != 0) {  // 대각선 이동시
            d = normalize(d);
        }
    }

    transform.position += transform.forward() * d.y * move_speed * dt;
    transform.position += -transform.right() * d.x * move_speed * dt;
}
