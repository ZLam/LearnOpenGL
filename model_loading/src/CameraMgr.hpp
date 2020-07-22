//
//  CameraMgr.hpp
//  model_loading
//
//  Created by lam on 18/10/22.
//  Copyright © 2018年 lam. All rights reserved.
//

#pragma once

#include "Camera.hpp"
#include <string>
#include <unordered_map>

class CameraMgr
{
public:
    ~CameraMgr();
    static CameraMgr* getInstance();
    Camera* add(std::string name, glm::vec3 position, glm::vec3 direction, glm::vec3 up, float yaw = -90.0f, float pitch = 0.0f);
    bool del(std::string name);
    bool isContain(std::string name);
    Camera* get(std::string name);
    void cleanup();
    
protected:
    std::unordered_map<std::string, Camera*> _map_camera;
    
private:
    CameraMgr();
};