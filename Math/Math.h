#pragma once

#include "MathDefines.h"

// glm위치가 바뀌면 여기서만 바꾸도록
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

using namespace glm;


using Vector2 = glm::vec2;
using Vector3 = glm::vec3;
using Vector4 = glm::vec4;
using Matrix3 = glm::mat3;
using Matrix4 = glm::mat4;
using Quaternion = glm::quat;

Quaternion identityQuaternion();
Matrix4 translationMatrix(const Vector3& translation);
Matrix4 rotationMatrix(const Quaternion& rotation);
Matrix4 scaleMatrix(const Vector3& scale);
