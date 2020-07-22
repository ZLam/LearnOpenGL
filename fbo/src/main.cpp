//
//  main.cpp
//  lighting
//
//  Created by lam on 18/8/16.
//  Copyright © 2018年 lam. All rights reserved.
//

#include "Define.hpp"
#include "RandomHelper.hpp"
#include "Shader.hpp"
#include "Model.hpp"
#include "CameraMgr.hpp"
#include "TextureMgr.hpp"

#include <iostream>
#include <vector>
#include <unordered_map>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

struct Light
{
	Light(glm::vec3 position, glm::vec3 direction, std::vector<glm::vec3> colors, float constant, float linear, float quadratic, float cutOff, float outerCutOff)
	{
		this->position = position;
		this->direction = direction;
		this->colors = colors;
		this->constant = constant;
		this->linear = linear;
		this->quadratic = quadratic;
		this->cutOff = cutOff;
		this->outerCutOff = outerCutOff;
	}
	glm::vec3 position;
	glm::vec3 direction;
	std::vector<glm::vec3> colors;
	float constant;
	float linear;
	float quadratic;
	float cutOff;
	float outerCutOff;
};

std::unordered_map<int, bool> map_keyPress;
std::unordered_map<int, bool> map_mousePress;
glm::vec2 pos_mouseR_moved = glm::vec2(-1.0f, -1.0f);
float factor_mouseR_moved = 0.05;

const int cubeNum = 3;
glm::vec3 cubePosArr[cubeNum] = {
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(2.0f, 0.0f, 2.0f),
	glm::vec3(-2.0f, 2.0f, -2.0f),
};
float cubeRadiansArr[cubeNum] = {
	2 * glm::pi<float>() * random_0_1(),
	2 * glm::pi<float>() * random_0_1(),
	2 * glm::pi<float>() * random_0_1(),
};

const int lightNum = 3;
Light lightArr[lightNum] = {
	Light(
		  glm::vec3(0.0f, 10.0f, 0.0f),
		  glm::vec3(-1.0f, -1.0f, 0.0f),
		  std::vector<glm::vec3>({glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f)}),
		  1.0f,
		  0.09f,
		  0.0032f,
		  0.0f,
		  0.0f
	),
	Light(
		  glm::vec3(2.0f, 2.0f, 2.0f),
		  glm::vec3(0.0f, 0.0f, 0.0f),
		  std::vector<glm::vec3>({glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f)}),
		  1.0f,
		  0.09f,
		  0.0032f,
		  0.0f,
		  0.0f
	),
	Light(
		  glm::vec3(0.0f, 2.0f, 0.0f),
		  glm::vec3(0.0f, -1.0f, 0.0f),
		  std::vector<glm::vec3>({glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f)}),
		  1.0f,
		  0.09f,
		  0.0032f,
		  glm::cos(glm::radians(15.0f)),
		  glm::cos(glm::radians(18.0f))
	),
};

float cameraSpeed = 0.2f;
int nStencilTestingType = 0;
int nFboTestingType = 0;
int nEffect = 0;

