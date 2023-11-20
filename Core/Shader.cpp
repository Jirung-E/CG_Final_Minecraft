#define _CRT_SECURE_NO_WARNINGS

#include "Shader.h"

#include "../Util/Util.h"

#include <fstream>

using namespace std;


static char* fileString(const string& file_name) {
    FILE* fptr = fopen(file_name.c_str(), "rb");
    if(!fptr) {
        return nullptr;
    }

    fseek(fptr, 0, SEEK_END);
    long int length = ftell(fptr);

    fseek(fptr, 0, SEEK_SET);
    char* buf = new char[length+1];
    if(buf == nullptr) {
        return nullptr;
    }
    fread(buf, 1, length, fptr);
    fclose(fptr);

    buf[length] = '\0';

    return buf;
}


Shader::Shader(const string& vertex_shader, const string& fragment_shader):
    vertex_shader_file { vertex_shader }, fragment_shader_file { fragment_shader } {
    makeShaderProgram();
}


void Shader::makeShaderProgram() {
    makeVertexShaders(vertex_shader_file);
    makeFragmentShaders(fragment_shader_file);

    program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shader);
    glAttachShader(program_id, fragment_shader);

    glLinkProgram(program_id);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void Shader::makeVertexShaders(const std::string& file) {
    GLchar* vertex_source;
    vertex_source = fileString(file);
    Log::log("Vertex shader source: \n%s", vertex_source);
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, (const GLchar**)&vertex_source, 0);
    glCompileShader(vertex_shader);

    GLint result;
    GLchar error_log[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &result);
    if(!result) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, error_log);
        Log::errorLog("ERROR: vertex shader compile failed: \n" + string { error_log });
        return;
    }
}

void Shader::makeFragmentShaders(const std::string& file) {
    GLchar* fragment_source;
    fragment_source = fileString(file);
    Log::log("Fragment shader source: \n%s", fragment_source);
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, (const GLchar**)&fragment_source, 0);
    glCompileShader(fragment_shader);

    GLint result;
    GLchar error_log[512];
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &result);
    if(!result) {
        glGetShaderInfoLog(fragment_shader, 512, NULL, error_log);
        Log::errorLog("ERROR: fragment shader compile failed: \n" + string { error_log });
        return;
    }
}
