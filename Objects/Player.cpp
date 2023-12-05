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

    initTextureCoord(body);

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

    initTextureCoord(left_arm);

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

    initTextureCoord(right_arm);

    right_arm->model->texture_id.push_back(texture_id);
    right_arm->transform.position = { -6, 10, 0 };
    right_arm->material = material;
    right_arm->setParent(body);

    left_leg = new Box { "left_leg", { 0, 12, 0 }, 4, 12, 4 };
    left_leg->model->texture_coords.clear();
    // front
    left_leg->model->texture_coords.push_back({ size_4*1, 1.0f-size_8*4 });
    left_leg->model->texture_coords.push_back({ size_4*2, 1.0f-size_8*4 });
    left_leg->model->texture_coords.push_back({ size_4*1, 1.0f-size_4*5 });
    left_leg->model->texture_coords.push_back({ size_4*2, 1.0f-size_4*5 });
    // back
    left_leg->model->texture_coords.push_back({ size_4*3, 1.0f-size_8*4 });
    left_leg->model->texture_coords.push_back({ size_4*4, 1.0f-size_8*4 });
    left_leg->model->texture_coords.push_back({ size_4*3, 1.0f-size_4*5 });
    left_leg->model->texture_coords.push_back({ size_4*4, 1.0f-size_4*5 });
    // top
    left_leg->model->texture_coords.push_back({ size_4*1, 1.0f-size_4*5 });
    left_leg->model->texture_coords.push_back({ size_4*2, 1.0f-size_4*5 });
    left_leg->model->texture_coords.push_back({ size_4*1, 1.0f-size_4*4 });
    left_leg->model->texture_coords.push_back({ size_4*2, 1.0f-size_4*4 });
    // bottom
    left_leg->model->texture_coords.push_back({ size_4*2, 1.0f-size_4*5 });
    left_leg->model->texture_coords.push_back({ size_4*3, 1.0f-size_4*5 });
    left_leg->model->texture_coords.push_back({ size_4*2, 1.0f-size_4*4 });
    left_leg->model->texture_coords.push_back({ size_4*3, 1.0f-size_4*4 });
    // left
    left_leg->model->texture_coords.push_back({ size_4*2, 1.0f-size_8*4 });
    left_leg->model->texture_coords.push_back({ size_4*3, 1.0f-size_8*4 });
    left_leg->model->texture_coords.push_back({ size_4*2, 1.0f-size_4*5 });
    left_leg->model->texture_coords.push_back({ size_4*3, 1.0f-size_4*5 });
    // right
    left_leg->model->texture_coords.push_back({ size_4*0, 1.0f-size_8*4 });
    left_leg->model->texture_coords.push_back({ size_4*1, 1.0f-size_8*4 });
    left_leg->model->texture_coords.push_back({ size_4*0, 1.0f-size_4*5 });
    left_leg->model->texture_coords.push_back({ size_4*1, 1.0f-size_4*5 });

    initTextureCoord(left_leg);
    left_leg->model->texture_id.push_back(texture_id);
    left_leg->transform.position = { 2, 0, 0 };
    left_leg->material = material;
    left_leg->setParent(body);

    right_leg = new Box { "right_leg", { 0, 12, 0 }, 4, 12, 4 };
    initUV(right_leg, 0, 1.0f-size_8*4, size_4, size_12, size_4);
    initTextureCoord(right_leg);
    right_leg->model->texture_id.push_back(texture_id);
    right_leg->transform.position = { -2, 0, 0 };
    right_leg->material = material;
    right_leg->setParent(body);
}

