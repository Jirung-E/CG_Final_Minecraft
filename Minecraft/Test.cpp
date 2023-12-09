#include "Test.h"

#include "ChunkBasedObjectManager.h"
#include "../Game/Component/Light.h"
#include "../Game/Texture.h"
#include "../Math/Line.h"

#include <gl/freeglut.h>

using namespace std;
using namespace Math;


Test::Test() : Game { "Test" },
events_handler { EventsHandler::getInstance(this) },
shader { "ShaderSource/vertex.glsl", "ShaderSource/fragment.glsl" },
objects_manager { objects },
view_mode { ViewMode::FirstPerson },
vertical_sensitivity { 0.8f },
camera_distance { 4.0f },
interaction_distance { 4.0f },
simulation_distance { 2 }
{
    hide_cursor = true;
    renderer.icons_texture_id = Texture::get("Resource/Textures/icons.png").getID();
    events_handler.link();
    renderer.setShader(&shader);
    renderer.camera = &camera;
    renderer.background_color = ColorRGB { 0x82, 0xB2, 0xFF };
    initWorld();
}

// --------------------------------------------------------------------------------------------- //

void Test::initWorld() {
    camera.transform.position = { 0.0f, 0.0f, 4.0f };
    camera.lookAt({ 0, 0, 0 });

    camera_direction = { -1, -1, -1 };

    focus_block = nullptr;
    focus_face = Face::Back;
    focus_entity = nullptr;

    initObjects();
}

void Test::initObjects() {
    objects_manager.deleteAll();

    for(int x=-6; x<0; ++x) {
        for(int z=1; z<7; ++z) {
            generateBlock(x, 2, z, Material::base);
        }
    }
    int count = 20;
    for(int i=-count; i<count; ++i) {
        for(int k=-count; k<count; ++k) {
            generateBlock(i, 1, k, (i+k)%2 ? Material::metal : Material::basic);
        }
    }
    Material m { Material::basic };
    m.base_color = convertHSVToRGB({ random<int>({ 120, 240 }), 0.5f, 1.0f });
    generateBlock(1, 2, 0, m);
    generateBlock(1, 3, 0, m);
    generateBlock(1, 4, 0, m);
    generateBlock(1, 5, 0, m);
    generateBlock(2, 5, 0, m);
    generateBlock(3, 5, 0, m);
    generateBlock(4, 5, 0, m);
    generateBlock(4, 4, 0, m);
    generateBlock(5, 4, 0, m);
    generateBlock(5, 3, 0, m);
    generateBlock(6, 3, 0, m);
    generateBlock(6, 2, 0, m);
    generateBlock(7, 2, 0, m);

    generatePlayerObject();

    Box* light1 = new Box { "light1" };
    light1->transform.position = { 0, 20, 0 };
    light1->material.base_color = ColorRGB { RGB_White };
    light1->addComponent<Light>();
    light1->getComponent<Light>()->ambient = 1.0f;
    objects_manager.add("light1", light1);

    Box* light2 = new Box { "light2" };
    light2->transform.position = { 5, 5, 5 };
    light2->material.base_color = ColorRGB { RGB_Red | ColorRGB { RGB_White, 0.5f }, 0.8f };
    light2->addComponent<Light>();
    Light* l2 = light2->getComponent<Light>();
    l2->ambient = 0.5f;
    l2->c1 = 0.01f;
    l2->c2 = 0.01f;
    objects.add("light2", light2);
}

void Test::generatePlayerObject() {
    player = new Player { "player" };
    player->transform.position = { 0, 3, 0 };
    objects_manager.player = player;
    objects_manager.add("player", player);
}

void Test::generateBlock(int x, int y, int z, const Material& material) {
    static int count = 1;
    string id = "block_" + to_string(count++);

    Block* block = new Block { id };
    block->transform.position = { x+0.5f, y, z+0.5f };
    block->material = material;
    block->model->texture_id.push_back(Texture::get("Resource/Textures/stone.png").getID());

    objects_manager.add(id, block);
}


