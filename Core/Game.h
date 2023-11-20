#pragma once

#include "ObjectManager.h"
#include "Renderer.h"
#include "../Game/Camera.h"

#include <chrono>


class Game abstract {
public:
    const std::string title;

private:
    int timer_value;
    std::chrono::system_clock::time_point prev_update_time;

protected:
    Renderer renderer;
    ObjectManager objects;

    Camera camera;

    float sensitivity;
    bool fix_cursor_when_motion;
    bool fix_cursor_when_drag;

    float dt;

protected:
    Game(const std::string& title);

public:
    virtual ~Game();
    
private:
    void initialize();
    void setup();

public:
    static void init(int argc, char** argv);
    void run();

    virtual void drawScene();
    virtual void reshape(int w, int h);
    virtual void timer(int value);
    virtual void keyboardEvent(unsigned char key);
    virtual void keyboardUpEvent(unsigned char key);
    virtual void specialKeyEvent(int key);
    virtual void mouseClickEvent(int button, int state, int x, int y);
    virtual void mouseMotionEvent(const Vector2& delta);
    virtual void mouseDragEvent(const Vector2& delta);

    // --------------------------------------------------------------------------------------------- //

protected:
    virtual void update();
};
