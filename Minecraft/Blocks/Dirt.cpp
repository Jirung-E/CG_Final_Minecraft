#include "Dirt.h"

#include "../../Game/Texture.h"

using namespace std;


Dirt::Dirt(const string& id) : Block { id } {
    model->texture_id.push_back(Texture::get("Resource/Textures/dirt.png").getID());
    material.reflectivity = 0.1f;
    material.shininess = 16;
}
