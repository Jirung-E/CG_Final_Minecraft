#include "EventsHandler.h"

#include <gl/freeglut.h>


EventsHandler::EventsHandler() : game { nullptr }, 
    mouse_click_point { nullptr }, 
    mouse_prev_point { nullptr }, 
    mouse_delta { 0, 0 }, timer_value { 1 } {

}


EventsHandler& EventsHandler::getInstance() {
    static EventsHandler instance { };
    return instance;
}


void EventsHandler::link(Game* game) {
    this->game = game;
    glutDisplayFunc(drawScene);
    glutReshapeFunc(reshape);
    glutTimerFunc(timer_value, timer, timer_value);
    glutKeyboardFunc(keyboardEvent);
    glutKeyboardUpFunc(keyboardUpEvent);
    glutSpecialFunc(specialKeyEvent);
    glutMouseFunc(mouseClickEvent);
    glutPassiveMotionFunc(mouseMotionEvent);
    glutMotionFunc(mouseDragEvent);
}


void EventsHandler::drawScene() {
    EventsHandler& handler = getInstance();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    handler.game->drawScene();

    glutSwapBuffers();
}

void EventsHandler::reshape(int w, int h) {
    EventsHandler& handler = getInstance();

    handler.game->reshape(w, h);

    glViewport(0, 0, w, h);
}

void EventsHandler::timer(int value) {
    EventsHandler& handler = getInstance();

    handler.game->timer(value);

    glutTimerFunc(handler.timer_value, timer, handler.timer_value);
}

void EventsHandler::keyboardEvent(unsigned char key, int x, int y) {
    EventsHandler& handler = getInstance();

    handler.game->keyboardEvent(key);
}

void EventsHandler::keyboardUpEvent(unsigned char key, int x, int y) {
    EventsHandler& handler = getInstance();

    handler.game->keyboardUpEvent(key);
}

void EventsHandler::specialKeyEvent(int key, int x, int y) {
    EventsHandler& handler = getInstance();

    handler.game->specialKeyEvent(key);
}

void EventsHandler::mouseClickEvent(int button, int state, int x, int y) {
    EventsHandler& handler = getInstance();

    switch(state) {
    case GLUT_DOWN:
        handler.mouse_click_point = new Vector2(x, y);
        break;
    case GLUT_UP:
        delete handler.mouse_click_point;
        handler.mouse_click_point = nullptr;
        break;
    }

    handler.game->mouseClickEvent(button, state, x, y);
}

void EventsHandler::mouseMotionEvent(int x, int y) {
    EventsHandler& handler = getInstance();

    if(handler.mouse_prev_point != nullptr) {
        handler.mouse_delta = Vector2(x, y) - *handler.mouse_prev_point;
        handler.game->mouseMotionEvent(handler.mouse_delta);
    }

    delete handler.mouse_prev_point;
    if(handler.game->fix_cursor_when_motion) {
        int w = glutGet(GLUT_WINDOW_WIDTH);
        int h = glutGet(GLUT_WINDOW_HEIGHT);
        glutWarpPointer(w/2, h/2);
        handler.mouse_prev_point = new Vector2 { w/2, h/2 };
    }
    else {
        handler.mouse_prev_point = new Vector2 { x, y };
    }
}

void EventsHandler::mouseDragEvent(int x, int y) {
    EventsHandler& handler = getInstance();

    if(handler.mouse_click_point != nullptr) {
        handler.mouse_delta = Vector2(x, y) - *handler.mouse_click_point;
        handler.game->mouseDragEvent(handler.mouse_delta);
    }

    delete handler.mouse_prev_point;
    if(handler.game->fix_cursor_when_drag) {
        int w = glutGet(GLUT_WINDOW_WIDTH);
        int h = glutGet(GLUT_WINDOW_HEIGHT);
        glutWarpPointer(w/2, h/2);
        handler.mouse_prev_point = new Vector2 { w/2, h/2 };
    }
    else {
        handler.mouse_prev_point = new Vector2 { x, y };
    }
}
