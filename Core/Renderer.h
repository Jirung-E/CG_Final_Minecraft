#pragma once

#include "TransformationStack.h"
#include "Shader.h"
#include "../Game/Camera.h"
#include "../Game/Object.h"
#include "../Util/Util.h"
#include "../Math/Math.h"

#include <vector>


class Renderer {
private:
    TransformationStack transform_stack;

    GLuint vao, vbo[2];

    std::vector<Vector3> vertices;
    std::vector<Vector3> normals;

    std::vector<Object*> objects;
    std::vector<Object*> light_objects;

public:
    Camera* camera;
    Shader* shader;
    ColorRGB background_color;

    enum Mode {
        Solid, Wire
    } render_mode;

public:
    Renderer(Camera* camera, Shader* shader);
    Renderer();

public:
    void renderStart();
    void renderObject(const Object* object);
    void pushObject(Object* object);
    void pushLightObject(Object* object);
    void render();

    void initBuffer();
    void clearBuffer();

    void setShader(Shader* shader);

private:
    void pushVertex(const Vector3& point, const Vector3& normal);
    void pushFacesToBuffer(const Object* object);

    void renderSolid() const;
    void renderWire() const;
};