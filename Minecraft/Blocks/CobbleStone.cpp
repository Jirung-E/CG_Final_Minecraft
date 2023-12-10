#include "CobbleStone.h"

#include "../../Game/Texture.h"

using namespace std;


CobbleStone::CobbleStone(const string& id) : Block { id } {
    model->texture_id.push_back(Texture::get("Resource/Textures/cobblestone.png").getID());
    material.reflectivity = 0.2f;
    material.shininess = 16;
}
