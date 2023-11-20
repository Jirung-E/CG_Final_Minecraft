#pragma once

#include "../Math/Math.h"


class Transform {
public:
    Vector3 position;
    Quaternion rotation;
    Vector3 scale;

public:
    Transform(const Vector3& position, const Quaternion& rotation, const Vector3& scale={ 1, 1, 1 });
    Transform(const Vector3& position, const Vector3& scale={ 1, 1, 1 });
    Transform();

public:
    Matrix4 matrix() const;
    Matrix4 translationMatrix() const;
    Matrix4 rotationMatrix() const;
    Matrix4 scaleMatrix() const;

    Vector3 forward() const;
    Vector3 up() const;
    Vector3 right() const;

    void lookAt(const Vector3& target);

    void rotate(const Vector3& axis, float radians);
    void rotateX(float radians);
    void rotateY(float radians);
    void rotateZ(float radians);

    void rotateWorldAxis(const Vector3& axis, float radians);
    void rotateWorldX(float radians);
    void rotateWorldY(float radians);
    void rotateWorldZ(float radians);
};
