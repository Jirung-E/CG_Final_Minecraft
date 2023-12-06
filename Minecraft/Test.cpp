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
    light2->material.base_color = ColorRGB { RGB_Red | ColorRGB { RGB_White, 0.7f }, 0.4f };
    light2->addComponent<Light>();
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

    

    // 1. 시선과 겹치는 오브젝트 검출
    // 2. 그중 가장 가까운 오브젝트 찾기
    // 3. 블럭인 경우 오브젝트 하이라이팅
    // 4. 좌/우 클릭에 따라 처리
    // 오브젝트의 크기는 1*2*1을 넘지 않는다.(블럭: 1*1*1, 플레이어 히트박스: 0.5*1.6*0.5)

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
