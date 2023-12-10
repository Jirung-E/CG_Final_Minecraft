#include "Bedrock.h"

#include "../../Game/Texture.h"

using namespace std;


Bedrock::Bedrock(const string& id) : Block { id } {
    model->texture_id.push_back(Texture::get("Resource/Textures/bedrock.png").getID());
    material.reflectivity = 0.2f;
    material.shininess = 16;
}
