#include "GameScene.h"

#include "../../Game/Component/Light.h"
#include "../../Game/Texture.h"
#include "../../Math/Line.h"
#include "../Blocks/Blocks.h"

#include <thread>
#include <gl/freeglut.h>

using namespace std;
using namespace Math;


GameScene::GameScene(Game* game) : Scene { game },
shader { "ShaderSource/vertex.glsl", "ShaderSource/fragment.glsl" },
objects_manager { objects },
running { true },
debug_info_thread_running { true },
view_mode { ViewMode::FirstPerson },
vertical_sensitivity { 0.8f },
camera_distance { 4.0f },
interaction_distance { 4.0f },
simulation_distance { 2 },
render_distance { 3 }
{
    renderer.icons_texture_id = Texture::get("Resource/Textures/icons.png").getID();
    //renderer.background_color = ColorRGB { 0x82, 0xB2, 0xFF };
    renderer.background_color = RGB_Black;
    renderer.render_distance = render_distance * ChunkInfo::chunk_size;
    initWorld();
}

// --------------------------------------------------------------------------------------------- //

void GameScene::initWorld() {
    camera.transform.position = { 0.0f, 0.0f, 4.0f };
    camera.lookAt({ 0, 0, 0 });

    camera_direction = { -1, -1, -1 };

    focus_block = nullptr;
    focus_face = Face::Back;
    focus_entity = nullptr;

    torch_count = 0;

    selected_block = BRICK;

    initObjects();
}

void GameScene::initObjects() {
    objects_manager.deleteAll();

    int count = 64;
    for(int i=-count; i<count; ++i) {
        for(int k=-count; k<count; ++k) {
            if(random<int>({ 0, 1000 }) == 0) {
                generateBlock(TORCH, i, 4, k);
                ++torch_count;
            }
            generateBlock(GRASS, i, 3, k);
            generateBlock(DIRT, i, 2, k);
            generateBlock(STONE, i, 1, k);
            generateBlock(STONE, i, 0, k);
            generateBlock(BEDROCK, i, -1, k);
        }
    }
    Material m { Material::basic };
    m.base_color = convertHSVToRGB({ random<int>({ 120, 240 }), 0.5f, 1.0f });
    generateBlock(IRON_BLOCK, 1, 2+2, 0);
    generateBlock(IRON_BLOCK, 1, 3+2, 0);
    generateBlock(IRON_BLOCK, 1, 4+2, 0);
    generateBlock(IRON_BLOCK, 1, 5+2, 0);
    generateBlock(IRON_BLOCK, 2, 5+2, 0);
    generateBlock(IRON_BLOCK, 3, 5+2, 0);
    generateBlock(IRON_BLOCK, 4, 5+2, 0);
    generateBlock(IRON_BLOCK, 4, 4+2, 0);
    generateBlock(IRON_BLOCK, 5, 4+2, 0);
    generateBlock(IRON_BLOCK, 5, 3+2, 0);
    generateBlock(IRON_BLOCK, 6, 3+2, 0);
    generateBlock(IRON_BLOCK, 6, 2+2, 0);
    generateBlock(IRON_BLOCK, 7, 2+2, 0);

    generatePlayerObject();

    //Object* light1 = new Object { "light1" };
    //light1->model = new Model { Model::sphere };
    //light1->transform.position = { 0, 20, 0 };
    //light1->addComponent<Light>();
    //Light* light = light1->getComponent<Light>();
    //light->ambient = 0.5f;
    //light->color = RGB_White;
    //objects_manager.add("light1", light1);
}

void GameScene::generatePlayerObject() {
    player = new Player { "player" };
    player->transform.position = { 0, 5, 0 };
    objects_manager.player = player;
    objects_manager.add("player", player);
}

void GameScene::generateBlock(const BlockID& block_id, int x, int y, int z) {
    string id = to_string(x) + ":" + to_string(y) + ":" + to_string(z);

    Block* block = nullptr;
    switch(block_id) {
    case STONE:
        id = "stone " + id;
        block = new Stone { id };
        break;
    case COBBLE_STONE:
        id = "cobblestone " + id;
        block = new CobbleStone { id };
        break;
    case BRICK:
        id = "brick " + id;
        block = new Brick { id };
        break;
    case DIRT:
        id = "dirt " + id;
        block = new Dirt { id };
        break;
    case GRASS:
        id = "grass " + id;
        block = new Grass { id };
        break;
    case IRON_BLOCK:
        id = "iron_block " + id;
        block = new IronBlock { id };
        break;
    case BEDROCK:
        id = "bedrock " + id;
        block = new Bedrock { id };
        break;
    case TORCH:
        id = "torch " + id;
        block = new Torch { id };
        break;
    case AIR: default:
        id = "air " + id;
        block = new Block { id };
        break;
    }
    block->transform.position = { x+0.5f, y, z+0.5f };

    objects_manager.add(id, block);
}


