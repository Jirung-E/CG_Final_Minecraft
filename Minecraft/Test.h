#pragma once

#include "../Core/Game.h"
#include "../Core/Shader.h"
#include "../Core/EventsHandler.h"
#include "../Objects/Player.h"
#include "../Objects/Block.h"


class Test : public Game {   // singleton
private:
    EventsHandler& events_handler;
    Shader shader;

    enum ViewMode {
        FirstPerson,
        SecondPerson,
        ThirdPerson,
        COUNT
    } view_mode;

    const float camera_distance;
    Vector3 camera_direction;
    float vertical_sensitivity;

    Player* player;
    std::vector<Block*> blocks;

    bool space_pressed;

    const float interaction_distance;

public:
    Test();

private:
    void initWorld();
    void initObjects();
    void generatePlayerObject();
    void generateBlock(int x, int y, int z, const Material& material);

    void rotateHead(float dx, float dy);

    void update();

    // --------------------------------------------------------------------------------------------- //

private:
    virtual void keyboardEvent(unsigned char key) override;
    virtual void keyboardUpEvent(unsigned char key) override;
    virtual void mouseClickEvent(int button, int state, int x, int y) override;
    virtual void mouseMotionEvent(const Vector2& delta) override;
    virtual void mouseDragEvent(const Vector2& delta) override;
};