#include "Game.h"

#include "../Game/Component/Light.h"

#include <gl/glew.h>
#include <gl/freeglut.h>

using namespace std;
using namespace chrono;


Game::Game(const std::string& title) : title { title }, 
    timer_value { 0 },
    dt { 0.0f },
    sensitivity { 0.1f },
    fix_cursor_when_motion { false },
    fix_cursor_when_drag { false } {
    initialize();
    setup();
}

Game::~Game() {
    objects.deleteAll();
    glutLeaveMainLoop();
}


void Game::initialize() {
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(title.c_str());

    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK) {
        Log::errorLog("Unable to initialize GLEW");
        exit(EXIT_FAILURE);
    }
    else {
        Log::log("GLEW Initialized");
    }
}

void Game::setup() {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
}


void Game::init(int argc, char** argv) {
    glutInit(&argc, argv);
}

void Game::run() {
    prev_update_time = system_clock::now();
    glutMainLoop();
}


// Events ----------------------------------------------------------------------------------------------- //

void Game::drawScene() {
    for(auto object : objects.objects) {
        if(object.second->getComponent<Light>() != nullptr) {
            renderer.pushLightObject(object.second);
		}
        else {
            renderer.pushObject(object.second);
        }
    }
    renderer.render();
}

void Game::reshape(int w, int h) {
    glViewport(0, 0, w, h);
    camera.aspect = (float)w / h;
}

void Game::timer(int value) {
    auto now = system_clock::now();
    dt = duration_cast<milliseconds>(now - prev_update_time).count() / 1000.0f;
    prev_update_time = now;
    update();

    glutPostRedisplay();
}

void Game::keyboardEvent(unsigned char key) {
    switch(key) {
    case 27:
        exit(EXIT_SUCCESS);
        break;
    }
}

void Game::keyboardUpEvent(unsigned char key) {
    
}

void Game::specialKeyEvent(int key) {
    switch(key) {
    case GLUT_KEY_F3:
        Log::print_log = !Log::print_log;
        break;
    }
}

void Game::mouseClickEvent(int button, int state, int x, int y) {

}

void Game::mouseMotionEvent(const Vector2& delta) {

}

void Game::mouseDragEvent(const Vector2& delta) {

}

// ----------------------------------------------------------------------------------------------------- //

void Game::update() {
    objects.update(dt);
}