void Test::rotateHead(float dx, float dy) {
    player->transform.rotateY(-dx);

    Vector3 direction = player->head->transform.forward();
    float s = dot(Vector3 { 0, 1, 0 }, direction);
    float angle = acos(s) + dy;
    if(angle <= 0) {
        dy = -acos(s);
    }
    else if(angle >= PI) {
        dy = static_cast<float>(PI - acos(s));
    }
    player->head->transform.rotateX(dy * vertical_sensitivity);
}


void Test::update() {
    //Log::log("dt: %f", dt);
    //Log::log("player: %f %f %f", player->transform.position.x, player->transform.position.y, player->transform.position.z);

    objects_manager.update(dt, simulation_distance);

    if(space_pressed) {
        player->jump();
    }

    auto head = player->head;
    switch(view_mode) {
        case FirstPerson: {
            Vector3 cam_pos { head->absoluteTransformMatrix() * Vector4 { 0, 0, 0, 1 } };
            cam_pos.y += 0.2f;
            camera.transform.position = cam_pos;
            Vector3 cam_rot { head->absoluteTransformMatrix() * Vector4 { 0, 0, 1, 0 } };
            camera.lookAt(cam_pos + cam_rot);
            player->render = false;
        }
        break;
        case SecondPerson: {
            Vector3 cam_pos { head->absoluteTransformMatrix() * Vector4 { 0, 1.8f/8, camera_distance, 1 } };
            camera.transform.position = cam_pos;
            Vector3 cam_rot { head->absoluteTransformMatrix() * Vector4 { 0, 0, 1, 0 } };
            camera.lookAt(cam_pos - cam_rot);
            player->render = true;
        }
        break;
        case ThirdPerson: {
            Vector3 cam_pos { head->absoluteTransformMatrix() * Vector4 { 0, 1.8f/8, -camera_distance, 1 } };
            camera.transform.position = cam_pos;
            Vector3 cam_rot { head->absoluteTransformMatrix() * Vector4 { 0, 0, 1, 0 } };
            camera.lookAt(cam_pos + cam_rot);
            player->render = true;
        }
        break;
    }


    // hit test
    auto mat = player->head->absoluteTransformMatrix();
    Vector3 ray_origin { mat * Vector4 { 0, 0, 0, 1 } };
    ray_origin.y += 0.2f;
    Vector3 ray_direction { mat * Vector4 { 0, 0, 1, 0 } };
    Vector3 ray_end = ray_origin + ray_direction * interaction_distance;

    float interaction2 = interaction_distance * interaction_distance;
    float min_distance = interaction_distance;

    focus_block = nullptr;

    for(const auto& block : objects_manager.blocks) {
        if(distance2(block->transform.position, player->transform.position) > interaction2) {
            continue;
        }

        mat = block->absoluteTransformMatrix();
        AABB* collider = block->getComponent<AABB>();
        Vector3 block_center { mat * Vector4 { collider->center, 1 } };
        Vector3 block_size { mat * Vector4 { collider->size, 0 } };

        float block_wx = block_size.x / 2.0f;
        float block_wy = block_size.y / 2.0f;
        float block_wz = block_size.z / 2.0f;
        float block_top = block_center.y + block_wy;
        float block_bottom = block_center.y - block_wy;
        float block_left = block_center.x - block_wx;
        float block_right = block_center.x + block_wx;
        float block_front = block_center.z + block_wz;
        float block_back = block_center.z - block_wz;

        float tx = INFINITY;    // x축이 충돌한 시간
        float ty = INFINITY;
        float tz = INFINITY;

        // x
        if(ray_direction.x != 0) {
            float t1 = (block_left - ray_origin.x) / ray_direction.x;
            float t2 = (block_right - ray_origin.x) / ray_direction.x;
            tx = std::min(t1, t2);
            if(tx < 0) {
                tx = std::max(t1, t2);
            }
        }
        if(block_left <= ray_origin.x && ray_origin.x <= block_right) {
            if(block_left <= ray_end.x && ray_end.x <= block_right) {
                tx = 0;
            }
        }
        // y
        if(ray_direction.y != 0) {
            float t1 = (block_bottom - ray_origin.y) / ray_direction.y;
            float t2 = (block_top - ray_origin.y) / ray_direction.y;
            ty = std::min(t1, t2);
            if(ty < 0) {
                ty = std::max(t1, t2);
            }
        }
        if(block_bottom <= ray_origin.y && ray_origin.y <= block_top) {
            if(block_bottom <= ray_end.y && ray_end.y <= block_top) {
                ty = 0;
            }
        }
        // z
        if(ray_direction.z != 0) {
            float t1 = (block_back - ray_origin.z) / ray_direction.z;
            float t2 = (block_front - ray_origin.z) / ray_direction.z;
            tz = std::min(t1, t2);
            if(tz < 0) {
                tz = std::max(t1, t2);
            }
        }
        if(block_back <= ray_origin.z && ray_origin.z <= block_front) {
            if(block_back <= ray_end.z && ray_end.z <= block_front) {
                tz = 0;
            }
        }

        if(tx >= min_distance || tx < 0 || ty >= min_distance || ty < 0 || tz >= min_distance || tz < 0) {
            continue;
        }

        float t = std::max(tx, std::max(ty, tz));

        Vector3 hit_point = ray_origin + ray_direction * t;
        if(hit_point.x < block_left || block_right < hit_point.x ||
            hit_point.y < block_bottom || block_top < hit_point.y ||
            hit_point.z < block_back || block_front < hit_point.z) {
            continue;
        }

        focus_block = block;
        min_distance = t;

        if(hit_point.x == block_left) {
            focus_face = Face::Left;
        }
        else if(hit_point.x == block_right) {
            focus_face = Face::Right;
        }
        else if(hit_point.y == block_top) {
            focus_face = Face::Top;
        }
        else if(hit_point.y == block_bottom) {
            focus_face = Face::Bottom;
        }
        else if(hit_point.z == block_front) {
            focus_face = Face::Front;
        }
        else if(hit_point.z == block_back) {
            focus_face = Face::Back;
        }
        else {
            continue;
        }
    }

    focus_entity = nullptr;
    min_distance = interaction_distance;
    for(const auto& entity : objects_manager.entities) {
        if(distance2(entity->transform.position, player->transform.position) > interaction2) {
            continue;
        }

        mat = entity->absoluteTransformMatrix();
        AABB* collider = entity->getComponent<AABB>();
        Vector3 block_center { mat * Vector4 { collider->center, 1 } };
        Vector3 block_size { mat * Vector4 { collider->size, 0 } };

        float block_wx = block_size.x / 2.0f;
        float block_wy = block_size.y / 2.0f;
        float block_wz = block_size.z / 2.0f;
        float block_top = block_center.y + block_wy;
        float block_bottom = block_center.y - block_wy;
        float block_left = block_center.x - block_wx;
        float block_right = block_center.x + block_wx;
        float block_front = block_center.z + block_wz;
        float block_back = block_center.z - block_wz;

        float tx = INFINITY;    // x축이 충돌한 시간
        float ty = INFINITY;
        float tz = INFINITY;

        // x
        if(ray_direction.x != 0) {
            float t1 = (block_left - ray_origin.x) / ray_direction.x;
            float t2 = (block_right - ray_origin.x) / ray_direction.x;
            tx = std::min(t1, t2);
            if(tx < 0) {
                tx = std::max(t1, t2);
            }
        }
        if(block_left <= ray_origin.x && ray_origin.x <= block_right) {
            if(block_left <= ray_end.x && ray_end.x <= block_right) {
                tx = 0;
            }
        }
        // y
        if(ray_direction.y != 0) {
            float t1 = (block_bottom - ray_origin.y) / ray_direction.y;
            float t2 = (block_top - ray_origin.y) / ray_direction.y;
            ty = std::min(t1, t2);
            if(ty < 0) {
                ty = std::max(t1, t2);
            }
        }
        if(block_bottom <= ray_origin.y && ray_origin.y <= block_top) {
            if(block_bottom <= ray_end.y && ray_end.y <= block_top) {
                ty = 0;
            }
        }
        // z
        if(ray_direction.z != 0) {
            float t1 = (block_back - ray_origin.z) / ray_direction.z;
            float t2 = (block_front - ray_origin.z) / ray_direction.z;
            tz = std::min(t1, t2);
            if(tz < 0) {
                tz = std::max(t1, t2);
            }
        }
        if(block_back <= ray_origin.z && ray_origin.z <= block_front) {
            if(block_back <= ray_end.z && ray_end.z <= block_front) {
                tz = 0;
            }
        }

        if(tx >= min_distance || tx < 0 || ty >= min_distance || ty < 0 || tz >= min_distance || tz < 0) {
            continue;
        }

        float t = std::max(tx, std::max(ty, tz));

        Vector3 hit_point = ray_origin + ray_direction * t;
        if(hit_point.x < block_left || block_right < hit_point.x ||
            hit_point.y < block_bottom || block_top < hit_point.y ||
            hit_point.z < block_back || block_front < hit_point.z) {
            continue;
        }

        focus_entity = entity;
        min_distance = t;
    }
}

