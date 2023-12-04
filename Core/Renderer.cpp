#include "Renderer.h"

#include "../Game/Component/Light.h"
#include "../Shader/Material.h"

#include <gl/glew.h>
#include <gl/freeglut.h>

using namespace std;


Renderer::Renderer(Camera* camera, Shader* shader) : camera { camera }, render_mode { Solid }, shader { shader },
    vao { 0 }, vbo { 0 },
    background_texture_id { 0 },
    background_color { 0.0f, 0.0f, 0.0f } {

}

Renderer::Renderer() : Renderer { nullptr, nullptr } {

}


void Renderer::renderStart() {
    glClearColor(background_color.r, background_color.g, background_color.b, 1);

    unsigned int view_location = glGetUniformLocation(shader->program_id, "view_transform");
    glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(camera->viewMatrix()));

    unsigned int proj_location = glGetUniformLocation(shader->program_id, "projection_transform");
    glUniformMatrix4fv(proj_location, 1, GL_FALSE, glm::value_ptr(camera->projectionMatrix()));

    unsigned int cam_pos = glGetUniformLocation(shader->program_id, "cam_pos");
    glUniform3fv(cam_pos, 1, glm::value_ptr(camera->transform.position));
}


void Renderer::renderObject(const Object* object) {
    if(!object->render) {
        return;
    }

    transform_stack.push(object->transform.matrix());

    if(object->model != nullptr) {
        unsigned int trans_location = glGetUniformLocation(shader->program_id, "model_transform");
        glUniformMatrix4fv(trans_location, 1, GL_FALSE, glm::value_ptr(transform_stack.top()));

        unsigned int color_location = glGetUniformLocation(shader->program_id, "material.color");
        const ColorRGB& c = object->material.base_color;
        glUniform4f(color_location, c.r, c.g, c.b, c.a);
        unsigned int shininess_location = glGetUniformLocation(shader->program_id, "material.shininess");
        glUniform1f(shininess_location, object->material.shininess);
        unsigned int reflectivity_location = glGetUniformLocation(shader->program_id, "material.reflectivity");
        glUniform1f(reflectivity_location, object->material.reflectivity);

        unsigned int use_texture_location = glGetUniformLocation(shader->program_id, "use_texture");
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
    
    unsigned int num_lights_location = glGetUniformLocation(shader->program_id, "num_lights");
    glUniform1i(num_lights_location, 0);
	for(auto& e : light_objects) {
		renderObject(e);
	}

    glUniform1i(num_lights_location, (int)light_objects.size());

    for(int i=0; i<light_objects.size(); ++i) {
        Light* light = light_objects[i]->getComponent<Light>();
        ColorRGB color = light_objects[i]->material.base_color;
        Vector3 pos = light_objects[i]->absoluteTransformMatrix() * Vector4 { light->position, 1.0f };
        string light_name = "lights[" + to_string(i) + "]";
        unsigned int lights_location = glGetUniformLocation(shader->program_id, (light_name+".position").c_str());
        glUniform3fv(lights_location, 1, glm::value_ptr(pos));
        lights_location = glGetUniformLocation(shader->program_id, (light_name+".color").c_str());
        glUniform3f(lights_location, color.r, color.g, color.b);
        lights_location = glGetUniformLocation(shader->program_id, (light_name+".intensity").c_str());
        glUniform1f(lights_location, color.a);
        lights_location = glGetUniformLocation(shader->program_id, (light_name+".ambient").c_str());
        glUniform1f(lights_location, light->ambient);
    }

    for(auto& e : objects) {
        renderObject(e);
    }
	
    objects.clear();
    light_objects.clear();
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
