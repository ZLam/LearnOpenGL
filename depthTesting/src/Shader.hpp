//
//  Shader.hpp
//  model_loading
//
//  Created by lam on 18/10/15.
//  Copyright © 2018年 lam. All rights reserved.
//

#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <string>

class Shader
{
public:
    Shader(const std::string& fullPath_vertex, const std::string& fullPath_fragment);
    unsigned int getId();
    void use();
    void cleanup();
    int getUniformLocation(const std::string& name);
    void uniform1i(const std::string& name, int value);
    void uniform1f(const std::string& name, float value);
    void uniform2f(const std::string& name, float x, float y);
    void uniform3f(const std::string& name, float x, float y, float z);
    void uniform4f(const std::string& name, float x, float y, float z, float w);
    void uniform2fv(const std::string& name, const glm::vec2& value);
    void uniform3fv(const std::string& name, const glm::vec3& value);
    void uniform4fv(const std::string& name, const glm::vec4& value);
    void uniformMatrix2fv(const std::string& name, const glm::mat2& value);
    void uniformMatrix3fv(const std::string& name, const glm::mat3& value);
    void uniformMatrix4fv(const std::string& name, const glm::mat4& value);
    
private:
    unsigned int _id;
    
    unsigned int compileShader(const std::string& fullPath, GLenum shaderType);
    bool check_shader_compile_status(GLuint obj);
    bool check_program_link_status(GLuint obj);
};