bool init();
void updatePolygonMode();
void handleCameraMove();
void callback_keyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
void callback_mouseButton(GLFWwindow* window, int button, int action, int mods);
void callback_mousePos(GLFWwindow* window, double posX, double posY);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    GLFWwindow* window = glfwCreateWindow(Window_Width, Window_Height, "MyGame", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "failed to create glfw window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "failed to initialize glad" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwSetKeyCallback(window, callback_keyboard);
    glfwSetMouseButtonCallback(window, callback_mouseButton);
    glfwSetCursorPosCallback(window, callback_mousePos);
    glViewport(0, 0, Window_Width, Window_Height);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    init();
    
    Shader shader_axis =Shader("/Users/lam/Desktop/Workspace/TestGL/fbo/res/vertex_axis.vs", "/Users/lam/Desktop/Workspace/TestGL/fbo/res/fragment_axis.fs");
    Shader shader_bg = Shader("/Users/lam/Desktop/Workspace/TestGL/fbo/res/vertex_bg.vs", "/Users/lam/Desktop/Workspace/TestGL/fbo/res/fragment_bg.fs");
    Shader shader_lighting = Shader("/Users/lam/Desktop/Workspace/TestGL/fbo/res/vertex_lighting.vs", "/Users/lam/Desktop/Workspace/TestGL/fbo/res/fragment_lighting.fs");
    Shader shader_cube = Shader("/Users/lam/Desktop/Workspace/TestGL/fbo/res/vertex_cube.vs", "/Users/lam/Desktop/Workspace/TestGL/fbo/res/fragment_cube.fs");
    Shader shader_model = Shader("/Users/lam/Desktop/Workspace/TestGL/fbo/res/vertex_model.vs", "/Users/lam/Desktop/Workspace/TestGL/fbo/res/fragment_model.fs");
    Shader shader_stencil = Shader("/Users/lam/Desktop/Workspace/TestGL/fbo/res/vertex_stencil.vs", "/Users/lam/Desktop/Workspace/TestGL/fbo/res/fragment_stencil.fs");
    Shader shader_color = Shader("/Users/lam/Desktop/Workspace/TestGL/fbo/res/vertex_color.vs", "/Users/lam/Desktop/Workspace/TestGL/fbo/res/fragment_color.fs");
    Shader shader_texture = Shader("/Users/lam/Desktop/Workspace/TestGL/fbo/res/vertex_texture.vs", "/Users/lam/Desktop/Workspace/TestGL/fbo/res/fragment_texture.fs");
    
    if (shader_axis.getId() == -1 || shader_bg.getId() == -1 || shader_lighting.getId() == -1 || shader_cube.getId() == -1 || shader_model.getId() == -1)
	{
		glfwTerminate();
		return -1;
	}
	
    float verticesData_axis[] = {
        //x axis
        0.0f,   0.0f,   0.0f,   1.0f,   0.0f,   0.0f,
        100.0f, 0.0f,   0.0f,   1.0f,   0.0f,   0.0f,
        //y axis
        0.0f,   0.0f,   0.0f,   0.0f,   1.0f,   0.0f,
        0.0f,   100.0f, 0.0f,   0.0f,   1.0f,   0.0f,
        //z axis
        0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   1.0f,
        0.0f,   0.0f,   100.0f, 0.0f,   0.0f,   1.0f,
    };
    float verticesData_plane[] = {
        1.0f,   1.0f,   0.0f,
        1.0f,   -1.0f,  0.0f,
        -1.0f,  -1.0f,  0.0f,
        -1.0f,  1.0f,   0.0f,
    };
    float verticesData_plane2[] = {
        1.0f,   1.0f,   0.0f,   1.0f,   1.0f,
        -1.0f,  -1.0f,  0.0f,   0.0f,   0.0f,
        1.0f,   -1.0f,  0.0f,   1.0f,   0.0f,
        
        1.0f,   1.0f,   0.0f,   1.0f,   1.0f,
        -1.0f,  1.0f,   0.0f,   0.0f,   1.0f,
        -1.0f,  -1.0f,  0.0f,   0.0f,   0.0f,
    };
    unsigned int indicesData_plane[] = {
        0,  2,  1,
        0,  3,  2,
    };
    float verticesData_bg[] = {
		//pos					//color					//normal
        0.5f,   0.5f,   0.0f,   1.0f,   1.0f,   1.0f,   0.0f,   0.0f,   -1.0f,
        -0.5f,  0.5f,   0.0f,   1.0f,   1.0f,   1.0f,   0.0f,   0.0f,   -1.0f,
        0.5f,   -0.5f,  0.0f,   1.0f,   1.0f,   1.0f,   0.0f,   0.0f,   -1.0f,
        -0.5f,  0.5f,   0.0f,   1.0f,   1.0f,   1.0f,   0.0f,   0.0f,   -1.0f,
        -0.5f,  -0.5f,  0.0f,   1.0f,   1.0f,   1.0f,   0.0f,   0.0f,   -1.0f,
        0.5f,   -0.5f,  0.0f,   1.0f,   1.0f,   1.0f,   0.0f,   0.0f,   -1.0f,
    };
    float verticesData_lighting[] = {
        -0.5f,  -0.5f,  0.5f,
        0.5f,   -0.5f,  0.5f,
        0.5f,   0.5f,   0.5f,
        -0.5f,  0.5f,   0.5f,
        
        0.5f,   -0.5f,  -0.5f,
        -0.5f,  -0.5f,  -0.5f,
        -0.5f,  0.5f,   -0.5f,
        0.5f,   0.5f,   -0.5f,
        
        -0.5f,  0.5f,   0.5f,
        0.5f,   0.5f,   0.5f,
        0.5f,   0.5f,   -0.5f,
        -0.5f,  0.5f,   -0.5f,
        
        -0.5f,  -0.5f,  -0.5f,
        0.5f,   -0.5f,  -0.5f,
        0.5f,   -0.5f,  0.5f,
        -0.5f,  -0.5f,  0.5f,
    };
    unsigned int indicesData_lighting[] = {
        0,  1,  2,
        3,  0,  2,
        
        8,  9,  11,
        9,  10, 11,
        
        4,  5,  7,
        5,  6,  7,
        
        12,  15,  14,
        12,  14,  13,
        
        1,  4,  2,
        2,  4,  7,
        
        0,  6,  5,
        0,  3,  6,
    };
    float verticesData_cube[] = {
		//pos					//color					//normal				//texture coord
        -0.5f,  -0.5f,  -0.5f,  1.0f,   1.0f,   1.0f,   0.0f,   0.0f,   -1.0f,	0.0f,  0.0f,
        0.5f,   -0.5f,  -0.5f,  1.0f,   1.0f,   1.0f,   0.0f,   0.0f,   -1.0f,	1.0f,  0.0f,
        0.5f,   0.5f,   -0.5f,  1.0f,   1.0f,   1.0f,   0.0f,   0.0f,   -1.0f,	1.0f,  1.0f,
        0.5f,   0.5f,   -0.5f,  1.0f,   1.0f,   1.0f,   0.0f,   0.0f,   -1.0f,	1.0f,  1.0f,
        -0.5f,  0.5f,   -0.5f,  1.0f,   1.0f,   1.0f,   0.0f,   0.0f,   -1.0f,	0.0f,  1.0f,
        -0.5f,  -0.5f,  -0.5f,  1.0f,   1.0f,   1.0f,   0.0f,   0.0f,   -1.0f,	0.0f,  0.0f,
        
        -0.5f,  -0.5f,  0.5f,   1.0f,   1.0f,   1.0f,   0.0f,   0.0f,   1.0f,	0.0f,  0.0f,
        0.5f,   -0.5f,  0.5f,   1.0f,   1.0f,   1.0f,   0.0f,   0.0f,   1.0f,	1.0f,  0.0f,
        0.5f,   0.5f,   0.5f,   1.0f,   1.0f,   1.0f,   0.0f,   0.0f,   1.0f,	1.0f,  1.0f,
        0.5f,   0.5f,   0.5f,   1.0f,   1.0f,   1.0f,   0.0f,   0.0f,   1.0f,	1.0f,  1.0f,
        -0.5f,  0.5f,   0.5f,   1.0f,   1.0f,   1.0f,   0.0f,   0.0f,   1.0f,	0.0f,  1.0f,
        -0.5f,  -0.5f,  0.5f,   1.0f,   1.0f,   1.0f,   0.0f,   0.0f,   1.0f,	0.0f,  0.0f,
		
        -0.5f,  0.5f,   0.5f,   1.0f,   1.0f,   1.0f,   -1.0f,  0.0f,   0.0f,	1.0f,  0.0f,
        -0.5f,  0.5f,   -0.5f,  1.0f,   1.0f,   1.0f,   -1.0f,  0.0f,   0.0f,	1.0f,  1.0f,
        -0.5f,  -0.5f,  -0.5f,  1.0f,   1.0f,   1.0f,   -1.0f,  0.0f,   0.0f,	0.0f,  1.0f,
        -0.5f,  -0.5f,  -0.5f,  1.0f,   1.0f,   1.0f,   -1.0f,  0.0f,   0.0f,	0.0f,  1.0f,
        -0.5f,  -0.5f,  0.5f,   1.0f,   1.0f,   1.0f,   -1.0f,  0.0f,   0.0f,	0.0f,  0.0f,
        -0.5f,  0.5f,   0.5f,   1.0f,   1.0f,   1.0f,   -1.0f,  0.0f,   0.0f,	1.0f,  0.0f,
        
        0.5f,   0.5f,   0.5f,   1.0f,   1.0f,   1.0f,   1.0f,   0.0f,   0.0f,	1.0f,  0.0f,
        0.5f,   0.5f,   -0.5f,  1.0f,   1.0f,   1.0f,   1.0f,   0.0f,   0.0f,	1.0f,  1.0f,
        0.5f,   -0.5f,  -0.5f,  1.0f,   1.0f,   1.0f,   1.0f,   0.0f,   0.0f,	0.0f,  1.0f,
        0.5f,   -0.5f,  -0.5f,  1.0f,   1.0f,   1.0f,   1.0f,   0.0f,   0.0f,	0.0f,  1.0f,
        0.5f,   -0.5f,  0.5f,   1.0f,   1.0f,   1.0f,   1.0f,   0.0f,   0.0f,	0.0f,  0.0f,
        0.5f,   0.5f,   0.5f,   1.0f,   1.0f,   1.0f,   1.0f,   0.0f,   0.0f,	1.0f,  0.0f,
        
        -0.5f,  -0.5f,  -0.5f,  1.0f,   1.0f,   1.0f,   0.0f,   -1.0f,  0.0f,	0.0f,  1.0f,
        0.5f,   -0.5f,  -0.5f,  1.0f,   1.0f,   1.0f,   0.0f,   -1.0f,  0.0f,	1.0f,  1.0f,
        0.5f,   -0.5f,  0.5f,   1.0f,   1.0f,   1.0f,   0.0f,   -1.0f,  0.0f,	1.0f,  0.0f,
        0.5f,   -0.5f,  0.5f,   1.0f,   1.0f,   1.0f,   0.0f,   -1.0f,  0.0f,	1.0f,  0.0f,
        -0.5f,  -0.5f,  0.5f,   1.0f,   1.0f,   1.0f,   0.0f,   -1.0f,  0.0f,	0.0f,  0.0f,
        -0.5f,  -0.5f,  -0.5f,  1.0f,   1.0f,   1.0f,   0.0f,   -1.0f,  0.0f,	0.0f,  1.0f,
        
        -0.5f,  0.5f,   -0.5f,  1.0f,   1.0f,   1.0f,   0.0f,   1.0f,   0.0f,	0.0f,  1.0f,
        0.5f,   0.5f,   -0.5f,  1.0f,   1.0f,   1.0f,   0.0f,   1.0f,   0.0f,	1.0f,  1.0f,
        0.5f,   0.5f,   0.5f,   1.0f,   1.0f,   1.0f,   0.0f,   1.0f,   0.0f,	1.0f,  0.0f,
        0.5f,   0.5f,   0.5f,   1.0f,   1.0f,   1.0f,   0.0f,   1.0f,   0.0f,	1.0f,  0.0f,
        -0.5f,  0.5f,   0.5f,   1.0f,   1.0f,   1.0f,   0.0f,   1.0f,   0.0f,	0.0f,  0.0f,
        -0.5f,  0.5f,   -0.5f,  1.0f,   1.0f,   1.0f,   0.0f,   1.0f,   0.0f,	0.0f,  1.0f,
    };
	
	//openGL的核心模式要求使用vao的，不然单用vbo是渲染不出东西的
    
    unsigned int vao_axis;
    unsigned int vbo_axis;
    
    glGenBuffers(1, &vbo_axis);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_axis);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData_axis), verticesData_axis, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenVertexArrays(1, &vao_axis);
    glBindVertexArray(vao_axis);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_axis);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    unsigned int vao_plane;
    unsigned int vbo_plane;
    unsigned int ebo_plane;
    
    glGenBuffers(1, &vbo_plane);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_plane);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData_plane), verticesData_plane, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenBuffers(1, &ebo_plane);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_plane);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesData_plane), indicesData_plane, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    glGenVertexArrays(1, &vao_plane);
    glBindVertexArray(vao_plane);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_plane);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_plane);
    glBindVertexArray(0);
    
    unsigned int vao_plane_fbo;
    unsigned int vbo_plane_fbo;
    
    glGenBuffers(1, &vbo_plane_fbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_plane_fbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData_plane2), verticesData_plane2, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenVertexArrays(1, &vao_plane_fbo);
    glBindVertexArray(vao_plane_fbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_plane_fbo);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
    
    unsigned int vao_bg;
    unsigned int vbo_bg;
    
    glGenBuffers(1, &vbo_bg);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_bg);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData_bg), verticesData_bg, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenVertexArrays(1, &vao_bg);
    glBindVertexArray(vao_bg);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_bg);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindVertexArray(0);
    
    unsigned int vao_lighting;
    unsigned int vbo_lighting;
    unsigned int ebo_lighting;
    
    glGenBuffers(1, &vbo_lighting);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_lighting);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData_lighting), verticesData_lighting, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenBuffers(1, &ebo_lighting);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_lighting);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesData_lighting), indicesData_lighting, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    glGenVertexArrays(1, &vao_lighting);
    glBindVertexArray(vao_lighting);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_lighting);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_lighting);
    glBindVertexArray(0);
    
    unsigned int vao_cube;
    unsigned int vbo_cube;
    auto pTexture_cube_diffuse = TextureMgr::getInstance()->addTexture("/Users/lam/Desktop/Workspace/TestGL/fbo/res/container2.png", Texture_Type::Diffuse);
    auto pTexture_cube_specular = TextureMgr::getInstance()->addTexture("/Users/lam/Desktop/Workspace/TestGL/fbo/res/container2_specular.png", Texture_Type::Specular);
    
    glGenBuffers(1, &vbo_cube);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_cube);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData_cube), verticesData_cube, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenVertexArrays(1, &vao_cube);
    glBindVertexArray(vao_cube);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_cube);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(9 * sizeof(float)));
    glBindVertexArray(0);
    
    unsigned int fbo;
    glGenFramebuffers(1, &fbo);
    
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    
    unsigned int textureFbo;
    glGenTextures(1, &textureFbo);
    glBindTexture(GL_TEXTURE_2D, textureFbo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Window_Width, Window_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Window_Width, Window_Height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    
    
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureFbo, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        glfwTerminate();
        return -1;
    }
    
    Model model = Model("/Users/lam/Desktop/Workspace/TestGL/fbo/res/nanosuit/nanosuit.obj");
    model.setPosition(glm::vec3(0.0f, 0.0f, -3.0f));
    model.setScale(glm::vec3(0.2f, 0.2f, 0.2f));
	
    shader_cube.use();
    shader_cube.uniform1i("material.texture_diffuse", 0);   //注意这里传的是int，应该是sampler2D只是一个int标识，并不是实际的纹理，是纹理单元
    shader_cube.uniform1i("material.texture_specular", 1);
	
    while(!glfwWindowShouldClose(window))
    {
        handleCameraMove();
        
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
        auto camera = CameraMgr::getInstance()->get("main");
        auto cameraPos = camera->getPosition();
        auto mat4_view = camera->getViewMatrix();
        auto mat4_projection = camera->getProjectionMatrix();
        
        int num_render = 1;
        int index_render = 1;
        if (nFboTestingType == 1)
        {
            num_render = 2;
        }
        
        while (index_render <= num_render)
        {
            if (nFboTestingType == 1)
            {
                glBindFramebuffer(GL_FRAMEBUFFER, fbo);
            }
            
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);     //记得clear并不是clear一次，默认帧缓冲要clear，自己创建的帧缓冲也要clear
            
            shader_texture.use();
            glm::mat4 mat4_model_fboPlane_scale = glm::scale(glm::mat4(1.0), glm::vec3(1.0 * Window_Width / Window_Height * 2.0, 1.0 * 2.0, 1.0));
            glm::mat4 mat4_model_fboPlane_rotate = glm::rotate(glm::mat4(1.0), (float)glm::radians(-45.0), glm::vec3(0.0, 1.0, 0.0));
            glm::mat4 mat4_model_fboPlane_translate = glm::translate(glm::mat4(1.0), glm::vec3(5.0, 2.0, -5.0));
            glm::mat4 mat4_model_fboPlane = mat4_model_fboPlane_translate * mat4_model_fboPlane_rotate * mat4_model_fboPlane_scale;
            shader_texture.uniformMatrix4fv("model", mat4_model_fboPlane);
            shader_texture.uniformMatrix4fv("view", mat4_view);
            shader_texture.uniformMatrix4fv("projection", mat4_projection);
            shader_texture.uniform1i("nEffect", nEffect);
            glBindVertexArray(vao_plane_fbo);
            glBindTexture(GL_TEXTURE_2D, textureFbo);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
            
            if (nStencilTestingType == 0)
            {
                glStencilFunc(GL_ALWAYS, 1, 0xFF);
                glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
                
                shader_stencil.use();
                shader_stencil.uniformMatrix4fv("model", glm::mat4(1.0f));
                shader_stencil.uniformMatrix4fv("view", mat4_view);
                shader_stencil.uniformMatrix4fv("projection", mat4_projection);
                glBindVertexArray(vao_plane);
                glDrawElements(GL_TRIANGLES, sizeof(indicesData_plane) / sizeof(float), GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);
            }
            else if (nStencilTestingType == 1)
            {
                glStencilMask(0xFF);
                glStencilFunc(GL_ALWAYS, 1, 0xFF);
                glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
                glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
                glDepthMask(GL_FALSE);
                
                shader_stencil.use();
                shader_stencil.uniformMatrix4fv("model", glm::mat4(1.0f));
                shader_stencil.uniformMatrix4fv("view", mat4_view);
                shader_stencil.uniformMatrix4fv("projection", mat4_projection);
                glBindVertexArray(vao_plane);
                glDrawElements(GL_TRIANGLES, sizeof(indicesData_plane) / sizeof(float), GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);
                
        //            glStencilMask(0x00);      //禁用模板写入，会让glClear也不起作用，应该说glClear也是一种写入操作，所以用keep操作保持值不变也是可行
                glStencilFunc(GL_EQUAL, 1, 0xFF);
                glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
                glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
                glDepthMask(GL_TRUE);
            }
            
            shader_axis.use();
            shader_axis.uniformMatrix4fv("model", glm::mat4(1.0f));
            shader_axis.uniformMatrix4fv("view", mat4_view);
            shader_axis.uniformMatrix4fv("projection", mat4_projection);
            glBindVertexArray(vao_axis);
            glDrawArrays(GL_LINE_STRIP, 0, sizeof(verticesData_axis) / sizeof(float));
            glBindVertexArray(0);
            
            shader_lighting.use();
            shader_lighting.uniformMatrix4fv("view", mat4_view);
            shader_lighting.uniformMatrix4fv("projection", mat4_projection);
            glBindVertexArray(vao_lighting);
            for (int i = 0; i < lightNum; i++)
            {
                glm::mat4 mat4_scale_lighting = glm::mat4(1.0f);
                glm::mat4 mat4_translate_lighting = glm::mat4(1.0f);
                mat4_scale_lighting = glm::scale(mat4_scale_lighting, glm::vec3(0.3f, 0.3f, 0.3f));
                mat4_translate_lighting = glm::translate(mat4_translate_lighting, lightArr[i].position);
                glm::mat4 mat4_model_lighting = mat4_translate_lighting * mat4_scale_lighting;
                shader_lighting.uniformMatrix4fv("model", mat4_model_lighting);
                glDrawElements(GL_TRIANGLES, sizeof(indicesData_lighting) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
            }
            glBindVertexArray(0);
            
            shader_bg.use();
            glm::mat4 mat4_rotate_bg = glm::mat4(1.0f);
            mat4_rotate_bg = glm::rotate(mat4_rotate_bg, (float)glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            glm::mat4 mat4_translate_bg = glm::mat4(1.0f);
            mat4_translate_bg = glm::translate(mat4_translate_bg, glm::vec3(0.0f, -1.0f, 0.0f));
            glm::mat4 mat4_scale_bg = glm::mat4(1.0f);
            mat4_scale_bg = glm::scale(mat4_scale_bg, glm::vec3(15.0f, 15.0f, 0.0f));
            glm::mat4 mat4_model_bg = mat4_translate_bg * mat4_rotate_bg * mat4_scale_bg;
            shader_bg.uniformMatrix4fv("model", mat4_model_bg);
            shader_bg.uniformMatrix4fv("view", mat4_view);
            shader_bg.uniformMatrix4fv("projection", mat4_projection);
            shader_bg.uniform3fv("eyePos", cameraPos);
            shader_bg.uniform3fv("material.ambient", glm::vec3(0.05f,0.0f,0.0f));
            shader_bg.uniform3fv("material.diffuse", glm::vec3(0.5f,0.4f,0.4f));
            shader_bg.uniform3fv("material.specular", glm::vec3(0.7f,0.04f,0.04f));
            shader_bg.uniform1f("material.shininess", 10.0f);
            for (int i = 0; i < lightNum; i++)
            {
                char buf[50];
                memset(buf, 0, sizeof(buf));
                sprintf(buf, "lightArr[%d].position", i);
                shader_bg.uniform3fv(buf, lightArr[i].position);
                memset(buf, 0, sizeof(buf));
                sprintf(buf, "lightArr[%d].direction", i);
                shader_bg.uniform3fv(buf, lightArr[i].direction);
                memset(buf, 0, sizeof(buf));
                sprintf(buf, "lightArr[%d].ambient", i);
                shader_bg.uniform3fv(buf, lightArr[i].colors[0]);
                memset(buf, 0, sizeof(buf));
                sprintf(buf, "lightArr[%d].diffuse", i);
                shader_bg.uniform3fv(buf, lightArr[i].colors[1]);
                memset(buf, 0, sizeof(buf));
                sprintf(buf, "lightArr[%d].specular", i);
                shader_bg.uniform3fv(buf, lightArr[i].colors[2]);
                memset(buf, 0, sizeof(buf));
                sprintf(buf, "lightArr[%d].constant", i);
                shader_bg.uniform1f(buf, lightArr[i].constant);
                memset(buf, 0, sizeof(buf));
                sprintf(buf, "lightArr[%d].linear", i);
                shader_bg.uniform1f(buf, lightArr[i].linear);
                memset(buf, 0, sizeof(buf));
                sprintf(buf, "lightArr[%d].quadratic", i);
                shader_bg.uniform1f(buf, lightArr[i].quadratic);
                memset(buf, 0, sizeof(buf));
                sprintf(buf, "lightArr[%d].cutOff", i);
                shader_bg.uniform1f(buf, lightArr[i].cutOff);
                memset(buf, 0, sizeof(buf));
                sprintf(buf, "lightArr[%d].outerCutOff", i);
                shader_bg.uniform1f(buf, lightArr[i].outerCutOff);
            }
            glBindVertexArray(vao_bg);
            glDrawArrays(GL_TRIANGLES, 0, sizeof(verticesData_bg) / (sizeof(float) * 9));
            glBindVertexArray(0);
            
            shader_cube.use();
            shader_cube.uniformMatrix4fv("view", mat4_view);
            shader_cube.uniformMatrix4fv("projection", mat4_projection);
            shader_cube.uniform3fv("eyePos", cameraPos);
            shader_cube.uniform3fv("material.ambient", glm::vec3(1.0f,1.0f,1.0f));
            shader_cube.uniform3fv("material.diffuse", glm::vec3(1.0f,1.0f,1.0f));
            shader_cube.uniform3fv("material.specular", glm::vec3(1.0f,1.0f,1.0f));
            shader_cube.uniform1f("material.shininess", 16.0f);
            for (int i = 0; i < cubeNum; i++)
            {
                char buf[50];
                memset(buf, 0, sizeof(buf));
                sprintf(buf, "lightArr[%d].position", i);
                shader_cube.uniform3fv(buf, lightArr[i].position);
                memset(buf, 0, sizeof(buf));
                sprintf(buf, "lightArr[%d].direction", i);
                shader_cube.uniform3fv(buf, lightArr[i].direction);
                memset(buf, 0, sizeof(buf));
                sprintf(buf, "lightArr[%d].ambient", i);
                shader_cube.uniform3fv(buf, lightArr[i].colors[0]);
                memset(buf, 0, sizeof(buf));
                sprintf(buf, "lightArr[%d].diffuse", i);
                shader_cube.uniform3fv(buf, lightArr[i].colors[1]);
                memset(buf, 0, sizeof(buf));
                sprintf(buf, "lightArr[%d].specular", i);
                shader_cube.uniform3fv(buf, lightArr[i].colors[2]);
                memset(buf, 0, sizeof(buf));
                sprintf(buf, "lightArr[%d].constant", i);
                shader_cube.uniform1f(buf, lightArr[i].constant);   //衰减值，http://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
                memset(buf, 0, sizeof(buf));
                sprintf(buf, "lightArr[%d].linear", i);
                shader_cube.uniform1f(buf, lightArr[i].linear);
                memset(buf, 0, sizeof(buf));
                sprintf(buf, "lightArr[%d].quadratic", i);
                shader_cube.uniform1f(buf, lightArr[i].quadratic);
                memset(buf, 0, sizeof(buf));
                sprintf(buf, "lightArr[%d].cutOff", i);
                shader_cube.uniform1f(buf, lightArr[i].cutOff);
                memset(buf, 0, sizeof(buf));
                sprintf(buf, "lightArr[%d].outerCutOff", i);
                shader_cube.uniform1f(buf, lightArr[i].outerCutOff);
            }
            std::vector<glm::mat4> arr_mat4_model_cube;
            for (int i = 0; i < cubeNum; i++)
            {
                glm::mat4 mat4_translate_cube = glm::mat4(1.0f);
                glm::mat4 mat4_rotate_cube = glm::mat4(1.0f);
                mat4_translate_cube = glm::translate(mat4_translate_cube, cubePosArr[i]);
                mat4_rotate_cube = glm::rotate(mat4_rotate_cube, (float)glfwGetTime() + cubeRadiansArr[i], glm::vec3(1.0f, 0.0f, 0.0f));
                mat4_rotate_cube = glm::rotate(mat4_rotate_cube, (float)glfwGetTime() + cubeRadiansArr[i] + 10, glm::vec3(0.0f, 1.0f, 0.0f));
                mat4_rotate_cube = glm::rotate(mat4_rotate_cube, (float)glfwGetTime() + cubeRadiansArr[i] + 100, glm::vec3(0.0f, 0.0f, 1.0f));
                glm::mat4 mat4_model_cube = mat4_translate_cube * mat4_rotate_cube;
                arr_mat4_model_cube.push_back(mat4_model_cube);
            }
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, pTexture_cube_diffuse->getId());
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, pTexture_cube_specular->getId());
            glBindVertexArray(vao_cube);
            for (int i = 0; i < cubeNum; i++)
            {
                glm::mat4 mat4_model_cube = arr_mat4_model_cube[i];
                shader_cube.uniformMatrix4fv("model", mat4_model_cube);
                if (nStencilTestingType == 2)
                {
                    glStencilMask(0xFF);
                    glStencilFunc(GL_ALWAYS, 1, 0xFF);
                    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
                }
                glDrawArrays(GL_TRIANGLES, 0, sizeof(verticesData_cube) / (sizeof(float) * 9));
            }
            glBindVertexArray(0);
            if (nStencilTestingType == 2)
            {
                shader_color.use();
                shader_color.uniformMatrix4fv("view", mat4_view);
                shader_color.uniformMatrix4fv("projection", mat4_projection);
                shader_color.uniform3fv("color", glm::vec3(0.0f, 1.0f, 0.0f));
                glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
                glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
                glDepthMask(GL_FALSE);
                glBindVertexArray(vao_cube);
                for (int i = 0; i < cubeNum; i++)
                {
                    glm::mat4 tMat4_scale = glm::mat4(1.0f);
                    tMat4_scale = glm::scale(tMat4_scale, glm::vec3(1.05f, 1.05f, 1.05f));
                    glm::mat4 mat4_model_cube = arr_mat4_model_cube[i] * tMat4_scale;
                    shader_color.uniformMatrix4fv("model", mat4_model_cube);
                    glDrawArrays(GL_TRIANGLES, 0, sizeof(verticesData_cube) / (sizeof(float) * 9));
                }
                glBindVertexArray(0);
                glStencilFunc(GL_ALWAYS, 1, 0xFF);
                glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
                glDepthMask(GL_TRUE);
            }
            
            model.draw(shader_model);
            /*
            glm::vec3 rotation_model = model.getRotation();
            rotation_model.y = (float)glfwGetTime();
            model.setRotaion(rotation_model);
             */
            
            if (nFboTestingType == 1)
            {
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                nFboTestingType = 0;
            }
            index_render++;
        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &vao_bg);
    glDeleteBuffers(1, &vbo_bg);
    glDeleteVertexArrays(1, &vao_lighting);
    glDeleteBuffers(1, &vbo_lighting);
    glDeleteBuffers(1, &ebo_lighting);
    glDeleteVertexArrays(1, &vao_cube);
    glDeleteBuffers(1, &vbo_cube);
    glDeleteVertexArrays(1, &vao_plane);
    glDeleteBuffers(1, &vbo_plane);
    glDeleteBuffers(1, &ebo_plane);
    shader_bg.cleanup();
    shader_lighting.cleanup();
    shader_cube.cleanup();
    shader_axis.cleanup();
    shader_model.cleanup();
    shader_stencil.cleanup();
    glfwTerminate();
    
    return 0;
}

