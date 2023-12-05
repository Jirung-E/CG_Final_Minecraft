#pragma once

#include "../Game/Entity.h"
#include "../Game/Component/Physics.h"
#include "../Game/Component/Collider.h"
#include "Box.h"


class Player : public Entity {
public:
    Box* head;
    Box* body;
    Box* left_arm;
    Box* right_arm;
    Box* left_leg;
    Box* right_leg;

    Physics* physics;
    AABB* feet;

    float player_arm_rotate_angle;
    float player_leg_rotate_angle;
    int player_arm_rotate_direction;

public:
    Player(const std::string& id);
    ~Player();

public:
    virtual void update(float dt) override;
    void jump();

private:
    void initComponent();
    void initModel();
    void initUV(Object* part, const float& start_x, const float& start_y, const float& width, const float& height, const float& depth);
    void initTextureCoord(Object* part);
    void initAnimation();
    void animation(float dt);
};
