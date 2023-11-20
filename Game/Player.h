#pragma once

#include "Entity.h"
#include "Box.h"
#include "Component/Physics.h"
#include "Component/Collider.h"


class Player : public Entity {
public:
    Box* head;
    Box* body;
    Box* left_arm;
    Box* right_arm;
    Box* left_leg;
    Box* right_leg;

    Physics* physics;
    AABB* hitbox;
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
    void initAnimation();
    void animation(float dt);
};