bool init()
{
//    int arr[] = {0, 2, 9};        //地址一样的喔
//    std::cout << arr << std::endl;
//    std::cout << &arr[0] << std::endl;
    
//    int nMaxTextureNum = 0;     //取最大支持纹理数
//    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &nMaxTextureNum);
//    std::cout << nMaxTextureNum << std::endl;
    
//    stbi_set_flip_vertically_on_load(true);
    
    updatePolygonMode();
    
    CameraMgr::getInstance()->add("main", glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0));
    
    return true;
}

void handleCameraMove()
{
    auto camera = CameraMgr::getInstance()->get("main");
    if (map_keyPress[GLFW_KEY_W])
    {
        auto tPos = camera->getPosition() + (cameraSpeed * camera->getDirection());
        camera->setPosition(tPos);
    }
    else if (map_keyPress[GLFW_KEY_S])
    {
        auto tPos = camera->getPosition() - (cameraSpeed * camera->getDirection());
        camera->setPosition(tPos);
    }
    else if (map_keyPress[GLFW_KEY_A])
    {
        auto tPos = camera->getPosition() - (cameraSpeed * camera->getRight());
        camera->setPosition(tPos);
    }
    else if (map_keyPress[GLFW_KEY_D])
    {
        auto tPos = camera->getPosition() + (cameraSpeed * camera->getRight());
        camera->setPosition(tPos);
    }
}

