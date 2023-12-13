#include "SoulTorch.h"

#include "../../Game/Texture.h"
#include "../../Game/Component/Light.h"

using namespace std;


SoulTorch::SoulTorch(const string& id) : Block { id } {
    model->texture_id.push_back(Texture::get("Resource/Textures/soul_torch.png").getID());

    initModel();

    getComponent<AABB>()->is_trigger = true;

    addComponent<Light>();
    Light* light = getComponent<Light>();
    light->color = { RGB_Cyan | ColorRGB { RGB_White, 0.5f } };
    light->position.y = 0.5f;
    light->ambient = 0.5f;
    //light->c1 = 0.1f;
    light->c2 = 0.1f;
}


void SoulTorch::initModel() {
    model->texture_coords.clear();

    float width = 2.0f/16.0f;
    float height = 10.0f/16.0f;
    transform.scale = { width, height, width };

    float start_x = 7.0f/16.0f;
    float start_y = 0;
    float end_x = 9.0f/16.0f;
    float end_y = 10.0f/16.0f;

    // front
    model->texture_coords.push_back({ start_x, start_y });
    model->texture_coords.push_back({ end_x,   start_y });
    model->texture_coords.push_back({ start_x, end_y });
    model->texture_coords.push_back({ end_x,   end_y });
    // back
    model->texture_coords.push_back({ start_x, start_y });
    model->texture_coords.push_back({ end_x,   start_y });
    model->texture_coords.push_back({ start_x, end_y });
    model->texture_coords.push_back({ end_x,   end_y });

    float bottom_end_y = 2.0f/16.0f;
    float top_start_y = 8.0f/16.0f;
    // top
    model->texture_coords.push_back({ start_x, top_start_y });
    model->texture_coords.push_back({ end_x,   top_start_y });
    model->texture_coords.push_back({ start_x, end_y });
    model->texture_coords.push_back({ end_x,   end_y });
    // bottom
    model->texture_coords.push_back({ start_x, start_y });
    model->texture_coords.push_back({ end_x,   start_y });
    model->texture_coords.push_back({ start_x, bottom_end_y });
    model->texture_coords.push_back({ end_x,   bottom_end_y });

    // left
    model->texture_coords.push_back({ start_x, start_y });
    model->texture_coords.push_back({ end_x,   start_y });
    model->texture_coords.push_back({ start_x, end_y });
    model->texture_coords.push_back({ end_x,   end_y });
    // right
    model->texture_coords.push_back({ start_x, start_y });
    model->texture_coords.push_back({ end_x,   start_y });
    model->texture_coords.push_back({ start_x, end_y });
    model->texture_coords.push_back({ end_x,   end_y });


    // front
    model->polygons[4].v1.texture_coord = 0;
    model->polygons[4].v2.texture_coord = 1;
    model->polygons[4].v3.texture_coord = 3;
    model->polygons[5].v1.texture_coord = 3;
    model->polygons[5].v2.texture_coord = 2;
    model->polygons[5].v3.texture_coord = 0;
    // back
    model->polygons[8].v1.texture_coord = 4;
    model->polygons[8].v2.texture_coord = 5;
    model->polygons[8].v3.texture_coord = 7;
    model->polygons[9].v1.texture_coord = 7;
    model->polygons[9].v2.texture_coord = 6;
    model->polygons[9].v3.texture_coord = 4;
    // top
    model->polygons[2].v1.texture_coord = 8;
    model->polygons[2].v2.texture_coord = 9;
    model->polygons[2].v3.texture_coord = 11;
    model->polygons[3].v1.texture_coord = 11;
    model->polygons[3].v2.texture_coord = 10;
    model->polygons[3].v3.texture_coord = 8;
    // bottom
    model->polygons[0].v1.texture_coord = 12;
    model->polygons[0].v2.texture_coord = 13;
    model->polygons[0].v3.texture_coord = 15;
    model->polygons[1].v1.texture_coord = 15;
    model->polygons[1].v2.texture_coord = 14;
    model->polygons[1].v3.texture_coord = 12;
    // left
    model->polygons[6].v1.texture_coord = 16;
    model->polygons[6].v2.texture_coord = 17;
    model->polygons[6].v3.texture_coord = 19;
    model->polygons[7].v1.texture_coord = 19;
    model->polygons[7].v2.texture_coord = 18;
    model->polygons[7].v3.texture_coord = 16;
    // right
    model->polygons[10].v1.texture_coord = 20;
    model->polygons[10].v2.texture_coord = 21;
    model->polygons[10].v3.texture_coord = 23;
    model->polygons[11].v1.texture_coord = 23;
    model->polygons[11].v2.texture_coord = 22;
    model->polygons[11].v3.texture_coord = 20;
}