// --------------------------------------------------------------------------------------------- //

void Test::keyboardEvent(unsigned char key) {
    Game::keyboardEvent(key);

    switch(key) {
    case 'i': case 'I':
        initWorld();
        break;
    case 'w': case 'W':
        player->move_direction.y += 1;
        break;
    case 'a': case 'A':
        player->move_direction.x -= 1;
        break;
    case 's': case 'S':
        player->move_direction.y -= 1;
        break;
    case 'd': case 'D':
        player->move_direction.x += 1;
        break;
    case ' ':
        space_pressed = true;
        break;
    case '\t':
        view_mode = (ViewMode)((view_mode + 1) % ViewMode::COUNT);
        if(view_mode == ViewMode::FirstPerson) {
            player->render = false;
        }
        else {
            player->render = true;
        }
        break;
    }
}

void Test::keyboardUpEvent(unsigned char key) {
    Game::keyboardUpEvent(key);

    switch(key) {
    case 'w': case 'W':
        player->move_direction.y -= 1;
        break;
    case 'a': case 'A':
        player->move_direction.x += 1;
        break;
    case 's': case 'S':
        player->move_direction.y += 1;
        break;
    case 'd': case 'D':
        player->move_direction.x -= 1;
        break;
    case ' ':
        space_pressed = false;
        break;
    }

}

