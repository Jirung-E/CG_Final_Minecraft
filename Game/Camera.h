#pragma once

#include "Transform.h"


class Camera {
public:
    Transform transform;

    float height;   // ortho
    float fovy;     // degree
    float aspect;
    float near_clip;
    float far_clip;

    enum Mode {
        Perspective, Orthogonal
    } view_mode;

public:
    Camera(const Vector3& position={ 0, 0, 10 });

public:
    void lookAt(const Vector3& target);
    Vector3 forward() const;

    void switchViewMode();

    void rotateHorizontal(float radians);
    void rotateVertical(float radians);

    Matrix4 viewMatrix() const;
    Matrix4 projectionMatrix() const;
};
