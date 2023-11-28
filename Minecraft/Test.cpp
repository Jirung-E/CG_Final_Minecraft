#include "Test.h"

#include "../Game/Component/Light.h"
#include "../Math/Line.h"

#include <gl/freeglut.h>

using namespace std;
using namespace Math;


Test::Test() : Game { "Test" },
events_handler { EventsHandler::getInstance(this) },
shader { "ShaderSource/vertex.glsl", "ShaderSource/fragment.glsl" },
view_mode { ViewMode::ThirdPerson },
vertical_sensitivity { 0.8f },
camera_distance { 4.0f },
interaction_distance { 4.0f },
simulation_distance { 2 }
{
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

    initObjects();
}

void Test::initObjects() {
    for(auto& e : objects) {
        if(e.second->parent == nullptr) {
            delete e.second;
        }
    }
    objects.clear();
    blocks.clear();

    std::pair<int, int> button_pos[] = {
        { -2, 1 },
        { -4, 2 },
        { -2, 3 },
        { -4, 4 },
        { -2, 5 },
        { -4, 6 },
    };
    Material gold { Material::metal };
    gold.base_color = ColorRGB { RGB_Yellow };
    for(auto& e : button_pos) {
        generateBlock(e.first, 2, e.second, gold);
        blocks.back()->addComponent<Physics>();
        blocks.back()->getComponent<Physics>()->velocity.x = 0.5f;
        blocks.back()->getComponent<Physics>()->gravity = { 0, 0, 0 };
    }
    int count = 10;
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
    objects.add("light1", light1);

    Box* light2 = new Box { "light2" };
    light2->transform.position = { 5, 5, 5 };
    light2->material.base_color = ColorRGB { RGB_Red | ColorRGB { RGB_White, 0.7f }, 0.4f };
    light2->addComponent<Light>();
    objects.add("light2", light2);
}

void Test::generatePlayerObject() {
    player = new Player { "player" };
    player->transform.position = { 0, 3, 0 };
    objects.add("player", player);
}

void Test::generateBlock(int x, int y, int z, const Material& material) {
    static int count = 1;
    string id = "block_" + to_string(count++);

    Block* block = new Block { };
    block->transform.position = { x+0.5f, y-0.5f, z+0.5f };
    block->material = material;

    objects.add(id, block);
    blocks.push_back(block);
}


void Test::rotateHead(float dx, float dy) {
    player->transform.rotateY(-dx);

    Vector3 direction = player->head->transform.forward();
    float s = dot(Vector3 { 0, 1, 0 }, direction);
    float angle = acos(s) + dy;
    if(angle <= 0) {
        dy = -acos(s);
    }
    else if(angle >= M_PI) {
        dy = static_cast<float>(M_PI - acos(s));
    }
    player->head->transform.rotateX(dy * vertical_sensitivity);
}


void Test::update() {
    Log::log("dt: %f", dt);

    //player->update(dt);   // ObjectManager���� ������Ʈ�� ûũ ������ ������Ʈ ��Ű�� ���⼭ �� �ʿ� ����
    ChunkInfo player_chunk { player->transform.position };
    Log::println("player chunk: %d, %d, %d", player_chunk.x, player_chunk.y, player_chunk.z);
    for(int x=player_chunk.x-simulation_distance; x<=player_chunk.x+simulation_distance; ++x) {
        for(int y=player_chunk.y-simulation_distance; y<=player_chunk.y+simulation_distance; ++y) {
            for(int z=player_chunk.z-simulation_distance; z<=player_chunk.z+simulation_distance; ++z) {
                objects.update(x, y, z, dt);

                for(auto& e : objects.getObjectsInChunk(x, y, z)) {
                    if(e == player) continue;
                    //e->transform.position.y += dt;

                    AABB* hitbox = e->getComponent<AABB>();
                    if(collide(player->hitbox, hitbox)) {
                        player->update(-dt);        // TODO: ���� ó�� ����� �ؾ���
                    }
                    if(collide(player->feet, hitbox)) {
                        player->physics->velocity.y = 0;
                        player->transform.position.y = hitbox->size.y/2.0f + e->transform.position.y;
                    }
                }
            }
        }
    }

    if(space_pressed) {
        player->jump();
    }

    auto head = player->head;
    switch(view_mode) {
        case FirstPerson: {
            Vector3 cam_pos { head->absoluteTransformMatrix() * Vector4 { 0, 0, 0, 1 } };
            cam_pos.y += 0.5f;
            camera.transform.position = cam_pos;
            Vector3 cam_rot { head->absoluteTransformMatrix() * Vector4 { 0, 0, 1, 0 } };
            camera.lookAt(cam_pos + cam_rot);
        }
        break;
        case SecondPerson: {
            Vector3 cam_pos { head->absoluteTransformMatrix() * Vector4 { 0, 1.8f/8, camera_distance, 1 } };
            camera.transform.position = cam_pos;
            Vector3 cam_rot { head->absoluteTransformMatrix() * Vector4 { 0, 0, 1, 0 } };
            camera.lookAt(cam_pos - cam_rot);
        }
        break;
        case ThirdPerson: {
            Vector3 cam_pos { head->absoluteTransformMatrix() * Vector4 { 0, 1.8f/8, -camera_distance, 1 } };
            camera.transform.position = cam_pos;
            Vector3 cam_rot { head->absoluteTransformMatrix() * Vector4 { 0, 0, 1, 0 } };
            camera.lookAt(cam_pos + cam_rot);
        }
        break;
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

    // 1. �ü��� ��ġ�� ������Ʈ ����
    // 2. ���� ���� ����� ������Ʈ ã��
    // 3. ���� ��� ������Ʈ ���̶�����
    // 4. ��/�� Ŭ���� ���� ó��

    // ������Ʈ�� ũ��� 1*2*1�� ���� �ʴ´�.(��: 1*1*1, �÷��̾� ��Ʈ�ڽ�: 0.5*1.6*0.5)

    //if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    //    Vector3 ray = player->head->transform.forward();
    //    Line line { Point { camera.transform.position }, Vector { ray } };
    //    float min_distance = INFINITY;
    //    Block* min_block = nullptr;
    //    for(auto& block : blocks) {
    //        float distance = distanceBetween(line, block->hitbox);
    //        if(distance < min_distance) {
    //            min_distance = distance;
    //            min_block = block;
    //        }
    //    }
    //    if(min_distance < interaction_distance) {
    //        min_block->material.base_color = convertHSVToRGB({ random<int>({ 120, 240 }), 0.5f, 1.0f });
    //    }
    //}
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
