//
//  Mesh.hpp
//  model_loading
//
//  Created by lam on 18/10/19.
//  Copyright © 2018年 lam. All rights reserved.
//

#pragma once

#include "Shader.hpp"
#include "Texture.hpp"
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <vector>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textureCoord;
    glm::vec3 color;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

struct TextureInfo
{
    std::string str;
    Texture_Type type;
};

class Model;

class Mesh
{
public:
    Mesh(std::vector<Vertex> arr_vertex, std::vector<unsigned int> arr_indices, std::vector<TextureInfo> arr_textureInfo, Model* model);
    ~Mesh();
    bool init();
    void draw(Shader& shader);
    
protected:
    std::vector<Vertex> _arr_vertex;
    std::vector<unsigned int> _arr_indices;
    std::vector<TextureInfo> _arr_textureInfo;
    unsigned int _vao;
    unsigned int _vbo;
    unsigned int _ebo;
    Model* _model;
};