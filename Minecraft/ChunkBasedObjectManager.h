#pragma once 

#include "../Core/ObjectManager.h"
#include "../Objects/Player.h"
#include "../Objects/Block.h"
#include "../Game/Entity.h"


class ChunkInfo {
public:
    int x;
    int y;
    int z;
    static const int chunk_size;

public:
    ChunkInfo(int x, int y, int z);
    ChunkInfo(const Vector3& position);
    ChunkInfo(const ChunkInfo& other);

    bool operator==(const ChunkInfo& other) const;
    bool operator!=(const ChunkInfo& other) const;
};


template <>
struct std::hash<ChunkInfo> {
    size_t operator()(const ChunkInfo& info) const {
        std::string str = std::to_string(info.x) + " " + std::to_string(info.y) + " " + std::to_string(info.z);
        return std::hash<std::string>()(str);
    }
};


class ChunkBasedObjectManager {
public:
    ObjectManager& objects;
    Player* player;
    ChunkInfo player_prev_chunk;
    std::unordered_set<Block*> blocks;     // ���� ���� �� ����Ʈ
    std::unordered_set<Entity*> entities;  // ���� ���� ��ƼƼ ����Ʈ
    std::unordered_set<Object*> objects_in_radius;  // ���� ���� ������Ʈ ����Ʈ
    std::unordered_set<ChunkInfo> chunks_in_radius; // ���� ���� ûũ ����Ʈ
    //int loaded_radius;

protected:
    std::unordered_map<ChunkInfo, std::unordered_set<Object*>> chunk_info;

public:
    ChunkBasedObjectManager(ObjectManager& objects);

public:
    void add(const std::string& name, Object* object);
    void remove(const std::string& name);
    Object* get(const std::string& name) const;
    void clear();
    void deleteAll();

    std::unordered_set<Object*> getObjectsInChunk(const ChunkInfo& chunk) const;
    std::unordered_set<Object*> getObjectsInChunk(int chunk_x, int chunk_y, int chunk_z) const;    // Ư�� ûũ ���� ������Ʈ���� ������
    std::unordered_set<Object*> getObjectsInRadius(const Vector3& position, int radius);           // radius: ûũ ����
    
    void update(float dt, int radius);  // �÷��̾��� ��ġ�� �������� radius ûũ ���� ������Ʈ ������Ʈ
};
