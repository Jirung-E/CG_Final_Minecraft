#include "LobyScene.h"

#include "GameScene.h"
#include "../../Game/Texture.h"

#include <gl/freeglut.h>

using namespace std;


LobyScene::LobyScene(Game* game) : Scene { game },
scene_manager { nullptr },
shader { "ShaderSource/vertex.glsl", "ShaderSource/fragment.glsl" },
background_texture { Texture::get("Resource/Textures/sunset_pending_keyart.png").getID() },
title_texture { Texture::get("Resource/Textures/title.png").getID() },
background { new Object { "loby_background" } },
title { new Object { "loby_title" } }
{
    background->model = new Model { Model::box };
    background->model->texture_id.push_back(background_texture);
    background->transform.scale = { 4.0f, 2.0f, 1.0f };
    background->transform.position = { 0.0f, -1.0f, 0.0f };
    title->model = new Model { Model::box };
    title->transform.scale = { 2.0f, 0.5f, 1.0f };
    title->transform.position = { 0.0f, 0.4f, 0.5f };
    title->model->texture_id.push_back(title_texture);

    camera.view_mode = Camera::Mode::Orthogonal;
    camera.transform.position = { 0.0f, 0.0f, 4.0f };
    camera.lookAt({ 0, 0, 0 });
    camera.height = 2.0f;
}

LobyScene::~LobyScene() {
    delete background;
    delete title;
}

void LobyScene::start() {
    game->hide_cursor = false;
    game->fix_cursor_when_drag = false;
    game->fix_cursor_when_motion = false;

    renderer.setShader(&shader);
    renderer.camera = &camera;

    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);
    reshape(w, h);
}

void LobyScene::exit() {
    if(scene_manager != nullptr) {
        scene_manager->popScene();
    }
    ::exit(EXIT_SUCCESS);
}

void LobyScene::update() {

}

// --------------------------------------------------------------------------------------------- //

void LobyScene::drawScene() {
    renderer.pushObject(background);
    renderer.pushObject(title);
    renderer.render();
}

void LobyScene::keyboardEvent(unsigned char key) {
    switch(key) {
    case 27:
        exit();
        break;
    case ' ':
        if(scene_manager != nullptr) {
            GameScene* game_scene = new GameScene(game);
            game_scene->scene_manager = scene_manager;
            scene_manager->pushScene(game_scene);
        }
        break;
    }
}

void LobyScene::mouseClickEvent(int button, int state, int x, int y) {

}
