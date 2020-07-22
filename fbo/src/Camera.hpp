//
//  Camera.hpp
//  model_loading
//
//  Created by lam on 18/10/16.
//  Copyright © 2018年 lam. All rights reserved.
//

#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Camera
{
public:
    Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 up, float yaw = -90.0f, float pitch = 0.0f);
    void setPosition(const glm::vec3& position);
    void setRotation(float yaw, float pitch);
    glm::vec3 getPosition();
    glm::vec3 getDirection();
    glm::vec3 getUp();
    glm::vec3 getRight();
    float getYaw();
    float getPitch();
    float getNear();
    float getFar();
    void setNear(float near);
    void setFar(float far);
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();
    void updateVectors();
    
protected:
    glm::vec3 _position;
    glm::vec3 _direction;
    glm::vec3 _up;
    glm::vec3 _right;
    glm::vec3 _worldUp;
    float _yaw;
    float _pitch;
    float _near;
    float _far;
};