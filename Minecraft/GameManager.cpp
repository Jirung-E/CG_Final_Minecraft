#include "GameManager.h"

#include "Scene/LobyScene.h"
#include "Scene/GameScene.h"


Main::Main() : Game { "Minecraft" },
    events_handler { EventsHandler::getInstance() } {
    events_handler.link(this);
    LobyScene* loby_scene = new LobyScene { this };
    loby_scene->scene_manager = &scenes;
    scenes.pushScene(loby_scene);
}


void Main::update() {
    scenes.topScene()->update();
}

// --------------------------------------------------------------------------------------------- //

void Main::drawScene() {
    scenes.topScene()->drawScene();
}

void Main::reshape(int w, int h) {
    Game::reshape(w, h);
    scenes.topScene()->reshape(w, h);
}

void Main::keyboardEvent(unsigned char key) {
    scenes.topScene()->keyboardEvent(key);
}

void Main::keyboardUpEvent(unsigned char key) {
    scenes.topScene()->keyboardUpEvent(key);
}

void Main::specialKeyEvent(int key) {
    scenes.topScene()->specialKeyEvent(key);
}

void Main::mouseClickEvent(int button, int state, int x, int y) {
    scenes.topScene()->mouseClickEvent(button, state, x, y);
}

void Main::mouseMotionEvent(const Vector2& delta) {
    scenes.topScene()->mouseMotionEvent(delta);
}

void Main::mouseDragEvent(const Vector2& delta) {
    scenes.topScene()->mouseDragEvent(delta);
}


//void Main::run() {
//    Game::run();
//}
