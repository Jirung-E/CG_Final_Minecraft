#include "Collider.h"

using namespace std;


Collider::Collider(Object* object, const string& id) : ComponentImpl { object, id },
    is_colliding { false } {

}


void Collider::update(float dt) {
    is_colliding = false;
}


// -------------------------------------------------------------------------------------------- //

AxisAlignedBoxCollider::AxisAlignedBoxCollider(Object* object, const string& id, const Vector3& size, const Vector3& center) : Collider { object, id },
size { size }, center { center } {

}

bool AxisAlignedBoxCollider::isColliding(const AxisAlignedBoxCollider& collider1, const AxisAlignedBoxCollider& collider2) {
    return false;
}

// -------------------------------------------------------------------------------------------- //

static bool collide(AxisAlignedBoxCollider* box1, AxisAlignedBoxCollider* box2) {
    auto mat = box1->object->transform.translationMatrix() * box1->object->transform.scaleMatrix();
    Vector3 c = mat * Vector4 { box1->center, 1 };
    Vector3 center1 = { c.x, c.y, c.z };
    Vector3 size1 = mat * Vector4 { box1->size, 0 };

    mat = box2->object->transform.translationMatrix() * box2->object->transform.scaleMatrix();
    c = mat * Vector4 { box2->center, 1 };
    Vector3 center2 = { c.x, c.y, c.z };
    Vector3 size2 = mat * Vector4 { box2->size, 0 };

    float wx1 = size1.x / 2.0f;
    float wy1 = size1.y / 2.0f;
    float wz1 = size1.z / 2.0f;
    float top1 = center1.y + wy1;
    float bottom1 = center1.y - wy1;
    float left1 = center1.x - wx1;
    float right1 = center1.x + wx1;
    float front1 = center1.z + wz1;
    float back1 = center1.z - wz1;

    float wx2 = size2.x / 2.0f;
    float wy2 = size2.y / 2.0f;
    float wz2 = size2.z / 2.0f;
    float top2 = center2.y + wy2;
    float bottom2 = center2.y - wy2;
    float left2 = center2.x - wx2;
    float right2 = center2.x + wx2;
    float front2 = center2.z + wz2;
    float back2 = center2.z - wz2;

    if(left1 > right2 || right1 < left2 || top1 < bottom2 || bottom1 > top2 || front1 < back2 || back1 > front2) {
        return false;
    }

    box1->is_colliding = true;
    box2->is_colliding = true;

    return true;
}


// -------------------------------------------------------------------------------------------- //


bool collide(Collider* collider1, Collider* collider2) {
    if(dynamic_cast<AxisAlignedBoxCollider*>(collider1) && dynamic_cast<AxisAlignedBoxCollider*>(collider2)) {
        return collide(dynamic_cast<AxisAlignedBoxCollider*>(collider1), dynamic_cast<AxisAlignedBoxCollider*>(collider2));
    }
    return false;
}
