#include "Transform.h"


Transform::Transform(const Vector3& position, const Quaternion& rotation, const Vector3& scale):
    position { position }, rotation { rotation }, scale { scale } {

}

Transform::Transform(const Vector3& position, const Vector3& scale) : Transform { position, identityQuaternion(), scale } {

}

Transform::Transform() : Transform { { 0, 0, 0 } } {

}


Matrix4 Transform::matrix() const {
    return translationMatrix() * rotationMatrix() * scaleMatrix();
}

Matrix4 Transform::translationMatrix() const {
    return ::translationMatrix(position);
}

Matrix4 Transform::rotationMatrix() const {
    return ::rotationMatrix(rotation);
}

Matrix4 Transform::scaleMatrix() const {
    return ::scaleMatrix(scale);
}


Vector3 Transform::forward() const {
    Matrix4 m = rotationMatrix();
    return normalize(Vector3 { m[2][0], m[2][1], m[2][2] });
}

Vector3 Transform::up() const {
    Matrix4 m = rotationMatrix();
    return normalize(Vector3 { m[1][0], m[1][1], m[1][2] });
}

Vector3 Transform::right() const {
    Matrix4 m = rotationMatrix();
    return normalize(Vector3 { m[0][0], m[0][1], m[0][2] });
}


void Transform::lookAt(const Vector3& target) {
    Vector3 direction = normalize(target - position);
    Vector3 up { 0, 1, 0 };
    Vector3 right = normalize(cross(up, direction));
    up = normalize(cross(direction, right));
    rotation = quat_cast(Matrix3 { right, up, direction });
}


void Transform::rotate(const Vector3& axis, float radians) {
    rotation = ::rotate(rotation, radians, axis);
}

void Transform::rotateX(float radians) {
    rotate({ 1, 0, 0 }, radians);
}

void Transform::rotateY(float radians) {
    rotate({ 0, 1, 0 }, radians);
}

void Transform::rotateZ(float radians) {
    rotate({ 0, 0, 1 }, radians);
}

void Transform::rotateWorldAxis(const Vector3& axis, float radians) {
    rotation = ::rotate(identityQuaternion(), radians, axis) * rotation;
}

void Transform::rotateWorldX(float radians) {
    rotateWorldAxis({ 1, 0, 0 }, radians);
}

void Transform::rotateWorldY(float radians) {
    rotateWorldAxis({ 0, 1, 0 }, radians);
}

void Transform::rotateWorldZ(float radians) {
    rotateWorldAxis({ 0, 0, 1 }, radians);
}