void Test::mouseClickEvent(int button, int state, int x, int y) {
    Game::mouseClickEvent(button, state, x, y);




    switch(state) {
    case GLUT_DOWN:
        switch(button) {
        case GLUT_LEFT_BUTTON:
            if(focus_block != nullptr) {
                objects_manager.remove(focus_block->id);
                focus_block = nullptr;
            }
            break;
        case GLUT_RIGHT_BUTTON:
            if(focus_block != nullptr) {
                Vector3 pos = focus_block->transform.position;
                pos.x -= 0.5f;
                pos.z -= 0.5f;
                switch(focus_face) {
                case Face::Left:
                    pos.x -= 1;
                    break;
                case Face::Right:
                    pos.x += 1;
                    break;
                case Face::Top:
                    pos.y += 1;
                    break;
                case Face::Bottom:
                    pos.y -= 1;
                    break;
                case Face::Front:
                    pos.z += 1;
                    break;
                case Face::Back:
                    pos.z -= 1;
                    break;
                }
                generateBlock(pos.x, pos.y, pos.z, Material::base);
            }
            break;
        }
        break;
    }
}

void Test::mouseMotionEvent(const Vector2& delta) {
    Game::mouseMotionEvent(delta);

    // 1pixel -> 1degree
    Vector2 d = delta * sensitivity;
    d = radians(d);
    rotateHead(d.x, d.y);
}

void Test::mouseDragEvent(const Vector2& delta) {
    Game::mouseDragEvent(delta);

    mouseMotionEvent(delta);
}
