#pragma once

#include "Scene.h"
#include "SceneManager.h"
#include "../../Core/EventsHandler.h"
#include "../../Core/Shader.h"
#include "../../Core/Renderer.h"
#include "../../Game/Camera.h"


class LobyScene : public Scene {
public:
    SceneManager* scene_manager;

private:
    Shader shader;
    Renderer renderer;
    Camera camera;
    unsigned int background_texture;
    unsigned int title_texture;
    Object* background;
    Object* title;

public:
    LobyScene(Game* game);
    virtual ~LobyScene();

public:
    virtual void start() override;
    virtual void exit() override;
    virtual void update() override;

    // --------------------------------------------------------------------------------------------- //

private:
    virtual void drawScene() override;
    virtual void keyboardEvent(unsigned char key) override;
    virtual void mouseClickEvent(int button, int state, int x, int y) override;
};
