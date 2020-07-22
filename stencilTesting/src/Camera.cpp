//
//  Camera.cpp
//  model_loading
//
//  Created by lam on 18/10/16.
//  Copyright © 2018年 lam. All rights reserved.
//

#include "Camera.hpp"
#include "Define.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 up, float yaw, float pitch)
:_position(position),
_direction(direction),
_up(up),
_worldUp(up),
_yaw(yaw),
_pitch(pitch),
_near(0.1f),
_far(100.0f)
{
    updateVectors();
}

void Camera::setPosition(const glm::vec3& position)
{
    _position = position;
}

void Camera::setRotation(float yaw, float pitch)
{
    _yaw = yaw;
    if (pitch > 89.0f)
    {
        _pitch = 89.0f;
    }
    else if (pitch < -89.0f)
    {
        _pitch = -89.0f;
    }
    else
    {
        _pitch = pitch;
    }
    updateVectors();
}

glm::vec3 Camera::getPosition()
{
    return _position;
}

glm::vec3 Camera::getDirection()
{
    return _direction;
}

glm::vec3 Camera::getUp()
{
    return _up;
}

glm::vec3 Camera::getRight()
{
    return _right;
}

float Camera::getYaw()
{
    return _yaw;
}

float Camera::getPitch()
{
    return _pitch;
}

float Camera::getNear()
{
    return _near;
}

float Camera::getFar()
{
    return _far;
}

void Camera::setNear(float near)
{
    _near = near;
}

void Camera::setFar(float far)
{
    _far = far;
}

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(_position, _position + _direction, _worldUp);
}

glm::mat4 Camera::getProjectionMatrix()
{
    return glm::perspective(glm::radians(45.0f), (float)Window_Width / (float)Window_Height, _near, _far);
}

void Camera::updateVectors()
{
    glm::vec3 dir;
    dir.x = glm::cos(glm::radians(_pitch)) * glm::cos(glm::radians(_yaw));
    dir.y = glm::sin(glm::radians(_pitch));
    dir.z = glm::cos(glm::radians(_pitch)) * glm::sin(glm::radians(_yaw));
    _direction = glm::normalize(dir);
    _right = glm::normalize(glm::cross(_direction, _worldUp));
    _up = glm::normalize(glm::cross(_right, _direction));
}