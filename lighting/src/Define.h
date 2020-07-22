//
//  Define.h
//  test
//
//  Created by lam on 18/6/20.
//  Copyright © 2018年 lam. All rights reserved.
//

#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <random>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define GLSL_330(src) "#version 330 core\n" #src

enum PolygonMode
{
    Fill = 0,
    Line = 1,
};

//struct Color4f
//{
//    float r;
//    float g;
//    float b;
//    float a;
//    
//    Color4f(float red = 0.0f, float green = 0.0f, float blue = 0.0f, float alpha = 1.0f)
//    {
//        r = red;
//        g = green;
//        b = blue;
//        a = alpha;
//    }
//    
//    void operator = (const Color4f& color)
//    {
//        r = color.r;
//        g = color.g;
//        b = color.b;
//        a = color.a;
//    }
//};

const unsigned int Window_Width = 1280;
const unsigned int Window_Height = 720;

PolygonMode g_polygonMode = PolygonMode::Fill;

const char* Shader_VertexSource = GLSL_330
(
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;
    out vec3 outColor;
    void main()
    {
        outColor = aColor;
        gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    }
);

const char* Shader_FragmentSource = GLSL_330
(
    in vec3 outColor;
    out vec4 FragColor;
    uniform float fAlpha;
    void main()
    {
        FragColor = vec4(outColor, 1.0f);
    }
);

std::random_device myRandom;
float getRandom_0_1()
{
    float f1 = myRandom();
    float f2 = myRandom.max();
    return f1 / f2;
}

std::string getStringFromFile(std::string fullPath)
{
    std::string str;
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        file.open(fullPath);
        std::stringstream strStream;
        strStream << file.rdbuf();
        str = strStream.str();
        file.close();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "err : getStringFromFile fail" << std::endl;
    }
    return str;
}