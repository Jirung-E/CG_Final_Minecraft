#include "LobyScene.h"

#include "GameScene.h"

using namespace std;


LobyScene::LobyScene(Game* game) : Scene { game },
    shader { "ShaderSource/vertex.glsl", "ShaderSource/fragment.glsl" } {

}

void LobyScene::start() {

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