void Player::initUV(Object* part, const float& start_x, const float& start_y, const float& width, const float& height, const float& depth) {
    part->model->texture_coords.clear();

    const float right_start_x = start_x;
    const float right_end_x = start_x+depth;
    const float front_start_x = right_end_x;
    const float front_end_x = front_start_x+width;
    const float left_start_x = front_end_x;
    const float left_end_x = left_start_x+depth;
    const float back_start_x = left_end_x;
    const float back_end_x = back_start_x+width;
    const float lfrb_start_y = start_y;
    const float lfrb_end_y = lfrb_start_y+height;
    // front
    part->model->texture_coords.push_back({ front_start_x, start_y });
    part->model->texture_coords.push_back({ front_end_x,   start_y });
    part->model->texture_coords.push_back({ front_start_x, lfrb_end_y });
    part->model->texture_coords.push_back({ front_end_x,   lfrb_end_y });
    // back
    part->model->texture_coords.push_back({ back_start_x, start_y });
    part->model->texture_coords.push_back({ back_end_x,   start_y });
    part->model->texture_coords.push_back({ back_start_x, lfrb_end_y });
    part->model->texture_coords.push_back({ back_end_x,   lfrb_end_y });
    // left
    part->model->texture_coords.push_back({ left_start_x, start_y });
    part->model->texture_coords.push_back({ left_end_x,   start_y });
    part->model->texture_coords.push_back({ left_start_x, lfrb_end_y });
    part->model->texture_coords.push_back({ left_end_x,   lfrb_end_y });
    // right
    part->model->texture_coords.push_back({ right_start_x, start_y });
    part->model->texture_coords.push_back({ right_end_x,   start_y });
    part->model->texture_coords.push_back({ right_start_x, lfrb_end_y });
    part->model->texture_coords.push_back({ right_end_x,   lfrb_end_y });

    const float top_start_x = front_start_x;
    const float top_end_x = front_end_x;
    const float bottom_start_x = top_end_x;
    const float bottom_end_x = bottom_start_x+width;
    const float tb_start_y = lfrb_end_y;
    const float tb_end_y = tb_start_y+depth;
    // top
    part->model->texture_coords.push_back({ top_start_x, tb_start_y });
    part->model->texture_coords.push_back({ top_end_x,   tb_start_y });
    part->model->texture_coords.push_back({ top_start_x, tb_end_y });
    part->model->texture_coords.push_back({ top_end_x,   tb_end_y });
    // bottom
    part->model->texture_coords.push_back({ bottom_start_x, tb_start_y });
    part->model->texture_coords.push_back({ bottom_end_x,   tb_start_y });
    part->model->texture_coords.push_back({ bottom_start_x, tb_end_y });
    part->model->texture_coords.push_back({ bottom_end_x,   tb_end_y });
}

void Player::initTextureCoord(Object* part) {
    // front
    part->model->polygons[4].v1.texture_coord = 0;
    part->model->polygons[4].v2.texture_coord = 1;
    part->model->polygons[4].v3.texture_coord = 3;
    part->model->polygons[5].v1.texture_coord = 3;
    part->model->polygons[5].v2.texture_coord = 2;
    part->model->polygons[5].v3.texture_coord = 0;
    // back
    part->model->polygons[8].v1.texture_coord = 4;
    part->model->polygons[8].v2.texture_coord = 5;
    part->model->polygons[8].v3.texture_coord = 7;
    part->model->polygons[9].v1.texture_coord = 7;
    part->model->polygons[9].v2.texture_coord = 6;
    part->model->polygons[9].v3.texture_coord = 4;
    // top
    part->model->polygons[2].v1.texture_coord = 8;
    part->model->polygons[2].v2.texture_coord = 9;
    part->model->polygons[2].v3.texture_coord = 11;
    part->model->polygons[3].v1.texture_coord = 11;
    part->model->polygons[3].v2.texture_coord = 10;
    part->model->polygons[3].v3.texture_coord = 8;
    // bottom
    part->model->polygons[0].v1.texture_coord = 12;
    part->model->polygons[0].v2.texture_coord = 13;
    part->model->polygons[0].v3.texture_coord = 15;
    part->model->polygons[1].v1.texture_coord = 15;
    part->model->polygons[1].v2.texture_coord = 14;
    part->model->polygons[1].v3.texture_coord = 12;
    // left
    part->model->polygons[6].v1.texture_coord = 16;
    part->model->polygons[6].v2.texture_coord = 17;
    part->model->polygons[6].v3.texture_coord = 19;
    part->model->polygons[7].v1.texture_coord = 19;
    part->model->polygons[7].v2.texture_coord = 18;
    part->model->polygons[7].v3.texture_coord = 16;
    // right
    part->model->polygons[10].v1.texture_coord = 20;
    part->model->polygons[10].v2.texture_coord = 21;
    part->model->polygons[10].v3.texture_coord = 23;
    part->model->polygons[11].v1.texture_coord = 23;
    part->model->polygons[11].v2.texture_coord = 22;
    part->model->polygons[11].v3.texture_coord = 20;
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
