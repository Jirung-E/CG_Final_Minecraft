#include "ChunkBasedObjectManager.h"

using namespace std;


const int ChunkInfo::chunk_size = 16;

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
        AABB* hitbox = e->getComponent<AABB>();     // 충돌체크로 보정된 위치에 가게 되면 소속된 청크가 변경될 수 있음. 
        // 방지하기 위해서는 블럭 먼저 업데이트, 그 뒤에 엔티티 업데이트
        // 하지만 블럭이 움직이지 않을거라 일단 이대로
        if(collide(player->hitbox, hitbox)) {
            player->update(-dt);        // TODO: 여기 처리 제대로 해야함
        }
        if(collide(player->feet, hitbox)) {
            player->physics->velocity.y = 0;
            player->transform.position.y = hitbox->size.y + e->transform.position.y;
        }
    }
}
