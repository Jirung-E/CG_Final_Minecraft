#include "Scene.h"

using namespace std;


Scene::Scene(Game* game) : game { game } {

}

Scene::~Scene() {

}


void Scene::drawScene() {
}

void Scene::reshape(int w, int h) {
}

void Scene::timer(int value) {
}

void Scene::keyboardEvent(unsigned char key) {
}

void Scene::keyboardUpEvent(unsigned char key) {
}

void Scene::specialKeyEvent(int key) {
}

void Scene::mouseClickEvent(int button, int state, int x, int y) {
}

void Scene::mouseMotionEvent(const Vector2& delta) {
}

void Scene::mouseDragEvent(const Vector2& delta) {
}
