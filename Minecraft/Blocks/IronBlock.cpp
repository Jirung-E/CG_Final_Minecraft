#include "IronBlock.h"

#include "../../Game/Texture.h"

using namespace std;


IronBlock::IronBlock(const string& id) : Block { id } {
    model->texture_id.push_back(Texture::get("Resource/Textures/iron_block.png").getID());
    material = { Material::metal, RGB_White };
}
