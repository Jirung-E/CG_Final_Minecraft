#include "ChunkBasedObjectManager.h"

using namespace std;


const int ChunkInfo::chunk_size = 2;

ChunkInfo::ChunkInfo(int x, int y, int z) : x { x }, y { y }, z { z } {

}

ChunkInfo::ChunkInfo(const Vector3& position) : ChunkInfo {
    int(floor(position.x / (float)chunk_size)),
    int(floor(position.y / (float)chunk_size)),
    int(floor(position.z / (float)chunk_size)) } {

}

ChunkInfo::ChunkInfo(const ChunkInfo& other) : ChunkInfo { other.x, other.y, other.z } {

}

bool ChunkInfo::operator==(const ChunkInfo& other) const {
    return x == other.x && y == other.y && z == other.z;
}

bool ChunkInfo::operator!=(const ChunkInfo& other) const {
    return !(*this == other);
}


// ---------------------------------------------------------------------------- //


ChunkBasedObjectManager::ChunkBasedObjectManager(ObjectManager& objects) : objects { objects }, player { nullptr } {

}

void ChunkBasedObjectManager::add(const string& name, Object* object) {
    objects.add(name, object);
    chunk_info[ChunkInfo { object->transform.position }].insert(object);
}

void ChunkBasedObjectManager::remove(const string& name) {
    Object* object = objects.get(name);
    objects.remove(name);
    auto& chunk_objects = chunk_info[ChunkInfo { object->transform.position }];
    chunk_objects.erase(find(chunk_objects.begin(), chunk_objects.end(), object));
}

Object* ChunkBasedObjectManager::get(const string& name) const {
    return objects.get(name);
}

void ChunkBasedObjectManager::clear() {
    objects.clear();
	chunk_info.clear();
}

void ChunkBasedObjectManager::deleteAll() {
    objects.deleteAll();
	chunk_info.clear();
}


unordered_set<Object*> ChunkBasedObjectManager::getObjectsInChunk(const ChunkInfo& chunk) const {
    if(chunk_info.find(chunk) == chunk_info.end()) {
        return unordered_set<Object*> { };
    }
    return chunk_info.at(chunk);
}

unordered_set<Object*> ChunkBasedObjectManager::getObjectsInChunk(int chunk_x, int chunk_y, int chunk_z) const {
    return getObjectsInChunk(ChunkInfo { chunk_x, chunk_y, chunk_z });
}

unordered_set<Object*> ChunkBasedObjectManager::getObjectsInRadius(const Vector3& position, int radius) {
    unordered_set<Object*> result;
    ChunkInfo center_chunk { position };
    for(int x=center_chunk.x-radius; x<=center_chunk.x+radius; ++x) {
        int dx = abs(x - center_chunk.x);
        for(int y=center_chunk.y-radius; y<=center_chunk.y+radius; ++y) {
            int dy = abs(y - center_chunk.y);
            for(int z=center_chunk.z-radius; z<=center_chunk.z+radius; ++z) {
                int dz = abs(z - center_chunk.z);
                if(dx*dx + dy*dy + dz*dz > radius*radius) {
                    continue;
                }
                for(auto& e : getObjectsInChunk(x, y, z)) {
                    result.insert(e);
                    if(Block* block = dynamic_cast<Block*>(e)) {        // TODO: 최적화 필요 - 호출될때마다 dynamic_cast를 하면 비효율적
                        blocks.insert(block);
                    }
                    else if(Entity* entity = dynamic_cast<Entity*>(e)) {
                        entities.insert(entity);
                    }
                }
            }
        }
    }
    return result;
}


