//
//  Model.cpp
//  model_loading
//
//  Created by lam on 18/10/19.
//  Copyright © 2018年 lam. All rights reserved.
//

#include "Model.hpp"
#include "TextureMgr.hpp"

Model::Model(const std::string& fullPath) :
_fullPath(fullPath),
_position(glm::vec3(0.0f, 0.0f, 0.0f)),
_scale(glm::vec3(1.0f, 1.0f, 1.0f)),
_rotation(glm::vec3(0.0f, 0.0f, 0.0f)),
_modelMatrix(glm::mat4(1.0f)),
_dirty_modelMatrix(true)
{
    _directory = _fullPath.substr(0, _fullPath.find_last_of('/'));
    _load();
}

Model::~Model()
{
    if (!_arr_mesh.empty())
    {
        for (auto v : _arr_mesh)
        {
            delete v;
            v = nullptr;
        }
        _arr_mesh.clear();
    }
}

void Model::draw(Shader &shader)
{
    if (_arr_mesh.empty())
    {
        return;
    }
    for (auto v : _arr_mesh)
    {
        v->draw(shader);
    }
}

void Model::_load()
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(_fullPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << importer.GetErrorString() << std::endl;
        return;
    }
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* ai_node, const aiScene* ai_scene)
{
    for (unsigned int i = 0; i < ai_node->mNumMeshes; i++)
    {
        unsigned int index_mesh = ai_node->mMeshes[i];
        aiMesh* ai_mesh = ai_scene->mMeshes[index_mesh];
        processMesh(ai_mesh, ai_scene);
    }
    for (unsigned int i = 0; i < ai_node->mNumChildren; i++)
    {
        aiNode* tNode = ai_node->mChildren[i];
        processNode(tNode, ai_scene);
    }
}

void Model::processMesh(aiMesh* ai_mesh, const aiScene* ai_scene)
{
    std::vector<Vertex> arr_vertex;
    std::vector<unsigned int> arr_indices;
    std::vector<TextureInfo> arr_textureInfo;
    for (unsigned int i = 0; i < ai_mesh->mNumVertices; i++)
    {
        Vertex vertex;
        
        auto tPosition = ai_mesh->mVertices[i];
        vertex.position = glm::vec3(tPosition.x, tPosition.y, tPosition.z);
        
        auto tNormal = ai_mesh->mNormals[i];
        vertex.normal = glm::vec3(tNormal.x, tNormal.y, tNormal.z);
        
        if (ai_mesh->mTextureCoords[0])
        {
            auto tTextureCoord = ai_mesh->mTextureCoords[0][i];
            vertex.textureCoord = glm::vec2(tTextureCoord.x, tTextureCoord.y);
        }
        else
        {
            vertex.textureCoord = glm::vec2(0.0f, 0.0f);
        }
        
        if (ai_mesh->mColors[0])
        {
            auto tColor = ai_mesh->mColors[0][i];
            vertex.color = glm::vec3(tColor.r, tColor.g, tColor.b);
        }
        else
        {
            vertex.color = glm::vec3(1.0f, 1.0f, 1.0f);
        }
        
        auto tTangent = ai_mesh->mTangents[i];
        vertex.tangent = glm::vec3(tTangent.x, tTangent.y, tTangent.z);
        
        auto tBitangent = ai_mesh->mBitangents[i];
        vertex.bitangent = glm::vec3(tBitangent.x, tBitangent.y, tBitangent.z);
        
        arr_vertex.push_back(vertex);
    }
    
    for (unsigned int i = 0; i < ai_mesh->mNumFaces; i++)
    {
        aiFace ai_face = ai_mesh->mFaces[i];
        for (unsigned int j = 0; j < ai_face.mNumIndices; j++)
        {
            arr_indices.push_back(ai_face.mIndices[j]);
        }
    }
    
    aiMaterial* mtl = ai_scene->mMaterials[ai_mesh->mMaterialIndex];
    aiTextureType arr_aiTextureType[] = {
        aiTextureType_DIFFUSE,
        aiTextureType_SPECULAR,
        aiTextureType_HEIGHT,
        aiTextureType_AMBIENT,
    };
    for (auto tType : arr_aiTextureType)
    {
        for (int i = 0; i < mtl->GetTextureCount(tType); i++)
        {
            aiString tStr;
            mtl->GetTexture(tType, i, &tStr);
            std::string fullPath = _directory + '/' + tStr.C_Str();
            Texture_Type textureType = TextureMgr::getInstance()->convert2TextureType(tType);
            if (!TextureMgr::getInstance()->isContainTexture(fullPath, textureType))
            {
                TextureMgr::getInstance()->addTexture(fullPath, textureType);
                TextureInfo tInfo;
                tInfo.str = fullPath;
                tInfo.type = textureType;
                arr_textureInfo.push_back(tInfo);
            }
        }
    }
    
    Mesh* mesh = new Mesh(arr_vertex, arr_indices, arr_textureInfo, this);
    _arr_mesh.push_back(mesh);
}

glm::vec3 Model::getPosition()
{
    return _position;
}

void Model::setPosition(glm::vec3 position)
{
    _position = position;
    _dirty_modelMatrix = true;
}

glm::vec3 Model::getScale()
{
    return _scale;
}

void Model::setScale(glm::vec3 scale)
{
    _scale = scale;
    _dirty_modelMatrix = true;
}

glm::vec3 Model::getRotation()
{
    return _rotation;
}

void Model::setRotaion(glm::vec3 rotation)
{
    _rotation = rotation;
    _dirty_modelMatrix = true;
}

glm::mat4 Model::getModelMatrix()
{
    if (_dirty_modelMatrix)
    {
        glm::mat4 mat4_translate = glm::mat4(1.0f);
        glm::mat4 mat4_scale = glm::mat4(1.0f);
        glm::mat4 mat4_rotation_xAsix = glm::mat4(1.0f);
        glm::mat4 mat4_rotation_yAsix = glm::mat4(1.0f);
        glm::mat4 mat4_rotation_zAsix = glm::mat4(1.0f);
        mat4_translate = glm::translate(mat4_translate, _position);
        mat4_scale = glm::scale(mat4_scale, _scale);
        mat4_rotation_xAsix = glm::rotate(mat4_rotation_xAsix, _rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        mat4_rotation_yAsix = glm::rotate(mat4_rotation_yAsix, _rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        mat4_rotation_zAsix = glm::rotate(mat4_rotation_zAsix, _rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 mat4_rotation = mat4_rotation_xAsix * mat4_rotation_yAsix * mat4_rotation_zAsix;
        _modelMatrix = mat4_translate * mat4_scale * mat4_rotation;
        _dirty_modelMatrix = false;
    }
    return _modelMatrix;
}