#include "Renderer.h"

#include "../Game/Component/Light.h"
#include "../Game/Texture.h"
#include "../Shader/Material.h"

#include <gl/glew.h>
#include <gl/freeglut.h>

using namespace std;


Renderer::Renderer(Camera* camera, Shader* shader) : camera { camera }, render_mode { Solid }, shader { shader },
    vao { 0 }, vbo { 0 },
    icons_texture_id { 0 },
    render_distance { 8 },
    background_color { 0.0f, 0.0f, 0.0f },
    view_location { 0 },
    proj_location { 0 },
    cam_pos_location { 0 },
    trans_location { 0 },
    use_texture_location { 0 },
    color_location { 0 },
    shininess_location { 0 },
    reflectivity_location { 0 },
    num_lights_location { 0 } {

}

Renderer::Renderer() : Renderer { nullptr, nullptr } {

}


void Renderer::renderStart() {
    glClearColor(background_color.r, background_color.g, background_color.b, 1);

    glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(camera->viewMatrix()));
    glUniformMatrix4fv(proj_location, 1, GL_FALSE, glm::value_ptr(camera->projectionMatrix()));
    glUniform3fv(cam_pos_location, 1, glm::value_ptr(camera->transform.position));
}


void Renderer::renderObject(const Object* object) {
    if(!object->render) {
        return;
    }

    transform_stack.push(object->transform.matrix());

    Vector3 object_pos = transform_stack.top() * Vector4 { 0, 0, 0, 1 };
    Vector3 to_object = object_pos - camera->transform.position;
    float distance2 = length2(to_object);
    if(distance2 > render_distance*render_distance) {
        transform_stack.pop();
        return;
    }
    else if(distance2 > 5*5) {
        if(dot(normalize(to_object), camera->forward()) < cos(radians(camera->fovy)/* * camera->aspect*/)) {
            transform_stack.pop();
            return;
        }
    }

    glUniform1f(render_distance_location, render_distance);
    glUniform3f(fog_color_location, background_color.r, background_color.g, background_color.b);

    if(object->model != nullptr) {
        glUniformMatrix4fv(trans_location, 1, GL_FALSE, glm::value_ptr(transform_stack.top()));

        const ColorRGB& c = object->material.base_color;
        glUniform4f(color_location, c.r, c.g, c.b, c.a);
        glUniform1f(shininess_location, object->material.shininess);
        glUniform1f(reflectivity_location, object->material.reflectivity);
        glUniform1i(use_texture_location, object->model->texture_id.size());

        pushFacesToBuffer(object);
        initBuffer();

        switch(render_mode) {
        case Solid:
            for(int i=0; i<object->model->polygons.size(); ++i) {
                if(i < object->model->texture_id.size()) {
                    glBindTexture(GL_TEXTURE_2D, object->model->texture_id[i]);
                }
                glDrawArrays(GL_TRIANGLES, i*3, 3);
            }
            break;
        case Wire:
            renderWire();
            break;
        }

        clearBuffer();
    }

    for(auto& c : object->children) {
        renderObject(c.second);
    }

    transform_stack.pop();
}

void Renderer::pushObject(Object* object) {
    objects.push_back(object);
}

void Renderer::pushLightObject(Object* object) {
    light_objects.push_back(object);
}


void Renderer::render() {
    renderStart();
    
    glUniform1i(num_lights_location, 0);
	for(auto& e : light_objects) {
		renderObject(e);
	}
    glUniform1i(num_lights_location, (int)light_objects.size());

    for(int i=0; i<light_objects.size(); ++i) {
        Light* light = light_objects[i]->getComponent<Light>();
        Vector3 pos = light_objects[i]->absoluteTransformMatrix() * Vector4 { light->position, 1.0f };
        string light_name = "lights[" + to_string(i) + "]";
        unsigned int lights_location = glGetUniformLocation(shader->program_id, (light_name+".position").c_str());
        glUniform3fv(lights_location, 1, glm::value_ptr(pos));
        lights_location = glGetUniformLocation(shader->program_id, (light_name+".color").c_str());
        glUniform3f(lights_location, light->color.r, light->color.g, light->color.b);
        lights_location = glGetUniformLocation(shader->program_id, (light_name+".intensity").c_str());
        glUniform1f(lights_location, light->color.a);
        lights_location = glGetUniformLocation(shader->program_id, (light_name+".ambient").c_str());
        glUniform1f(lights_location, light->ambient);
        lights_location = glGetUniformLocation(shader->program_id, (light_name+".c1").c_str());
        glUniform1f(lights_location, light->c1);
        lights_location = glGetUniformLocation(shader->program_id, (light_name+".c2").c_str());
        glUniform1f(lights_location, light->c2);
    }

    for(auto& e : objects) {
        renderObject(e);
    }
	
    objects.clear();
    light_objects.clear();
}

