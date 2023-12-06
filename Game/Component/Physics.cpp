#include "Physics.h"


Physics::Physics(Object* object, const std::string& id) : ComponentImpl { object, id },
    velocity { 0, 0, 0 }, acceleration { 0, 0, 0 }, gravity { 0, -10, 0 },
    force { 0, 0, 0 }, mass { 1 } {

}


void Physics::update(float dt) {
    object->transform.position += velocity * dt;
    velocity += acceleration * dt;
    velocity += gravity * dt;
    velocity += force;
    force = { 0, 0, 0 };
}
