//
//  Texture.hpp
//  model_loading
//
//  Created by lam on 18/10/19.
//  Copyright © 2018年 lam. All rights reserved.
//

#pragma once

#include "glad/glad.h"
#include <iostream>
#include <string>

enum Texture_Type
{
    Diffuse = 0,
    Specular,
    Normal,
    Height,
};

unsigned int loadTexture(const std::string& fullPath);

class Texture
{
public:
    Texture(const std::string& fullPath, const Texture_Type& type);
    ~Texture();
    unsigned int getId();
    
protected:
    unsigned int _id;
    Texture_Type _type;
};