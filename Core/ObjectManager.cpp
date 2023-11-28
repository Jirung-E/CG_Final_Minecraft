#include "ObjectManager.h"

using namespace std;


const int ChunkInfo::chunk_size = 2;        // 테스트를 위해 2로 설정

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


void ObjectManager::add(const string& name, Object* object) {
    objects[name] = object;
    chunk_info[ChunkInfo { object->transform.position }].push_back(object);
}

void ObjectManager::remove(const string& name) {
    Object* object = objects.at(name);
    objects.erase(name);
    auto& chunk_objects = chunk_info[ChunkInfo { object->transform.position }];
    chunk_objects.erase(find(chunk_objects.begin(), chunk_objects.end(), object));
}

Object* ObjectManager::get(const string& name) const {
    return objects.at(name);
}

vector<Object*> ObjectManager::getObjectsInChunk(int chunk_x, int chunk_y, int chunk_z) const {
    if(chunk_info.find(ChunkInfo { chunk_x, chunk_y, chunk_z }) == chunk_info.end()) {
        return vector<Object*> { };
    }
    return chunk_info.at(ChunkInfo { chunk_x, chunk_y, chunk_z });
}

vector<Object*> ObjectManager::getObjectsInChunk(const ChunkInfo& chunk) const {
    return getObjectsInChunk(chunk.x, chunk.y, chunk.z);
}

void ObjectManager::clear() {
    objects.clear();
    chunk_info.clear();
}

void ObjectManager::deleteAll() {
    for(auto& e : objects) {
        delete e.second;
    }
    clear();
}


void ObjectManager::update(float dt) {
    for(auto& e : objects) {
        ChunkInfo chunk { e.second->transform.position };

        e.second->update(dt);

        ChunkInfo new_chunk { e.second->transform.position };
        if(new_chunk != chunk) {
            chunk_info[new_chunk].push_back(e.second);
            chunk_info[chunk].erase(find(chunk_info[chunk].begin(), chunk_info[chunk].end(), e.second));
        }
    }
}

void ObjectManager::update(int chunk_x, int chunk_y, int chunk_z, float dt) {
    ChunkInfo chunk { chunk_x, chunk_y, chunk_z };
    for(auto it = chunk_info[chunk].begin(); it != chunk_info[chunk].end(); ++it) {
        (*it)->update(dt);

        ChunkInfo new_chunk { (*it)->transform.position };
        if(new_chunk != chunk) {
            chunk_info[new_chunk].push_back(*it);
            chunk_info[chunk].erase(it--);
        }
    }
}
