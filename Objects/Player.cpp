#include "Player.h"

using namespace std;


Player::Player(const string& id) : Entity { id } {
    move_direction = { 0, 0 };
    move_speed = 4.0f;
    initAnimation();
    initComponent();
    initModel();
}

Player::~Player() {

}


void Player::update(float dt) {
    Entity::update(dt);

    physics->update(dt);

    Vector2 d = move_direction;
    if(d.x != 0) {      // 옆으로 이동시
        body->transform.lookAt(body->transform.position + Vector3 { -d.x, 0, 1 });
    }
    else {
        body->transform.lookAt(body->transform.position + Vector3 { 0, 0, 1 });
    }

    animation(dt);
}

void Player::jump() {
    if(feet->is_colliding) {
        physics->force.y += 5;
    }
}


void Player::initComponent() {
    addComponent<Physics>();
    physics = getComponent<Physics>();
    addComponent<AABB, Vector3, Vector3>("hitbox", { 0.5f, 1.6f, 0.5f }, { 0, 1, 0 });
    addComponent<AABB, Vector3, Vector3>("foot", { 0.4f, 0.1f, 0.4f }, { 0, 0.05f, 0 });
    feet = getComponent<AABB>("foot");
    hitbox = getComponent<AABB>("hitbox");
}

void Player::initModel() {
    // 키 = 32픽셀
    // 머리 8*8*8
    // 몸통 8*12*4
    // 팔 4*12*4
    // 다리 4*12*4

    // 키 1.8m
    float size = 1.8f/32;
    ColorRGB skin_color { 255, 227, 184 };

    head = new Box { "head", { 0, -4*size, 0 }, 8*size, 8*size, 8*size };
    head->transform.position = Vector3 { 0, 24, 0 } * size;
    head->material = material;
    head->material.base_color = skin_color;
    addChild(head);

    Object* nose = new Box { "nose", Vector3 { 0, -0.5f*size, -0.5f } *size, 2*size, 1*size, 1*size };
    nose->transform.position = Vector3 { 0, 3, 4 } * size;
    nose->material = material;
    nose->material.base_color = skin_color | ColorRGB { RGB_Black, 0.2f };
    head->addChild(nose);

    Object* right_eye = new Box { "right_eye", Vector3 { 0, -0.5f, -0.5f } *size, 2*size, 1*size, 1*size };
    right_eye->transform.position = Vector3 { -2, 4, 3 } * size;
    right_eye->transform.scale.z = 1.01f;
    right_eye->material = material;
    right_eye->material.base_color = RGB_Black;
    head->addChild(right_eye);

    Object* left_eye = new Box { "left_eye", Vector3 { 0, -0.5f, -0.5f } *size, 2*size, 1*size, 1*size };
    left_eye->transform.position = Vector3 { 2, 4, 3 } * size;
    left_eye->transform.scale.z = 1.01f;
    left_eye->material = material;
    left_eye->material.base_color = RGB_Black;
    head->addChild(left_eye);

    body = new Box { "body", { 0, -6, 0 }, 8, 12, 4 };
    body->transform.position = Vector3 { 0, 12, 0 } * size;
    body->transform.scale *= size;
    body->material = material;
    body->material.base_color = RGB_Yellow;
    addChild(body);

    left_arm = new Box { "left_arm", { 0, 4, 0 }, 4, 12, 4 };
    left_arm->transform.position = { 6, 10, 0 };
    left_arm->material = material;
    left_arm->material.base_color = skin_color;
    left_arm->setParent(body);

    right_arm = new Box { "right_arm", { 0, 4, 0 }, 4, 12, 4 };
    right_arm->transform.position = { -6, 10, 0 };
    right_arm->material = material;
    right_arm->material.base_color = skin_color;
    right_arm->setParent(body);

    left_leg = new Box { "left_leg", { 0, 6, 0 }, 4, 12, 4 };
    left_leg->transform.position = { 2, 0, 0 };
    left_leg->material = material;
    left_leg->material.base_color = RGB_Blue | ColorRGB { RGB_Black, 0.4f };
    left_leg->setParent(body);

    right_leg = new Box { "right_leg", { 0, 6, 0 }, 4, 12, 4 };
    right_leg->transform.position = { -2, 0, 0 };
    right_leg->material = material;
    right_leg->material.base_color = RGB_Blue | ColorRGB { RGB_Black, 0.4f };
    right_leg->setParent(body);
}

void Player::initAnimation() {
    player_arm_rotate_angle = 0;
    player_leg_rotate_angle = 0;
    player_arm_rotate_direction = 1;
}

void Player::animation(float dt) {
    if(dt < 0) {
        return;
    }

    float anim_speed = 1.6f;    // cycle per second
    float max_angle = 50.0f;
    float angle_per_sec = max_angle * anim_speed;
    float angle = angle_per_sec * move_speed * player_arm_rotate_direction * dt;
    float radian = radians(angle);

    if(move_direction.x != 0 || move_direction.y != 0) {
        player_arm_rotate_angle += angle;
        if(player_arm_rotate_angle >= max_angle) {
            player_arm_rotate_angle = max_angle;
            player_arm_rotate_direction = -1;
        }
        else if(player_arm_rotate_angle <= -max_angle) {
            player_arm_rotate_angle = -max_angle;
            player_arm_rotate_direction = 1;
        }
        right_arm->transform.rotateX(radian);
        left_arm->transform.rotateX(-radian);
        right_leg->transform.rotateX(-radian);
        left_leg->transform.rotateX(radian);
    }
    else {
        // 원래각도로 돌아가는 애니메이션
        // player_arm_rotate_angle
        if(abs(player_arm_rotate_angle) > move_speed) {
            if(player_arm_rotate_angle > 0) {
                player_arm_rotate_direction = -1;
            }
            else {
                player_arm_rotate_direction = 1;
            }
            player_arm_rotate_angle += angle;
            right_arm->transform.rotateX(radian);
            left_arm->transform.rotateX(-radian);
            right_leg->transform.rotateX(-radian);
            left_leg->transform.rotateX(radian);
        }
        else {
            right_arm->transform.rotation = { 1, 0, 0, 0 };
            left_arm->transform.rotation = { 1, 0, 0, 0 };
            right_leg->transform.rotation = { 1, 0, 0, 0 };
            left_leg->transform.rotation = { 1, 0, 0, 0 };
        }
    }
}
