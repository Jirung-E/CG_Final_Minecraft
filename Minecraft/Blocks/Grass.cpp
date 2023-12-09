#include "Grass.h"

#include "../../Game/Texture.h"

using namespace std;


Grass::Grass(const string& id) : Block { id } {
    model->texture_id.push_back(Texture::get("Resource/Textures/dirt.png").getID());
    model->texture_id.push_back(Texture::get("Resource/Textures/dirt.png").getID());
    model->texture_id.push_back(Texture::get("Resource/Textures/grass_carried.png").getID());
    model->texture_id.push_back(Texture::get("Resource/Textures/grass_carried.png").getID());
    model->texture_id.push_back(Texture::get("Resource/Textures/grass_side_carried.png").getID());
    material.reflectivity = 0.1f;
    material.shininess = 16;
}
