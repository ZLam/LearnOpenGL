//
//  Shader.cpp
//  model_loading
//
//  Created by lam on 18/10/15.
//  Copyright © 2018年 lam. All rights reserved.
//

#include "Shader.hpp"
#include "FileHelper.hpp"
#include <iostream>
#include <vector>

Shader::Shader(const std::string& fullPath_vertex, const std::string& fullPath_fragment)
{
    _id = -1;
    unsigned int nShader_vertex = compileShader(fullPath_vertex, GL_VERTEX_SHADER);
    if (nShader_vertex == -1)
    {
        return;
    }
    unsigned int nShader_fragment = compileShader(fullPath_fragment, GL_FRAGMENT_SHADER);
    if (nShader_fragment == -1)
    {
        return;
    }
    unsigned int nShaderProgram;
    nShaderProgram = glCreateProgram();
    glAttachShader(nShaderProgram, nShader_vertex);
    glAttachShader(nShaderProgram, nShader_fragment);
    glLinkProgram(nShaderProgram);
    if (!check_program_link_status(nShaderProgram))
    {
        return;
    }
    glDeleteShader(nShader_vertex);
    glDeleteShader(nShader_fragment);
    _id = nShaderProgram;
}

unsigned int Shader::getId()
{
    return _id;
}

void Shader::use()
{
    if (_id == -1)
    {
        std::cout << "warning : Shader::use(), dont use shader, id is -1" << std::endl;
        return;
    }
    glUseProgram(_id);
}

void Shader::cleanup()
{
    if (_id == -1)
    {
        return;
    }
    glDeleteProgram(_id);
    _id = -1;
}

int Shader::getUniformLocation(const std::string& name)
{
    return glGetUniformLocation(_id, name.c_str());
}

void Shader::uniform1i(const std::string &name, int value)
{
    if (_id == -1)
    {
        std::cout << "warning : Shader::uniform1i(), dont use shader, id is -1" << std::endl;
        return;
    }
    int index = getUniformLocation(name);
    if (index != -1)
    {
        glUniform1i(index, value);
    }
}

void Shader::uniform1f(const std::string &name, float value)
{
    if (_id == -1)
    {
        std::cout << "warning : Shader::uniform1f(), dont use shader, id is -1" << std::endl;
        return;
    }
    int index = getUniformLocation(name);
    if (index != -1)
    {
        glUniform1f(index, value);
    }
}

void Shader::uniform2f(const std::string &name, float x, float y)
{
    if (_id == -1)
    {
        std::cout << "warning : Shader::uniform2f(), dont use shader, id is -1" << std::endl;
        return;
    }
    int index = getUniformLocation(name);
    if (index != -1)
    {
        glUniform2f(index, x, y);
    }
}

void Shader::uniform3f(const std::string &name, float x, float y, float z)
{
    if (_id == -1)
    {
        std::cout << "warning : Shader::uniform3f(), dont use shader, id is -1" << std::endl;
        return;
    }
    int index = getUniformLocation(name);
    if (index != -1)
    {
        glUniform3f(index, x, y, z);
    }
}

void Shader::uniform4f(const std::string &name, float x, float y, float z, float w)
{
    if (_id == -1)
    {
        std::cout << "warning : Shader::uniform4f(), dont use shader, id is -1" << std::endl;
        return;
    }
    int index = getUniformLocation(name);
    if (index != -1)
    {
        glUniform4f(index, x, y, z, w);
    }
}

void Shader::uniform2fv(const std::string &name, const glm::vec2 &value)
{
    if (_id == -1)
    {
        std::cout << "warning : Shader::uniform2fv(), dont use shader, id is -1" << std::endl;
        return;
    }
    int index = getUniformLocation(name);
    if (index != -1)
    {
        glUniform2fv(index, 1, glm::value_ptr(value));
    }
}

void Shader::uniform3fv(const std::string &name, const glm::vec3 &value)
{
    if (_id == -1)
    {
        std::cout << "warning : Shader::uniform3fv(), dont use shader, id is -1" << std::endl;
        return;
    }
    int index = getUniformLocation(name);
    if (index != -1)
    {
        glUniform3fv(index, 1, glm::value_ptr(value));
    }
}

void Shader::uniform4fv(const std::string &name, const glm::vec4 &value)
{
    if (_id == -1)
    {
        std::cout << "warning : Shader::uniform4fv(), dont use shader, id is -1" << std::endl;
        return;
    }
    int index = getUniformLocation(name);
    if (index != -1)
    {
        glUniform4fv(index, 1, glm::value_ptr(value));
    }
}

void Shader::uniformMatrix2fv(const std::string &name, const glm::mat2 &value)
{
    if (_id == -1)
    {
        std::cout << "warning : Shader::uniformMatrix2fv(), dont use shader, id is -1" << std::endl;
        return;
    }
    int index = getUniformLocation(name);
    if (index != -1)
    {
        glUniformMatrix2fv(index, 1, GL_FALSE, glm::value_ptr(value));
    }
}

void Shader::uniformMatrix3fv(const std::string &name, const glm::mat3 &value)
{
    if (_id == -1)
    {
        std::cout << "warning : Shader::uniformMatrix3fv(), dont use shader, id is -1" << std::endl;
        return;
    }
    int index = getUniformLocation(name);
    if (index != -1)
    {
        glUniformMatrix3fv(index, 1, GL_FALSE, glm::value_ptr(value));
    }
}

void Shader::uniformMatrix4fv(const std::string &name, const glm::mat4 &value)
{
    if (_id == -1)
    {
        std::cout << "warning : Shader::uniformMatrix4fv(), dont use shader, id is -1" << std::endl;
        return;
    }
    int index = getUniformLocation(name);
    if (index != -1)
    {
        glUniformMatrix4fv(index, 1, GL_FALSE, glm::value_ptr(value));
    }
}

unsigned int Shader::compileShader(const std::string& fullPath, GLenum shaderType)
{
    unsigned int nShader;
    nShader = glCreateShader(shaderType);
    std::string str_shaderSource = getStringFromFile(fullPath.c_str());
    const char* c_shaderSource = str_shaderSource.c_str();
    glShaderSource(nShader, 1, &c_shaderSource, nullptr);
    glCompileShader(nShader);
    if (!check_shader_compile_status(nShader))
    {
        return -1;
    }
    return nShader;
}

bool Shader::check_shader_compile_status(GLuint obj)
{
    GLint status;
    glGetShaderiv(obj, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE)
    {
        GLint length;
        glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> log(length);
        glGetShaderInfoLog(obj, length, &length, &log[0]);
        std::cerr << &log[0];
        return false;
    }
    return true;
}

bool Shader::check_program_link_status(GLuint obj)
{
    GLint status;
    glGetProgramiv(obj, GL_LINK_STATUS, &status);
    if(status == GL_FALSE)
    {
        GLint length;
        glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> log(length);
        glGetProgramInfoLog(obj, length, &length, &log[0]);
        std::cerr << &log[0];
        return false;
    }
    return true;
}