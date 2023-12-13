#pragma once

#include "../../Math/Math.h"

#include "../../Core/Game.h"


class Scene {
public:
    Game* game;

public:
    Scene(Game* game);
    ~Scene();

public:
    virtual void start() abstract;
    virtual void update() abstract;
    virtual void exit() abstract;

    // --------------------------------------------------------------------------------------------- //
public:
    virtual void drawScene();
    virtual void reshape(int w, int h);
    virtual void timer(int value);
    virtual void keyboardEvent(unsigned char key);
    virtual void keyboardUpEvent(unsigned char key);
    virtual void specialKeyEvent(int key);
    virtual void mouseClickEvent(int button, int state, int x, int y);
    virtual void mouseMotionEvent(const Vector2& delta);
    virtual void mouseDragEvent(const Vector2& delta);
};
