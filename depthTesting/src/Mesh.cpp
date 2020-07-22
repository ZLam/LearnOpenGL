//
//  Mesh.cpp
//  model_loading
//
//  Created by lam on 18/10/19.
//  Copyright © 2018年 lam. All rights reserved.
//

#include "Mesh.hpp"
#include "Model.hpp"
#include "CameraMgr.hpp"
#include "TextureMgr.hpp"

Mesh::Mesh(std::vector<Vertex> arr_vertex, std::vector<unsigned int> arr_indices, std::vector<TextureInfo> arr_textureInfo, Model* model) :
_arr_vertex(arr_vertex),
_arr_indices(arr_indices),
_arr_textureInfo(arr_textureInfo),
_model(model)
{
    init();
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_ebo);
}

bool Mesh::init()
{
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ebo);
    
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * _arr_vertex.size(), &_arr_vertex[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * _arr_indices.size(), &_arr_indices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, position)));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, textureCoord)));
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, color)));
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, tangent)));
    glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, bitangent)));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBindVertexArray(0);
    
    return true;
}

void Mesh::draw(Shader& shader)
{
    auto camera = CameraMgr::getInstance()->get("main");
    shader.use();
    
    unsigned int num_diffuse = 0;
    unsigned int num_specular = 0;
    unsigned int num_normal = 0;
    unsigned int num_height = 0;
    for (unsigned int i = 0; i < _arr_textureInfo.size(); i++)
    {
        auto texture = TextureMgr::getInstance()->getTexture(_arr_textureInfo[i].str, _arr_textureInfo[i].type);
        std::string str;
        glActiveTexture(GL_TEXTURE0 + i);
        if (_arr_textureInfo[i].type == Texture_Type::Diffuse)
        {
            str = "texture_diffuse" + std::to_string(num_diffuse);
            num_diffuse++;
        }
        else if (_arr_textureInfo[i].type == Texture_Type::Specular)
        {
            str = "texture_specular" + std::to_string(num_specular);
            num_specular++;
        }
        else if (_arr_textureInfo[i].type == Texture_Type::Normal)
        {
            str = "texture_normal" + std::to_string(num_normal);
            num_normal++;
        }
        else if (_arr_textureInfo[i].type == Texture_Type::Height)
        {
            str = "texture_height" + std::to_string(num_height);
            num_height++;
        }
        shader.uniform1i(str, i);
        glBindTexture(GL_TEXTURE_2D, texture->getId());
    }
    
    shader.uniformMatrix4fv("model", _model->getModelMatrix());
    shader.uniformMatrix4fv("view", camera->getViewMatrix());
    shader.uniformMatrix4fv("projection", camera->getProjectionMatrix());
    
    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, _arr_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    
    glActiveTexture(GL_TEXTURE0);
}