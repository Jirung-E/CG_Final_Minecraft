#include "Player.h"

#include "../Game/Texture.h"

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

    // texture 64*64
    unsigned int texture_id = Texture::get("Resource/Textures/steve.png").getID();
    float size_4 = 4.0f/64;
    float size_8 = 8.0f/64;
    float size_12 = 12.0f/64;

    head = new Box { "head", { 0, 0, 0 }, 8*size, 8*size, 8*size };
    head->model->texture_coords.clear();
    head->model->texture_coords.push_back({ size_8, 1.0f-size_8*2 });
    head->model->texture_coords.push_back({ size_8*2.0f, 1.0f-size_8*2 });
    head->model->texture_coords.push_back({ size_8, 1.0f-size_8 });
    head->model->texture_coords.push_back({ size_8*2.0f, 1.0f-size_8 });
    head->model->texture_coords.push_back({ size_8*3.0f, 1.0f-size_8*2 });
    head->model->texture_coords.push_back({ size_8*4.0f, 1.0f-size_8*2 });
    head->model->texture_coords.push_back({ size_8*3.0f, 1.0f-size_8 });
    head->model->texture_coords.push_back({ size_8*4.0f, 1.0f-size_8 });
    // front
    head->model->polygons[4].v1.texture_coord = 0;
    head->model->polygons[4].v2.texture_coord = 1;
    head->model->polygons[4].v3.texture_coord = 3;
    head->model->polygons[5].v1.texture_coord = 3;
    head->model->polygons[5].v2.texture_coord = 2;
    head->model->polygons[5].v3.texture_coord = 0;
    // back
    head->model->polygons[8].v1.texture_coord = 4;
    head->model->polygons[8].v2.texture_coord = 5;
    head->model->polygons[8].v3.texture_coord = 7;
    head->model->polygons[9].v1.texture_coord = 7;
    head->model->polygons[9].v2.texture_coord = 6;
    head->model->polygons[9].v3.texture_coord = 4;

    head->model->texture_id.push_back(texture_id);
    head->transform.position = Vector3 { 0, 24, 0 } * size;
    head->material = material;
    addChild(head);

    body = new Box { "body", { 0, 0, 0 }, 8, 12, 4 };
    body->model->texture_coords.clear();
    // front
    body->model->texture_coords.push_back({ size_4*5, 1.0f-size_8*4 });
    body->model->texture_coords.push_back({ size_4*7, 1.0f-size_8*4 });
    body->model->texture_coords.push_back({ size_4*5, 1.0f-size_4*5 });
    body->model->texture_coords.push_back({ size_4*7, 1.0f-size_4*5 });
    // back
    body->model->texture_coords.push_back({ size_4*8, 1.0f-size_8*4 });
    body->model->texture_coords.push_back({ size_4*10, 1.0f-size_8*4 });
    body->model->texture_coords.push_back({ size_4*8, 1.0f-size_4*5 });
    body->model->texture_coords.push_back({ size_4*10, 1.0f-size_4*5 });
    // top
    body->model->texture_coords.push_back({ size_4*5, 1.0f-size_4*5 });
    body->model->texture_coords.push_back({ size_4*7, 1.0f-size_4*5 });
    body->model->texture_coords.push_back({ size_4*5, 1.0f-size_4*4 });
    body->model->texture_coords.push_back({ size_4*7, 1.0f-size_4*4 });
    // bottom
    body->model->texture_coords.push_back({ size_4*7, 1.0f-size_4*5 });
    body->model->texture_coords.push_back({ size_4*9, 1.0f-size_4*5 });
    body->model->texture_coords.push_back({ size_4*7, 1.0f-size_4*4 });
    body->model->texture_coords.push_back({ size_4*9, 1.0f-size_4*4 });
    // left
    body->model->texture_coords.push_back({ size_4*7, 1.0f-size_8*4 });
    body->model->texture_coords.push_back({ size_4*8, 1.0f-size_8*4 });
    body->model->texture_coords.push_back({ size_4*7, 1.0f-size_4*5 });
    body->model->texture_coords.push_back({ size_4*8, 1.0f-size_4*5 });
    // right
    body->model->texture_coords.push_back({ size_4*4, 1.0f-size_8*4 });
    body->model->texture_coords.push_back({ size_4*5, 1.0f-size_8*4 });
    body->model->texture_coords.push_back({ size_4*4, 1.0f-size_4*5 });
    body->model->texture_coords.push_back({ size_4*5, 1.0f-size_4*5 });

    // front
    body->model->polygons[4].v1.texture_coord = 0;
    body->model->polygons[4].v2.texture_coord = 1;
    body->model->polygons[4].v3.texture_coord = 3;
    body->model->polygons[5].v1.texture_coord = 3;
    body->model->polygons[5].v2.texture_coord = 2;
    body->model->polygons[5].v3.texture_coord = 0;
    // back
    body->model->polygons[8].v1.texture_coord = 4;
    body->model->polygons[8].v2.texture_coord = 5;
    body->model->polygons[8].v3.texture_coord = 7;
    body->model->polygons[9].v1.texture_coord = 7;
    body->model->polygons[9].v2.texture_coord = 6;
    body->model->polygons[9].v3.texture_coord = 4;
    // top
    body->model->polygons[2].v1.texture_coord = 8;
    body->model->polygons[2].v2.texture_coord = 9;
    body->model->polygons[2].v3.texture_coord = 11;
    body->model->polygons[3].v1.texture_coord = 11;
    body->model->polygons[3].v2.texture_coord = 10;
    body->model->polygons[3].v3.texture_coord = 8;
    // bottom
    body->model->polygons[0].v1.texture_coord = 12;
    body->model->polygons[0].v2.texture_coord = 13;
    body->model->polygons[0].v3.texture_coord = 15;
    body->model->polygons[1].v1.texture_coord = 15;
    body->model->polygons[1].v2.texture_coord = 14;
    body->model->polygons[1].v3.texture_coord = 12;
    // left
    body->model->polygons[6].v1.texture_coord = 16;
    body->model->polygons[6].v2.texture_coord = 17;
    body->model->polygons[6].v3.texture_coord = 19;
    body->model->polygons[7].v1.texture_coord = 19;
    body->model->polygons[7].v2.texture_coord = 18;
    body->model->polygons[7].v3.texture_coord = 16;
    // right
    body->model->polygons[10].v1.texture_coord = 20;
    body->model->polygons[10].v2.texture_coord = 21;
    body->model->polygons[10].v3.texture_coord = 23;
    body->model->polygons[11].v1.texture_coord = 23;
    body->model->polygons[11].v2.texture_coord = 22;
    body->model->polygons[11].v3.texture_coord = 20;

    body->model->texture_id.push_back(texture_id);
    body->transform.position = Vector3 { 0, 12, 0 } * size;
    body->transform.scale *= size;
    body->material = material;
    addChild(body);

    left_arm = new Box { "left_arm", { 0, 10, 0 }, 4, 12, 4 };
    left_arm->model->texture_coords.clear();
    // front
    left_arm->model->texture_coords.push_back({ size_4*11, 1.0f-size_8*4 });
    left_arm->model->texture_coords.push_back({ size_4*12, 1.0f-size_8*4 });
    left_arm->model->texture_coords.push_back({ size_4*11, 1.0f-size_4*5 });
    left_arm->model->texture_coords.push_back({ size_4*12, 1.0f-size_4*5 });
    // back
    left_arm->model->texture_coords.push_back({ size_4*13, 1.0f-size_8*4 });
    left_arm->model->texture_coords.push_back({ size_4*14, 1.0f-size_8*4 });
    left_arm->model->texture_coords.push_back({ size_4*13, 1.0f-size_4*5 });
    left_arm->model->texture_coords.push_back({ size_4*14, 1.0f-size_4*5 });
    // top
    left_arm->model->texture_coords.push_back({ size_4*11, 1.0f-size_4*5 });
    left_arm->model->texture_coords.push_back({ size_4*12, 1.0f-size_4*5 });
    left_arm->model->texture_coords.push_back({ size_4*11, 1.0f-size_4*4 });
    left_arm->model->texture_coords.push_back({ size_4*12, 1.0f-size_4*4 });
    // bottom
    left_arm->model->texture_coords.push_back({ size_4*12, 1.0f-size_4*5 });
    left_arm->model->texture_coords.push_back({ size_4*13, 1.0f-size_4*5 });
    left_arm->model->texture_coords.push_back({ size_4*12, 1.0f-size_4*4 });
    left_arm->model->texture_coords.push_back({ size_4*13, 1.0f-size_4*4 });
    // left
    left_arm->model->texture_coords.push_back({ size_4*12, 1.0f-size_8*4 });
    left_arm->model->texture_coords.push_back({ size_4*13, 1.0f-size_8*4 });
    left_arm->model->texture_coords.push_back({ size_4*12, 1.0f-size_4*5 });
    left_arm->model->texture_coords.push_back({ size_4*13, 1.0f-size_4*5 });
    // right
    left_arm->model->texture_coords.push_back({ size_4*10, 1.0f-size_8*4 });
    left_arm->model->texture_coords.push_back({ size_4*11, 1.0f-size_8*4 });
    left_arm->model->texture_coords.push_back({ size_4*10, 1.0f-size_4*5 });
    left_arm->model->texture_coords.push_back({ size_4*11, 1.0f-size_4*5 });

    // front
    left_arm->model->polygons[4].v1.texture_coord = 0;
    left_arm->model->polygons[4].v2.texture_coord = 1;
    left_arm->model->polygons[4].v3.texture_coord = 3;
    left_arm->model->polygons[5].v1.texture_coord = 3;
    left_arm->model->polygons[5].v2.texture_coord = 2;
    left_arm->model->polygons[5].v3.texture_coord = 0;
    // back
    left_arm->model->polygons[8].v1.texture_coord = 4;
    left_arm->model->polygons[8].v2.texture_coord = 5;
    left_arm->model->polygons[8].v3.texture_coord = 7;
    left_arm->model->polygons[9].v1.texture_coord = 7;
    left_arm->model->polygons[9].v2.texture_coord = 6;
    left_arm->model->polygons[9].v3.texture_coord = 4;
    // top
    left_arm->model->polygons[2].v1.texture_coord = 8;
    left_arm->model->polygons[2].v2.texture_coord = 9;
    left_arm->model->polygons[2].v3.texture_coord = 11;
    left_arm->model->polygons[3].v1.texture_coord = 11;
    left_arm->model->polygons[3].v2.texture_coord = 10;
    left_arm->model->polygons[3].v3.texture_coord = 8;
    // bottom
    left_arm->model->polygons[0].v1.texture_coord = 12;
    left_arm->model->polygons[0].v2.texture_coord = 13;
    left_arm->model->polygons[0].v3.texture_coord = 15;
    left_arm->model->polygons[1].v1.texture_coord = 15;
    left_arm->model->polygons[1].v2.texture_coord = 14;
    left_arm->model->polygons[1].v3.texture_coord = 12;
    // left
    left_arm->model->polygons[6].v1.texture_coord = 16;
    left_arm->model->polygons[6].v2.texture_coord = 17;
    left_arm->model->polygons[6].v3.texture_coord = 19;
    left_arm->model->polygons[7].v1.texture_coord = 19;
    left_arm->model->polygons[7].v2.texture_coord = 18;
    left_arm->model->polygons[7].v3.texture_coord = 16;
    // right
    left_arm->model->polygons[10].v1.texture_coord = 20;
    left_arm->model->polygons[10].v2.texture_coord = 21;
    left_arm->model->polygons[10].v3.texture_coord = 23;
    left_arm->model->polygons[11].v1.texture_coord = 23;
    left_arm->model->polygons[11].v2.texture_coord = 22;
    left_arm->model->polygons[11].v3.texture_coord = 20;

    left_arm->model->texture_id.push_back(texture_id);
    left_arm->transform.position = { 6, 10, 0 };
    left_arm->material = material;
    left_arm->setParent(body);

    right_arm = new Box { "right_arm", { 0, 10, 0 }, 4, 12, 4 };
    right_arm->model->texture_coords.clear();
    // front
    right_arm->model->texture_coords.push_back({ size_4*9, 0 });
    right_arm->model->texture_coords.push_back({ size_4*10, 0 });
    right_arm->model->texture_coords.push_back({ size_4*9, size_12 });
    right_arm->model->texture_coords.push_back({ size_4*10,size_12 });
    // back
    right_arm->model->texture_coords.push_back({ size_4*11, 0 });
    right_arm->model->texture_coords.push_back({ size_4*12, 0 });
    right_arm->model->texture_coords.push_back({ size_4*11, size_12 });
    right_arm->model->texture_coords.push_back({ size_4*12, size_12 });
    // top
    right_arm->model->texture_coords.push_back({ size_4*9, size_12 });
    right_arm->model->texture_coords.push_back({ size_4*10, size_12 });
    right_arm->model->texture_coords.push_back({ size_4*9, size_12+size_4 });
    right_arm->model->texture_coords.push_back({ size_4*10, size_12+size_4 });
    // bottom
    right_arm->model->texture_coords.push_back({ size_4*10, size_12 });
    right_arm->model->texture_coords.push_back({ size_4*11, size_12 });
    right_arm->model->texture_coords.push_back({ size_4*10, size_12+size_4 });
    right_arm->model->texture_coords.push_back({ size_4*11, size_12+size_4 });
    // left
    right_arm->model->texture_coords.push_back({ size_4*10, 0 });
    right_arm->model->texture_coords.push_back({ size_4*11, 0 });
    right_arm->model->texture_coords.push_back({ size_4*10, size_12 });
    right_arm->model->texture_coords.push_back({ size_4*11, size_12 });
    // right
    right_arm->model->texture_coords.push_back({ size_4*8, 0 });
    right_arm->model->texture_coords.push_back({ size_4*9, 0 });
    right_arm->model->texture_coords.push_back({ size_4*8, size_12 });
    right_arm->model->texture_coords.push_back({ size_4*9, size_12 });

    // front
    right_arm->model->polygons[4].v1.texture_coord = 0;
    right_arm->model->polygons[4].v2.texture_coord = 1;
    right_arm->model->polygons[4].v3.texture_coord = 3;
    right_arm->model->polygons[5].v1.texture_coord = 3;
    right_arm->model->polygons[5].v2.texture_coord = 2;
    right_arm->model->polygons[5].v3.texture_coord = 0;
    // back
    right_arm->model->polygons[8].v1.texture_coord = 4;
    right_arm->model->polygons[8].v2.texture_coord = 5;
    right_arm->model->polygons[8].v3.texture_coord = 7;
    right_arm->model->polygons[9].v1.texture_coord = 7;
    right_arm->model->polygons[9].v2.texture_coord = 6;
    right_arm->model->polygons[9].v3.texture_coord = 4;
    // top
    right_arm->model->polygons[2].v1.texture_coord = 8;
    right_arm->model->polygons[2].v2.texture_coord = 9;
    right_arm->model->polygons[2].v3.texture_coord = 11;
    right_arm->model->polygons[3].v1.texture_coord = 11;
    right_arm->model->polygons[3].v2.texture_coord = 10;
    right_arm->model->polygons[3].v3.texture_coord = 8;
    // bottom
    right_arm->model->polygons[0].v1.texture_coord = 12;
    right_arm->model->polygons[0].v2.texture_coord = 13;
    right_arm->model->polygons[0].v3.texture_coord = 15;
    right_arm->model->polygons[1].v1.texture_coord = 15;
    right_arm->model->polygons[1].v2.texture_coord = 14;
    right_arm->model->polygons[1].v3.texture_coord = 12;
    // left
    right_arm->model->polygons[6].v1.texture_coord = 16;
    right_arm->model->polygons[6].v2.texture_coord = 17;
    right_arm->model->polygons[6].v3.texture_coord = 19;
    right_arm->model->polygons[7].v1.texture_coord = 19;
    right_arm->model->polygons[7].v2.texture_coord = 18;
    right_arm->model->polygons[7].v3.texture_coord = 16;
    // right
    right_arm->model->polygons[10].v1.texture_coord = 20;
    right_arm->model->polygons[10].v2.texture_coord = 21;
    right_arm->model->polygons[10].v3.texture_coord = 23;
    right_arm->model->polygons[11].v1.texture_coord = 23;
    right_arm->model->polygons[11].v2.texture_coord = 22;
    right_arm->model->polygons[11].v3.texture_coord = 20;

    right_arm->model->texture_id.push_back(texture_id);
    right_arm->transform.position = { -6, 10, 0 };
    right_arm->material = material;
    right_arm->setParent(body);

    left_leg = new Box { "left_leg", { 0, 12, 0 }, 4, 12, 4 };
    left_leg->model->texture_id.push_back(texture_id);
    left_leg->transform.position = { 2, 0, 0 };
    left_leg->material = material;
    left_leg->setParent(body);

    right_leg = new Box { "right_leg", { 0, 12, 0 }, 4, 12, 4 };
    right_leg->model->texture_id.push_back(texture_id);
    right_leg->transform.position = { -2, 0, 0 };
    right_leg->material = material;
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