void callback_keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    std::cout << "===callback_keyboard===begin" << std::endl;
    std::cout << "key:" << key << " " << "scancode:" << scancode << " " << "action:" << action << " " << "mods:" << mods << std::endl;
    std::cout << "===callback_keyboard===end" << std::endl;
    
    if (action == GLFW_PRESS)
    {
        //按下
        map_keyPress[key] = true;
    }
    else if (action == GLFW_REPEAT)
    {
        //按住
    }
    else if (action == GLFW_RELEASE)
    {
        //松开
        if (key == GLFW_KEY_ESCAPE)
        {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        else if (key == GLFW_KEY_F1)
        {
            g_polygonMode = g_polygonMode == PolygonMode::Fill ? PolygonMode::Line : PolygonMode::Fill;
            updatePolygonMode();
        }
        else if (key == GLFW_KEY_F2)
        {
            for (int i = 0; i < lightNum; i++)
            {
                lightArr[i].colors[0] = glm::vec3(random_0_1(), random_0_1(), random_0_1());
                lightArr[i].colors[1] = glm::vec3(random_0_1(), random_0_1(), random_0_1());
                lightArr[i].colors[2] = glm::vec3(random_0_1(), random_0_1(), random_0_1());
            }
        }
        else if (key == GLFW_KEY_F3)
        {
            nStencilTestingType = 0;
        }
        else if (key == GLFW_KEY_F4)
        {
            nStencilTestingType = 1;
        }
        else if (key == GLFW_KEY_F5)
        {
            nStencilTestingType = 2;
        }
        else if (key == GLFW_KEY_F6)
        {
            nFboTestingType = 1;
        }
        else if (key == GLFW_KEY_KP_0)
        {
            nEffect = 0;
        }
        else if (key == GLFW_KEY_KP_1)
        {
            nEffect = 1;
        }
        else if (key == GLFW_KEY_KP_2)
        {
            nEffect = 2;
        }
        else if (key == GLFW_KEY_KP_3)
        {
            nEffect = 3;
        }
        else if (key == GLFW_KEY_KP_4)
        {
            nEffect = 4;
        }
        else if (key == GLFW_KEY_KP_5)
        {
            nEffect = 5;
        }
        else if (key == GLFW_KEY_KP_6)
        {
            nEffect = 6;
        }
        else if (key == GLFW_KEY_SPACE)
        {
            std::cout << random_0_1() << " " << random_0_1() << " " << random_0_1() << std::endl;
        }
        
        map_keyPress[key] = false;
    }
}

