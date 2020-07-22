//
//  Model.hpp
//  model_loading
//
//  Created by lam on 18/10/19.
//  Copyright © 2018年 lam. All rights reserved.
//

#pragma once

#include "Shader.hpp"
#include "Mesh.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <string>
#include <iostream>

class Model
{
public:
    Model(const std::string& fullPath);
    ~Model();
    void draw(Shader& shader);
    glm::vec3 getPosition();
    void setPosition(glm::vec3 position);
    glm::vec3 getScale();
    void setScale(glm::vec3 scale);
    glm::vec3 getRotation();
    void setRotaion(glm::vec3 rotation);
    glm::mat4 getModelMatrix();
    
protected:
    std::vector<Mesh*> _arr_mesh;
    std::string _fullPath;
    std::string _directory;
    glm::vec3 _position;
    glm::vec3 _scale;
    glm::vec3 _rotation;
    glm::mat4 _modelMatrix;
    bool _dirty_modelMatrix;
    
    void _load();
    void processNode(aiNode* ai_node, const aiScene* ai_scene);
    void processMesh(aiMesh* ai_mesh, const aiScene* ai_scene);
};