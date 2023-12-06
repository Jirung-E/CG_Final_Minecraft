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
            AABB* hitbox = entity->hitbox;
            
            auto mat = prev_transform.translationMatrix() * prev_transform.scaleMatrix();
            Vector3 prev_center = { mat * Vector4 { hitbox->center, 1 } };
            Vector3 size = mat * Vector4 { hitbox->size, 0 };

            mat = entity->transform.translationMatrix() * entity->transform.scaleMatrix();
            Vector3 curr_center = { mat * Vector4 { hitbox->center, 1 } };

            mat = e->transform.translationMatrix() * e->transform.scaleMatrix();
            Vector3 block_center = { mat * Vector4 { block->center, 1 } };
            Vector3 block_size = mat * Vector4 { block->size, 0 };

            float wx = size.x / 2.0f;
            float wy = size.y / 2.0f;
            float wz = size.z / 2.0f;

            float prev_top = prev_center.y + wy;
            float prev_bottom = prev_center.y - wy;
            float prev_left = prev_center.x - wx;
            float prev_right = prev_center.x + wx;
            float prev_front = prev_center.z + wz;
            float prev_back = prev_center.z - wz;

            float curr_top = curr_center.y + wy;
            float curr_bottom = curr_center.y - wy;
            float curr_left = curr_center.x - wx;
            float curr_right = curr_center.x + wx;
            float curr_front = curr_center.z + wz;
            float curr_back = curr_center.z - wz;

            float block_wx = block_size.x / 2.0f;
            float block_wy = block_size.y / 2.0f;
            float block_wz = block_size.z / 2.0f;
            float block_top = block_center.y + block_wy;
            float block_bottom = block_center.y - block_wy;
            float block_left = block_center.x - block_wx;
            float block_right = block_center.x + block_wx;
            float block_front = block_center.z + block_wz;
            float block_back = block_center.z - block_wz;

            Vector3 vel = (curr_center - prev_center) / dt;
            
            float tx = INFINITY;    // x축이 충돌한 시간
            float ty = INFINITY;
            float tz = INFINITY;
            bool already_collide_x = false;
            bool already_collide_y = false;
            bool already_collide_z = false;

            // x축 충돌
            if(block_left <= curr_left && curr_left <= block_right) {
                tx = 0;
                already_collide_x = true;
            }
            else if(block_left <= curr_right && curr_right <= block_right) {
                tx = 0;
                already_collide_x = true;
            }
            if(vel.x > 0) {
                if(prev_right <= block_left && block_left <= curr_right) {
                    tx = (block_left - prev_right) / vel.x;
                    already_collide_x = false;
                }
            }
            else if(vel.x < 0) {
                if(curr_left <= block_right && block_right <= prev_left) {
                    tx = (block_right - prev_left) / vel.x;
                    already_collide_x = false;
                }
            }
            // y
            if(block_bottom <= curr_bottom && curr_bottom <= block_top) {
                ty = 0;
                already_collide_y = true;
            }
            else if(block_bottom <= curr_top && curr_top <= block_top) {
                ty = 0;
                already_collide_y = true;
            }
            if(vel.y > 0) {
                if(prev_top <= block_bottom && block_bottom <= curr_top) {
                    ty = (block_bottom - prev_top) / vel.y;
                    already_collide_y = false;
                }
            }
            else if(vel.y < 0) {
                if(curr_bottom <= block_top && block_top <= prev_bottom) {
                    ty = (block_top - prev_bottom) / vel.y;
                    already_collide_y = false;
                }
            }
            // z
            if(block_back <= curr_back && curr_back <= block_front) {
                tz = 0;
                already_collide_z = true;
            }
            else if(block_back <= curr_front && curr_front <= block_front) {
                tz = 0;
                already_collide_z = true;
            }
            if(vel.z > 0) {
                if(prev_front <= block_back && block_back <= curr_front) {
                    tz = (block_back - prev_front) / vel.z;
                    already_collide_z = false;
                }
            }
            else if(vel.z < 0) {
                if(curr_back <= block_front && block_front <= prev_back) {
                    tz = (block_front - prev_back) / vel.z;
                    already_collide_z = false;
                }
            }


            if(tx > dt || tx < 0 || ty > dt || ty < 0 || tz > dt || tz < 0) {
                continue;
            }

            float t = std::max(tx, std::max(ty, tz));

            Vector3 hit_point = prev_center + vel * t;
            float hit_left = hit_point.x - wx;
            float hit_right = hit_point.x + wx;
            float hit_top = hit_point.y + wy;
            float hit_bottom = hit_point.y - wy;
            float hit_front = hit_point.z + wz;
            float hit_back = hit_point.z - wz;
            if(hit_left > block_right || hit_right < block_left || 
                hit_top < block_bottom || hit_bottom > block_top || 
                hit_front < block_back || hit_back > block_front) {
                continue;
            }

            entity->transform.position = hit_point;
            entity->transform.position.y -= wy;
            Log::log("tx: %f", tx);
            Log::log("ty: %f", ty);
            Log::log("tz: %f", tz);
            Log::log("player position: %f %f %f", entity->transform.position.x, entity->transform.position.y, entity->transform.position.z);
            
            Physics* physics = entity->getComponent<Physics>();
            Vector3 d = curr_center - hit_point;
            if(tx == t && !already_collide_x) {
                Log::log("x 충돌");
                d.x = 0;
                if(physics != nullptr) {
                    physics->velocity.x = 0;
                }
            }
            if(ty == t && !already_collide_y) {
                Log::log("y 충돌");
                d.y = 0;
                if(physics != nullptr) {
                    physics->velocity.y = 0;
                }
            }
            if(tz == t && !already_collide_z) {
                Log::log("z 충돌");
                d.z = 0;
                if(physics != nullptr) {
                    physics->velocity.z = 0;
                }
            }

            entity->transform.position += d;

            Log::log("\n");
        }

        if(collide(player->feet, block)) {
            //player->physics->velocity.y = 0;
            //player->transform.position.y = block->size.y + e->transform.position.y;
        }
    }
}
