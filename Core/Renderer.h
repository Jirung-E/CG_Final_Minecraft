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

    GLuint vao, vbo;

    std::vector<float> vertices;

    std::vector<Object*> objects;
    std::vector<Object*> light_objects;

    unsigned int view_location;
    unsigned int proj_location;
    unsigned int cam_pos_location;
    unsigned int trans_location;
    unsigned int use_texture_location;
    unsigned int color_location;
    unsigned int shininess_location;
    unsigned int reflectivity_location;
    unsigned int num_lights_location;
    unsigned int render_distance_location;
    unsigned int fog_color_location;

public:
    Camera* camera;
    Shader* shader;
    ColorRGB background_color;
    unsigned int icons_texture_id;
    int render_distance;
    //ColorRGB fog_color;

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
    void renderCrosshair();

    void initBuffer();
    void clearBuffer();

    void setShader(Shader* shader);

    void pushVertex(const Vector3& point, const Vector3& normal, const Vector2& tex_coord=Vector2 { 0, 0 });

private:
    void pushFacesToBuffer(const Object* object);

    void renderSolid() const;
    void renderWire() const;
};