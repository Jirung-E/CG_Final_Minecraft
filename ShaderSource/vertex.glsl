#version 330 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;

out vec3 frag_pos;
out vec3 frag_normal;

uniform mat4 model_transform;
uniform mat4 view_transform;
uniform mat4 projection_transform;

void main(void) {
    gl_Position = projection_transform * view_transform * model_transform * vec4(vertex_position, 1.0);

    frag_pos = vec3(model_transform * vec4(vertex_position, 1.0));
    mat4 normal_transform = transpose(inverse(model_transform));
    frag_normal = vec3(normal_transform * vec4(vertex_normal, 1.0));
}