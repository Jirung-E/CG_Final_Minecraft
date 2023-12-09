#include "Brick.h"

#include "../../Game/Texture.h"

using namespace std;


Brick::Brick(const string& id) : Block { id } {
    model->texture_id.push_back(Texture::get("Resource/Textures/brick.png").getID());
    material.reflectivity = 0.3f;
    material.shininess = 16;
}
