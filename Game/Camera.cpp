#include "Camera.h"


Camera::Camera(const Vector3& position):
    transform { position },
    fovy { 60.0f },
    aspect { 800.0f/600.0f },
    near_clip { 0.1f },
    far_clip { 100.0f },
    view_mode { Perspective } {

}


void Camera::lookAt(const Vector3& target) {
    transform.lookAt(target);
    transform.rotateY(radians(180.0f));
}

Vector3 Camera::forward() const {
    return -transform.forward();
}


void Camera::switchViewMode() {
    view_mode = Mode((view_mode + 1) % 2);
}


void Camera::rotateHorizontal(float radians) {
    transform.rotateWorldY(radians);
}

void Camera::rotateVertical(float radians) {
    // up/down limit
    Vector3 direction = forward();
    float s = dot(Vector3 { 0, 1, 0 }, direction);
    float angle = acos(s) + radians;
    if(angle <= 0) {
        radians = -acos(s);
    }
    else if(angle >= M_PI) {
        radians = static_cast<float>(M_PI - acos(s));
    }

    transform.rotateX(radians);
}


Matrix4 Camera::viewMatrix() const {
    return inverse(transform.matrix());
}

Matrix4 Camera::projectionMatrix() const {
    Matrix4 proj_matrix = Matrix4(1.0f);
    switch(view_mode) {
    case Perspective:
        proj_matrix = perspective(radians(fovy), aspect, near_clip, far_clip);
        break;
    case Orthogonal:
        proj_matrix = ortho(-1.2f*aspect, 1.2f*aspect, -1.2f, 1.2f, near_clip, far_clip);
        break;
    }
    return proj_matrix;
}
