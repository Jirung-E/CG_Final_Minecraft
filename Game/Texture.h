#pragma once

#include <unordered_map>
#include <string>


class Texture {
private:
    unsigned int id;

    static std::unordered_map<std::string, Texture> textures;

public:
    Texture();
    Texture(unsigned int id);

public:
    unsigned int getID() const;
    static Texture& get(const std::string& path);
};
