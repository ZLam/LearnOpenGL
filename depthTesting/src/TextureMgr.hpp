//
//  TextureMgr.hpp
//  model_loading
//
//  Created by lam on 18/10/19.
//  Copyright © 2018年 lam. All rights reserved.
//

#pragma once

#include "Texture.hpp"
#include "assimp/material.h"
#include <string>
#include <unordered_map>

class TextureMgr
{
public:
    ~TextureMgr();
    bool init();
    static TextureMgr* getInstance();
    Texture* addTexture(const std::string fullPath, const Texture_Type& type);
    bool delTexture(const std::string fullPath, const Texture_Type& type);
    Texture* getTexture(const std::string fullPath, const Texture_Type& type);
    bool isContainTexture(const std::string fullPath, const Texture_Type& type);
    void cleanup();
    Texture_Type convert2TextureType(aiTextureType enum_aiTextureType);
    
protected:
    std::unordered_map<std::string, Texture*> _diffuseTextureMap;
    std::unordered_map<std::string, Texture*> _specularTextureMap;
    
private:
    TextureMgr();
};