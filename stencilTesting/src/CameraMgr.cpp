//
//  CameraMgr.cpp
//  model_loading
//
//  Created by lam on 18/10/22.
//  Copyright © 2018年 lam. All rights reserved.
//

#include "CameraMgr.hpp"

static CameraMgr* __cameraMgr = nullptr;

CameraMgr::CameraMgr()
{
    
}

CameraMgr::~CameraMgr()
{
    cleanup();
    __cameraMgr = nullptr;
}

CameraMgr* CameraMgr::getInstance()
{
    if (!__cameraMgr)
    {
        __cameraMgr = new CameraMgr();
    }
    return __cameraMgr;
}

Camera* CameraMgr::add(std::string name, glm::vec3 position, glm::vec3 direction, glm::vec3 up, float yaw, float pitch)
{
    if (isContain(name))
    {
        return nullptr;
    }
    Camera* p = new Camera(position, direction, up, yaw, pitch);
    _map_camera[name] = p;
    return p;
}

bool CameraMgr::del(std::string name)
{
    bool ret = false;
    if (isContain(name))
    {
        auto ite = _map_camera.find(name);
        delete ite->second;
        ite->second = nullptr;
        _map_camera.erase(ite);
        ret = true;
    }
    return ret;
}

bool CameraMgr::isContain(std::string name)
{
    bool ret = false;
    auto ite = _map_camera.find(name);
    if (ite != _map_camera.end())
    {
        ret = true;
    }
    return ret;
}

Camera* CameraMgr::get(std::string name)
{
    if (!isContain(name))
    {
        return nullptr;
    }
    return _map_camera[name];
}

void CameraMgr::cleanup()
{
    for (auto v : _map_camera)
    {
        delete v.second;
        v.second = nullptr;
    }
    _map_camera.clear();
}