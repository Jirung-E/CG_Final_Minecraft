#include "ChunkBasedObjectManager.h"

using namespace std;


const int ChunkInfo::chunk_size = 2;        // �׽�Ʈ�� ���� 2�� ����

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
    chunk_info[ChunkInfo { object->transform.position }].push_back(object);
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


vector<Object*> ChunkBasedObjectManager::getObjectsInChunk(const ChunkInfo& chunk) const {
    if(chunk_info.find(chunk) == chunk_info.end()) {
        return vector<Object*> { };
    }
    return chunk_info.at(chunk);
}

vector<Object*> ChunkBasedObjectManager::getObjectsInChunk(int chunk_x, int chunk_y, int chunk_z) const {
    return getObjectsInChunk(ChunkInfo { chunk_x, chunk_y, chunk_z });
}

vector<Object*> ChunkBasedObjectManager::getObjectsInRadius(const Vector3& position, int radius) {
    vector<Object*> result;
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
                    result.push_back(e);
                    if(Block* block = dynamic_cast<Block*>(e)) {        // TODO: ����ȭ �ʿ� - ȣ��ɶ����� dynamic_cast�� �ϸ� ��ȿ����
                        blocks.push_back(block);
                    }
                    else if(Entity* entity = dynamic_cast<Entity*>(e)) {
                        entities.push_back(entity);
                    }
                }
            }
        }
    }
    return result;
}


void ChunkBasedObjectManager::update(float dt, int radius) {
    auto objs = getObjectsInRadius(player->transform.position, radius);
    
    for(auto& e : objs) {
        ChunkInfo chunk { e->transform.position };

		e->update(dt);

        ChunkInfo new_chunk { e->transform.position };
        if(chunk != new_chunk) {
            chunk_info[new_chunk].push_back(e);
            auto it = find(chunk_info[chunk].begin(), chunk_info[chunk].end(), e);
            if(it != chunk_info[chunk].end()) {
                chunk_info[chunk].erase(it);
            }
        }
	}

    // TODO: ����ȭ �ʿ� - blocks�� �ݺ����� 2�� ��
    for(auto& e : blocks) {
        AABB* hitbox = e->getComponent<AABB>();     // �浹üũ�� ������ ��ġ�� ���� �Ǹ� �Ҽӵ� ûũ�� ����� �� ����. 
        // �����ϱ� ���ؼ��� �� ���� ������Ʈ, �� �ڿ� ��ƼƼ ������Ʈ
        // ������ ���� �������� �����Ŷ� �ϴ� �̴��
        if(collide(player->hitbox, hitbox)) {
            player->update(-dt);        // TODO: ���� ó�� ����� �ؾ���
        }
        if(collide(player->feet, hitbox)) {
            Log::println("player on block");
            player->physics->velocity.y = 0;
            player->transform.position.y = hitbox->size.y/2.0f + e->transform.position.y;
        }
    }
}
