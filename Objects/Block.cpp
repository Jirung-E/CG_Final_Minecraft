#include "Block.h"


Block::Block(float width, float height, float depth) : Box { "", width, height, depth },
width { width }, height { height }, depth { depth } {
    addComponent<AABB, Vector3, Vector3>("hitbox", { width, height, depth }, { 0, 0, 0 });
    hitbox = getComponent<AABB>("hitbox");
}

Block::Block() : Block { 1, 1, 1 } {

}


float Block::top() const {
    return transform.position.y + height/2.0f;
}

float Block::bottom() const {
    return transform.position.y - height/2.0f;
}

float Block::left() const {
    return transform.position.x - width/2.0f;
}

float Block::right() const {
    return transform.position.x + width/2.0f;
}

float Block::front() const {
    return transform.position.z + depth/2.0f;
}

float Block::back() const {
    return transform.position.z - depth/2.0f;
}
