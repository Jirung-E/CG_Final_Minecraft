#pragma once

#include "ChunkBasedObjectManager.h"
#include "../Core/Game.h"
#include "../Core/Shader.h"
#include "../Core/EventsHandler.h"
#include "../Objects/Player.h"
#include "../Objects/Block.h"


class Test : public Game {   // singleton
private:
    EventsHandler& events_handler;
    Shader shader;
    ChunkBasedObjectManager objects_manager;

    enum ViewMode {
        FirstPerson,
        ThirdPerson,
        SecondPerson,
        COUNT
    } view_mode;

    const float camera_distance;
    Vector3 camera_direction;
    float vertical_sensitivity;

    Player* player;

    bool space_pressed;

    const float interaction_distance;
    const int simulation_distance;

    enum Face {
        Left, Right, Top, Bottom, Front, Back
    } focus_face;     // 만약 각 면에 정확히 동시에 닿는다면 x, y, z 순으로
    Block* focus_block;
    Entity* focus_entity;

public:
    Test();

private:
    void initWorld();
    void initObjects();
    void generatePlayerObject();
    void generateBlock(int x, int y, int z, const Material& material);

    void rotateHead(float dx, float dy);

    virtual void update() override;

    // --------------------------------------------------------------------------------------------- //

private:
    virtual void keyboardEvent(unsigned char key) override;
    virtual void keyboardUpEvent(unsigned char key) override;
    virtual void mouseClickEvent(int button, int state, int x, int y) override;
    virtual void mouseMotionEvent(const Vector2& delta) override;
    virtual void mouseDragEvent(const Vector2& delta) override;
};