void callback_mouseButton(GLFWwindow* window, int button, int action, int mods)
{
    std::cout << "===callback_mouseButton===begin" << std::endl;
    std::cout << "button:" << button << " action:" << action << " mods:" << mods << std::endl;
    std::cout << "===callback_mouseButton===end" << std::endl;
    
    if (action == GLFW_PRESS)
    {
        pos_mouseR_moved.x = -1.0f;
        pos_mouseR_moved.y = -1.0f;
        map_mousePress[button] = true;
    }
    else if (action == GLFW_RELEASE)
    {
        pos_mouseR_moved.x = -1.0f;
        pos_mouseR_moved.y = -1.0f;
        map_mousePress[button] = false;
    }
}

void callback_mousePos(GLFWwindow* window, double posX, double posY)
{
    //    std::cout << posX << "   " << posY << std::endl;
    if (map_mousePress[GLFW_MOUSE_BUTTON_RIGHT])
    {
        std::cout << posX << "   " << posY << std::endl;
        
        if (pos_mouseR_moved.x < 0 || pos_mouseR_moved.y < 0)
        {
            pos_mouseR_moved.x = posX;
            pos_mouseR_moved.y = posY;
        }
        else
        {
            auto camera = CameraMgr::getInstance()->get("main");
            glm::vec2 offset;
            offset.x = posX - pos_mouseR_moved.x;
            //            offset.y = posY - pos_mouseR_moved.y;
            offset.y = pos_mouseR_moved.y - posY;
            pos_mouseR_moved.x = posX;
            pos_mouseR_moved.y = posY;
            offset.x = offset.x * factor_mouseR_moved;
            offset.y = offset.y * factor_mouseR_moved;
            auto yaw = camera->getYaw() + offset.x;
            auto pitch = camera->getPitch() + offset.y;
            camera->setRotation(yaw, pitch);
        }
    }
}

void updatePolygonMode()
{
    glPolygonMode(GL_FRONT_AND_BACK, g_polygonMode == PolygonMode::Fill ? GL_FILL : GL_LINE);
}