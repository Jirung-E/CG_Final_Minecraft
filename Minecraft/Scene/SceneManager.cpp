#include "SceneManager.h"

using namespace std;


SceneManager::~SceneManager() {
    while(!scenes.empty()) {
        popScene();
    }
}

void SceneManager::pushScene(Scene* scene) {
    scenes.push(scene);
    scene->start();
}

void SceneManager::popScene() {
    delete scenes.top();
    scenes.pop();
}

Scene* SceneManager::topScene() const {
    return scenes.top();
}