void Renderer::renderCrosshair() {
    const float crosshair_size = 0.05f;
    const float texture_start_x = 0.0f;
    const float texture_start_y = 1.0 - 15.0f / 256.0f;
    const float texture_end_x = 15.0f / 256.0f;
    const float texture_end_y = 1.0f;

    Object* crosshair = new Object { "crosshair" };
    crosshair->transform.position = { 0, 0, -1 };
    crosshair->transform.scale = { crosshair_size/camera->aspect, crosshair_size, 1.0f };
    crosshair->model = new Model { };
    crosshair->model->points = {
        { -1.0f, -1.0f, 0.0f },
        { 1.0f, -1.0f, 0.0f },
        { 1.0f, 1.0f, 0.0f },
        { -1.0f, 1.0f, 0.0f }
    };
    crosshair->model->normals = {
        { 0.0f, 0.0f, 1.0f }
    };
    crosshair->model->texture_coords = {
        { texture_start_x, texture_start_y },
        { texture_end_x,   texture_start_y },
        { texture_end_x,   texture_end_y },
        { texture_start_x, texture_end_y }
    };
    crosshair->model->polygons = {
        { { 0, 0, 0 }, { 1, 0, 1 }, { 2, 0, 2 } },
        { { 0, 0, 0 }, { 2, 0, 2 }, { 3, 0, 3 } }
    };
    crosshair->material = Material::base;

    glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(Matrix4 { 1.0f }));
    glUniformMatrix4fv(proj_location, 1, GL_FALSE, glm::value_ptr(Matrix4 { 1.0f }));
    glUniform3fv(cam_pos_location, 1, glm::value_ptr(Matrix4 { 1.0f }));
    glUniformMatrix4fv(trans_location, 1, GL_FALSE, glm::value_ptr(crosshair->transform.matrix()));
    glUniform1i(num_lights_location, 0);
    glUniform1i(use_texture_location, icons_texture_id);

    pushFacesToBuffer(crosshair);
    initBuffer();

    glBindTexture(GL_TEXTURE_2D, icons_texture_id);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    clearBuffer();
    delete crosshair;
}


void Renderer::initBuffer() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2*sizeof(Vector3)+sizeof(Vector2), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2*sizeof(Vector3)+sizeof(Vector2), (void*)sizeof(Vector3));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2*sizeof(Vector3)+sizeof(Vector2), (void*)(2*sizeof(Vector3)));
    glEnableVertexAttribArray(2);
}

void Renderer::clearBuffer() {
    glDeleteBuffers(2, &vbo);
    glDeleteVertexArrays(1, &vao);
    vertices.clear();
}


void Renderer::setShader(Shader* shader) {
    this->shader = shader;
    glUseProgram(shader->program_id);

    view_location = glGetUniformLocation(shader->program_id, "view_transform");
    proj_location = glGetUniformLocation(shader->program_id, "projection_transform");
    cam_pos_location = glGetUniformLocation(shader->program_id, "cam_pos");
    trans_location = glGetUniformLocation(shader->program_id, "model_transform");
    use_texture_location = glGetUniformLocation(shader->program_id, "use_texture");
    color_location = glGetUniformLocation(shader->program_id, "material.color");
    shininess_location = glGetUniformLocation(shader->program_id, "material.shininess");
    reflectivity_location = glGetUniformLocation(shader->program_id, "material.reflectivity");
    num_lights_location = glGetUniformLocation(shader->program_id, "num_lights");
    render_distance_location = glGetUniformLocation(shader->program_id, "render_distance");
    fog_color_location = glGetUniformLocation(shader->program_id, "fog_color");
}


void Renderer::pushVertex(const Vector3& point, const Vector3& normal, const Vector2& tex_coord) {
    vertices.push_back(point.x);
    vertices.push_back(point.y);
    vertices.push_back(point.z);
    vertices.push_back(normal.x);
    vertices.push_back(normal.y);
    vertices.push_back(normal.z);
    vertices.push_back(tex_coord.x);
    vertices.push_back(tex_coord.y);
}

void Renderer::pushFacesToBuffer(const Object* object) {
    vector<Vector3>& points = object->model->points;
    vector<Vector3>& normals = object->model->normals;
    vector<IndexedPolygon>& faces = object->model->polygons;
    vector<Vector2>& tex_coords = object->model->texture_coords;
    for(const auto& face : faces) {
        pushVertex(points[face.v1.point], normals[face.v1.normal], tex_coords[face.v1.texture_coord]);
        pushVertex(points[face.v2.point], normals[face.v2.normal], tex_coords[face.v2.texture_coord]);
        pushVertex(points[face.v3.point], normals[face.v3.normal], tex_coords[face.v3.texture_coord]);
    }
}


void Renderer::renderSolid() const {
    glDrawArrays(GL_TRIANGLES, 0, (int)vertices.size()/8);
}

void Renderer::renderWire() const {
    int num_of_faces = (int)vertices.size() / 24;
    for(int i=0; i<num_of_faces; ++i) {
        glDrawArrays(GL_LINE_LOOP, i*3, 3);
    }
}
