#include "Block.h"

using namespace std;

Block::Block(const string& id) : Box { id } {
    addComponent<AABB, Vector3, Vector3>("hitbox", { 1, 1, 1 }, { 0, 0.5f, 0 });
    hitbox = getComponent<AABB>("hitbox");
}
