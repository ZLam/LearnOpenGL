//
//  TextureMgr.cpp
//  model_loading
//
//  Created by lam on 18/10/19.
//  Copyright © 2018年 lam. All rights reserved.
//

#include "TextureMgr.hpp"
#include "stb_image.h"

static TextureMgr* __textureMgr = nullptr;

TextureMgr* TextureMgr::getInstance()
{
    if (!__textureMgr)
    {
        __textureMgr = new TextureMgr();
    }
    return __textureMgr;
}

TextureMgr::TextureMgr()
{
    init();
}

TextureMgr::~TextureMgr()
{
    cleanup();
    __textureMgr = nullptr;
}

bool TextureMgr::init()
{
//    stbi_set_flip_vertically_on_load(true);
    return true;
}

Texture* TextureMgr::addTexture(const std::string fullPath, const Texture_Type& type)
{
    std::unordered_map<std::string, Texture*>* _map;
    if (type == Texture_Type::Diffuse)
    {
        _map = &_diffuseTextureMap;
    }
    else
    {
        _map = &_specularTextureMap;
    }
    auto ite = _map->find(fullPath);
    if (ite == _map->end())
    {
        auto texture = new Texture(fullPath, type);
        (*_map)[fullPath] = texture;
    }
    return (*_map)[fullPath];
}

bool TextureMgr::delTexture(const std::string fullPath, const Texture_Type &type)
{
    bool ret = false;
    std::unordered_map<std::string, Texture*>* _map;
    if (type == Texture_Type::Diffuse)
    {
        _map = &_diffuseTextureMap;
    }
    else
    {
        _map = &_specularTextureMap;
    }
    auto ite = _map->find(fullPath);
    if (ite != _map->end())
    {
        delete ite->second;
        ite->second = nullptr;
        _map->erase(ite);
        ret = true;
    }
    return ret;
}

Texture* TextureMgr::getTexture(const std::string fullPath, const Texture_Type &type)
{
    std::unordered_map<std::string, Texture*>* _map;
    if (type == Texture_Type::Diffuse)
    {
        _map = &_diffuseTextureMap;
    }
    else
    {
        _map = &_specularTextureMap;
    }
    return (*_map)[fullPath];
}

bool TextureMgr::isContainTexture(const std::string fullPath, const Texture_Type& type)
{
    bool ret = false;
    std::unordered_map<std::string, Texture*>* _map;
    if (type == Texture_Type::Diffuse)
    {
        _map = &_diffuseTextureMap;
    }
    else
    {
        _map = &_specularTextureMap;
    }
    auto ite = _map->find(fullPath);
    if (ite != _map->end())
    {
        ret = true;
    }
    return ret;
}

void TextureMgr::cleanup()
{
    for (auto v : _diffuseTextureMap)
    {
        delete v.second;
        v.second = nullptr;
    }
    _diffuseTextureMap.clear();
    for (auto v : _specularTextureMap)
    {
        delete v.second;
        v.second = nullptr;
    }
    _specularTextureMap.clear();
}

Texture_Type TextureMgr::convert2TextureType(aiTextureType enum_aiTextureType)
{
    Texture_Type ret;
    if (enum_aiTextureType == aiTextureType_DIFFUSE)
    {
        ret = Texture_Type::Diffuse;
    }
    else if (enum_aiTextureType == aiTextureType_SPECULAR)
    {
        ret = Texture_Type::Specular;
    }
    else if (enum_aiTextureType == aiTextureType_HEIGHT)
    {
        ret = Texture_Type::Normal;
    }
    else if (enum_aiTextureType == aiTextureType_AMBIENT)
    {
        ret = Texture_Type::Height;
    }
    return ret;
}