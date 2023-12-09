#include "Stone.h"

#include "../../Game/Texture.h"

using namespace std;


Stone::Stone(const string& id) : Block { id } {
    model->texture_id.push_back(Texture::get("Resource/Textures/stone.png").getID());
    material.reflectivity = 0.2f;
    material.shininess = 16;
}