void ChunkBasedObjectManager::update(float dt, int radius) {
    blocks.clear();
    entities.clear();

    auto objs = getObjectsInRadius(player->transform.position, radius);
    
    for(auto& e : objs) {
        ChunkInfo chunk { e->transform.position };

		e->update(dt);
        //e->transform.position.y += dt;

        ChunkInfo new_chunk { e->transform.position };
        if(chunk != new_chunk) {
            chunk_info[new_chunk].insert(e);
            auto it = find(chunk_info[chunk].begin(), chunk_info[chunk].end(), e);
            if(it != chunk_info[chunk].end()) {
                chunk_info[chunk].erase(it);
            }
        }
	}

    // TODO: 최적화 필요 - blocks는 반복문을 2번 돔
    for(auto& e : blocks) {
        AABB* block = e->getComponent<AABB>();     // 충돌체크로 보정된 위치에 가게 되면 소속된 청크가 변경될 수 있음. 
        if(block == nullptr) {
            continue;
        }
        // 방지하기 위해서는 블럭 먼저 업데이트, 그 뒤에 엔티티 업데이트
        // 하지만 블럭이 움직이지 않을거라 일단 이대로
        for(auto& entity : entities) {
            // 1. 이전 프레임과 현재 프레임 보간해서 그 사이에 충돌지점을 찾음
            Transform& prev_transform = entity->previus_transform;
            AABB* hitbox = new AABB { nullptr, entity->hitbox->id+"_clone", entity->hitbox->size, entity->hitbox->center };
            
            auto mat = prev_transform.translationMatrix() * prev_transform.scaleMatrix();
            Vector3 center1 = { mat * Vector4 { hitbox->center, 1 } };
            Vector3 size1 = mat * Vector4 { hitbox->size, 0 };

            mat = e->transform.translationMatrix() * e->transform.scaleMatrix();
            Vector3 center2 = { mat * Vector4 { block->center, 1 } };
            Vector3 size2 = mat * Vector4 { block->size, 0 };

            float wx1 = size1.x / 2.0f;
            float wy1 = size1.y / 2.0f;
            float wz1 = size1.z / 2.0f;
            float top1 = center1.y + wy1;
            float bottom1 = center1.y - wy1;
            float left1 = center1.x - wx1;
            float right1 = center1.x + wx1;
            float front1 = center1.z + wz1;
            float back1 = center1.z - wz1;

            float wx2 = size2.x / 2.0f;
            float wy2 = size2.y / 2.0f;
            float wz2 = size2.z / 2.0f;
            float top2 = center2.y + wy2;
            float bottom2 = center2.y - wy2;
            float left2 = center2.x - wx2;
            float right2 = center2.x + wx2;
            float front2 = center2.z + wz2;
            float back2 = center2.z - wz2;

            Vector3 velocity = entity->transform.position - prev_transform.position;

            float t_dy = dt;
            if(velocity.y != 0) {
                float t_dy1 = (top1 - bottom2) / -velocity.y;
                float t_dy2 = (top2 - bottom1) / velocity.y;
                t_dy = fminf(fmaxf(t_dy1, 0), fmaxf(t_dy2, 0));
            }
            float t_dx = dt;
            if(velocity.x != 0) {
                float t_dx1 = (right1 - left2) / -velocity.x;
                float t_dx2 = (right2 - left1) / velocity.x;
                t_dx = fminf(fmaxf(t_dx1, 0), fmaxf(t_dx2, 0));
            }
            float t_dz = dt;
            if(velocity.z != 0) {
                float t_dz1 = (front1 - back2) / -velocity.z;
                float t_dz2 = (front2 - back1) / velocity.z;
                t_dz = fminf(fmaxf(t_dz1, 0), fmaxf(t_dz2, 0));
            }
            float t_min = fminf(t_dy, fminf(t_dx, t_dz));

            if(t_min < dt) {
                Log::log("t_min: %f", t_min);
                Log::log("velocity * t_min: %f %f %f\n", velocity.x * t_min, velocity.y * t_min, velocity.z * t_min);
                entity->transform.position -= velocity * (dt-t_min);
                Physics* physics = entity->getComponent<Physics>();
                if(physics != nullptr) {
                    if(t_min == t_dy) {
                        physics->velocity.y = 0;
                    }
                    else if(t_min == t_dx) {
                        physics->velocity.x = 0;
                    }
                    else if(t_min == t_dz) {
                        physics->velocity.z = 0;
                    }
                }
            }

            entity->previus_transform = entity->transform;

            delete hitbox;
        }

        if(collide(player->feet, block)) {
            Log::log("collide\n");
            player->physics->velocity.y = 0;
            player->transform.position.y = block->size.y + e->transform.position.y;
        }
    }
}
