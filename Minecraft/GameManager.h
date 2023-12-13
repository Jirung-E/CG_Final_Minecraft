#pragma once

#include "../Core/Game.h"
#include "../Core/EventsHandler.h"
#include "Scene/SceneManager.h"


class Main : public Game {
private:
    EventsHandler& events_handler;
    SceneManager scenes;

public:
    Main();

public:
    virtual void update() override;

    // --------------------------------------------------------------------------------------------- //

private:
    virtual void drawScene() override;
    virtual void reshape(int w, int h) override;
    virtual void keyboardEvent(unsigned char key) override;
    virtual void keyboardUpEvent(unsigned char key) override;
    virtual void specialKeyEvent(int key);
    virtual void mouseClickEvent(int button, int state, int x, int y) override;
    virtual void mouseMotionEvent(const Vector2& delta) override;
    virtual void mouseDragEvent(const Vector2& delta) override;


public:
    //virtual void run() override;
};