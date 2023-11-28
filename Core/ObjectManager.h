#pragma once

#include <string>
#include <unordered_map>

#include "../Game/Object.h"


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


class ObjectManager {
    friend class Game;

private:
    std::unordered_map<std::string, Object*> objects;
    std::unordered_map<ChunkInfo, std::vector<Object*>> chunk_info;
    
public:
    void add(const std::string& name, Object* object);
    void remove(const std::string& name);       // delete는 같이 하지 않음
    Object* get(const std::string& name) const;
    std::vector<Object*> getObjectsInChunk(int chunk_x, int chunk_y, int chunk_z) const;  // 특정 청크 안의 오브젝트들을 가져옴
    std::vector<Object*> getObjectsInChunk(const ChunkInfo& chunk) const;
    void clear();
    void deleteAll();

    void update(float dt);
    void update(int chunk_x, int chunk_y, int chunk_z, float dt);          // 특정 청크만 업데이트

    auto begin() const { return objects.begin(); }
    auto end() const { return objects.end(); }
};
