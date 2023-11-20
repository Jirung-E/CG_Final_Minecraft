#pragma once

#include "Game.h"


class EventsHandler {       // Singleton
private:
    Game* game;

    Vector2* mouse_click_point;
    Vector2* mouse_prev_point;

public:
    Vector2 mouse_delta;
    int timer_value;

private:
    EventsHandler(Game* game);
    EventsHandler(const EventsHandler&) = delete;
    EventsHandler& operator=(const EventsHandler&) = delete;

public:
    static EventsHandler& getInstance(Game* game);
    void link();

public:
    static void drawScene();
    static void reshape(int w, int h);
    static void timer(int value);
    static void keyboardEvent(unsigned char key, int x, int y);
    static void keyboardUpEvent(unsigned char key, int x, int y);
    static void specialKeyEvent(int key, int x, int y);
    static void mouseClickEvent(int button, int state, int x, int y);
    static void mouseMotionEvent(int x, int y);
    static void mouseDragEvent(int x, int y);
};
