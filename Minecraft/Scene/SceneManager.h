#pragma once

#include "Scene.h"

#include <stack>


class SceneManager {
private:
    std::stack<Scene*> scenes;

public:
    ~SceneManager();

public:
    void pushScene(Scene* scene);
    void popScene();
    Scene* topScene() const;
};