void GameScene::rotateHead(float dx, float dy) {
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


void GameScene::start() {
    game->hide_cursor = true;
    game->fix_cursor_when_drag = true;
    game->fix_cursor_when_motion = true;
    renderer.setShader(&shader);
    renderer.camera = &camera;
    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);
    reshape(w, h);

    debug_info_thread = thread { [&]() {
        while(debug_info_thread_running) {
            sleep(0.1f);
            showDebugInfo();
        }
        running = false;
    } };
    debug_info_thread.detach();
}

void GameScene::exit() {
    debug_info_thread_running = false;
    Log::print_log = false;

    game->hide_cursor = false;
    game->fix_cursor_when_drag = false;
    game->fix_cursor_when_motion = false;


    while(running) {
        setCursorPosition(0, 0);
        Log::print("waiting for debug_info_thread to finish...");
    }

    if(scene_manager != nullptr) {
        scene_manager->popScene();
    }
}

void GameScene::update() {
    objects_manager.update(game->dt, simulation_distance);

    if(player->transform.position.y < -20) {
        initWorld();
        return;
    }
    if(torch_count <= 0) {
        initWorld();
        return;
    }

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

    float interaction2 = (float)pow(interaction_distance+2, 2);
    float min_distance = interaction_distance;
    float cam_min_distance = camera_distance;

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

void GameScene::drawScene() {
    if(focus_block != nullptr) {
        focus_block->material.base_color = RGB_LightGray;
    }
    auto objs = objects_manager.getObjectsInRadius(player->transform.position, 1+render_distance);
    for(auto object : objs) {
        if(object->getComponent<Light>() != nullptr) {
            renderer.pushLightObject(object);
        }
        else {
            renderer.pushObject(object);
        }
    }
    renderer.render();
    renderer.renderCrosshair();

    if(focus_block != nullptr) {
        focus_block->material.base_color = RGB_White;
    }
}

void GameScene::reshape(int w, int h) {
    camera.aspect = (float)w / h;
}

void GameScene::keyboardEvent(unsigned char key) {
    //Game::keyboardEvent(key);

    switch(key) {
    case 27:
        exit();
        break;
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
    case '1':
        selected_block = BRICK;
        break;
    case '2':
        selected_block = COBBLE_STONE;
        break;
    case '3':
        selected_block = IRON_BLOCK;
        break;
    case '4':
        selected_block = TORCH;
        break;
    }
}

void GameScene::keyboardUpEvent(unsigned char key) {
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
    case '+':
        player->move_speed = 20;
        break;
    case '=':
        player->move_speed = 4;
        break;
    }
}

void GameScene::specialKeyEvent(int key) {
    switch(key) {
    case GLUT_KEY_F3:
        Log::print_log = !Log::print_log;
        break;
    }
}

void GameScene::mouseClickEvent(int button, int state, int x, int y) {
    switch(state) {
    case GLUT_DOWN:
        switch(button) {
        case GLUT_LEFT_BUTTON:
            if(focus_block != nullptr) {
                if(focus_block->id.find("bedrock") != string::npos) {
                    break;
                }
                if(focus_block->id.find("torch") != string::npos) {
                    --torch_count;
                }
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
                generateBlock(selected_block, (int)pos.x, (int)pos.y, (int)pos.z);
            }
            break;
        }
        break;
    }
}

void GameScene::mouseMotionEvent(const Vector2& delta) {
    // 1pixel -> 1degree
    Vector2 d = delta * game->sensitivity;
    d = radians(d);
    rotateHead(d.x, d.y);
}

void GameScene::mouseDragEvent(const Vector2& delta) {
    mouseMotionEvent(delta);
}


void GameScene::showDebugInfo() const {
    setCursorPosition(0, 0);
    Log::log("FPS: %d                             ", int(1.0f / game->dt));
    Log::log("dt: %f                              ", game->dt);
    Log::log("player: %6.2f %6.2f %6.2f           ", player->transform.position.x, player->transform.position.y, player->transform.position.z);
    Log::log("player move speed: %f               ", player->move_speed);
    string face_str[] = { "Left", "Right", "Top", "Bottom", "Front", "Back" };
    Log::log("focus_block: %s                     ", focus_block == nullptr ? "null" : (focus_block->id + " ("+face_str[focus_face]+")").c_str());
    Log::log("focus_entity: %s                    ", focus_entity == nullptr ? "null" : focus_entity->id.c_str());
    Log::log("simulation_distance: %d             ", simulation_distance);
    Log::log("render_distance: %d                 ", render_distance);
    Log::log("view_mode: %d                       ", view_mode);
    Log::log("torch_count: %d                     ", torch_count);
    switch(selected_block) {
    case BRICK:
        Log::log("selected_block: BRICK               ");
        break;
    case COBBLE_STONE:
        Log::log("selected_block: COBBLE_STONE        ");
        break;
    case IRON_BLOCK:
        Log::log("selected_block: IRON_BLOCK          ");
        break;
    case TORCH:
        Log::log("selected_block: TORCH               ");
        break;
    }
}
