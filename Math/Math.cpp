#include "Math.h"


Quaternion identityQuaternion() {
    return Quaternion(1, 0, 0, 0);  // w, x, y, z
}

Matrix4 translationMatrix(const Vector3& translation) {
    return glm::translate(glm::mat4(1.0f), translation);
}

Matrix4 rotationMatrix(const Quaternion& rotation) {
    return glm::toMat4(rotation);
}

Matrix4 scaleMatrix(const Vector3& scale) {
    return glm::scale(glm::mat4(1.0f), scale);
}
