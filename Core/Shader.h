#pragma once

#include <string>

#include <gl/glew.h>


class Shader {
public:
    GLuint vertex_shader, fragment_shader;
    GLuint program_id;

    const std::string vertex_shader_file;
    const std::string fragment_shader_file;

public:
    Shader(const std::string& vertex_shader, const std::string& fragment_shader);

private:
    void makeShaderProgram();
    void makeVertexShaders(const std::string& file);
    void makeFragmentShaders(const std::string& file);
};