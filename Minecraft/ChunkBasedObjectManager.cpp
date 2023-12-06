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
            Vector3 block_center = { mat * Vector4 { block->center, 1 } };
            Vector3 block_size = mat * Vector4 { block->size, 0 };

            float wx1 = size1.x / 2.0f;
            float wy1 = size1.y / 2.0f;
            float wz1 = size1.z / 2.0f;
            float top1 = center1.y + wy1;
            float bottom1 = center1.y - wy1;
            float left1 = center1.x - wx1;
            float right1 = center1.x + wx1;
            float front1 = center1.z + wz1;
            float back1 = center1.z - wz1;

            float block_wx = block_size.x / 2.0f;
            float block_wy = block_size.y / 2.0f;
            float block_wz = block_size.z / 2.0f;
            float block_top = block_center.y + block_wy;
            float block_bottom = block_center.y - block_wy;
            float block_left = block_center.x - block_wx;
            float block_right = block_center.x + block_wx;
            float block_front = block_center.z + block_wz;
            float block_back = block_center.z - block_wz;

            Vector3 dir = (entity->transform.position - prev_transform.position) / dt;

            float t_dy = INFINITY;
            if(dir.y != 0) {
                float t_dy1 = (block_bottom - top1) / dir.y;
                float t_dy2 = (bottom1 - block_top) / dir.y;
                if(t_dy1 >= 0 && t_dy2 >= 0) {
                    t_dy = fminf(t_dy1, t_dy2);
                }
            }
            float t_dx = INFINITY;
            if(dir.x != 0) {
                float t_dx1 = (block_left - right1) / dir.x;
                float t_dx2 = (left1 - block_right) / dir.x;
                if(t_dx1 >= 0 && t_dx2 >= 0) {
                    t_dx = fminf(t_dx1, t_dx2);
                }
            }
            float t_dz = INFINITY;
            if(dir.z != 0) {
                float t_dz1 = (block_back - front1) / dir.z;
                float t_dz2 = (back1 - block_front) / dir.z;
                if(t_dz1 >= 0 && t_dz2 >= 0) {
                    t_dz = fminf(t_dz1, t_dz2);
                }
            }

            // 보간되는 구간 사이에 충돌이 있는지 체크
            if(t_dx <= dt || t_dy <= dt || t_dz <= dt) {
                float t_min = fminf(t_dy, fminf(t_dx, t_dz));

                // t_min에서 충돌하는지 체크
                // ...

                Log::log("dt: %f", dt);
                Log::log("t_min: %f", t_min);
                Log::log("dir * t_min: %f %f %f\n", dir.x * t_min, dir.y * t_min, dir.z * t_min);
                entity->transform.position = entity->previus_transform.position + dir * t_min;
                entity->move(t_min);
                Physics* physics = entity->getComponent<Physics>();
                if(physics != nullptr) {
                    if(t_dx <= dt) {
                        physics->velocity.x = 0;
                    } 
                    if(t_dy <= dt) {
                        physics->velocity.y = 0;
                    } 
                    if(t_dz <= dt) {
                        physics->velocity.z = 0;
                    }
                }
            }

            //entity->previus_transform = entity->transform;

            delete hitbox;
        }

        if(collide(player->feet, block)) {
            player->physics->velocity.y = 0;
            player->transform.position.y = block->size.y + e->transform.position.y;
        }
    }
}
