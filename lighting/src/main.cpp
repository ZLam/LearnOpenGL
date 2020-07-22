//
//  main.cpp
//  lighting
//
//  Created by lam on 18/8/16.
//  Copyright © 2018年 lam. All rights reserved.
//

#include <iostream>
#include <vector>
#include <unordered_map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Define.h"

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

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 cameraDir = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0);
float cameraSpeed = 0.2f;
std::unordered_map<int, bool> map_keyPress;
std::unordered_map<int, bool> map_mousePress;
glm::vec2 pos_mouseR_moved = glm::vec2(-1.0f, -1.0f);
float factor_mouseR_moved = 0.05;
float pitch = 0.0f;
float yaw = -90.0f;
glm::vec3 lightingPos = glm::vec3(0.5f, 3.0f, 0.5f);
glm::vec3 lightingDir = glm::vec3(-1.0f, -1.0f, 1.0f);
//glm::vec3 lightingDir = glm::vec3(0.0f, 0.0f, 0.0f);
//glm::vec3 lightingDir = glm::vec3(0.0f, -1.0f, 0.0f);
//float lightingCutOff = glm::cos(glm::radians(15.0f));
float lightingCutOff = 0.0f;
//float lightingOuterCutOff = glm::cos(glm::radians(18.0f));
float lightingOuterCutOff = 0.0f;
glm::vec3 lightingColor = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 lightingColors[3] = {
	glm::vec3(0.2f, 0.2f, 0.2f),	//ambient
	glm::vec3(1.0f, 1.0f, 1.0f),	//diffuse
	glm::vec3(1.0f, 1.0f, 1.0f)		//specular
};
const int cubeNum = 3;
glm::vec3 cubePosArr[cubeNum] = {
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(2.0f, 0.0f, 2.0f),
	glm::vec3(-2.0f, 2.0f, -2.0f),
};
float cubeRadiansArr[cubeNum] = {
	2 * glm::pi<float>() * getRandom_0_1(),
	2 * glm::pi<float>() * getRandom_0_1(),
	2 * glm::pi<float>() * getRandom_0_1(),
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

bool check_shader_compile_status(GLuint obj);
bool check_program_link_status(GLuint obj);
unsigned int getShader(const std::string& fullPath, GLenum shaderType);
unsigned int getShaderProgram(const std::string& fullPath_vertex, const std::string& fullPath_fragment);
unsigned int loadTexture(char const * fullPath);
void updatePolygonMode();
void callback_keyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
void callback_mouseButton(GLFWwindow* window, int button, int action, int mods);
void callback_mousePos(GLFWwindow* window, double posX, double posY);
bool init();
void handleCameraMove();

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
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    updatePolygonMode();
    stbi_set_flip_vertically_on_load(true);
    init();
    
    unsigned int shaderProgram_bg = getShaderProgram("/Users/zlam/Workspace/LearnOpenGL/lighting/res/vertex_bg.vs", "/Users/zlam/Workspace/LearnOpenGL/lighting/res/fragment_bg.fs");
    unsigned int shaderProgram_lighting = getShaderProgram("/Users/zlam/Workspace/LearnOpenGL/lighting/res/vertex_lighting.vs", "/Users/zlam/Workspace/LearnOpenGL/lighting/res/fragment_lighting.fs");
    unsigned int shaderProgram_cube = getShaderProgram("/Users/zlam/Workspace/LearnOpenGL/lighting/res/vertex_cube.vs", "/Users/zlam/Workspace/LearnOpenGL/lighting/res/fragment_cube.fs");
	if (shaderProgram_lighting == -1 || shaderProgram_lighting == -1 || shaderProgram_cube == -1)
	{
		glfwTerminate();
		return -1;
	}
	
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
	unsigned int hTexture_cube = loadTexture("/Users/zlam/Workspace/LearnOpenGL/lighting/res/container2.png");
	unsigned int hTexture_cube_specular = loadTexture("/Users/zlam/Workspace/LearnOpenGL/lighting/res/container2_specular.png");
    
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
    
    int index_uniform_model_lighting = glGetUniformLocation(shaderProgram_lighting, "model");
    int index_uniform_view_lighting = glGetUniformLocation(shaderProgram_lighting, "view");
    int index_uniform_projection_lighting = glGetUniformLocation(shaderProgram_lighting, "projection");
	
	int index_uniform_model_bg = glGetUniformLocation(shaderProgram_bg, "model");
	int index_uniform_view_bg = glGetUniformLocation(shaderProgram_bg, "view");
	int index_uniform_projection_bg = glGetUniformLocation(shaderProgram_bg, "projection");
//	    int index_uniform_lightingColor_bg = glGetUniformLocation(shaderProgram_bg, "lightingColor");
//	    int index_uniform_lightingPos_bg = glGetUniformLocation(shaderProgram_bg, "lightingPos");
	int index_uniform_eyePos_bg = glGetUniformLocation(shaderProgram_bg, "eyePos");
	int index_uniform_material_ambient_bg = glGetUniformLocation(shaderProgram_bg, "material.ambient");
	int index_uniform_material_diffuse_bg = glGetUniformLocation(shaderProgram_bg, "material.diffuse");
	int index_uniform_material_specular_bg = glGetUniformLocation(shaderProgram_bg, "material.specular");
	int index_uniform_material_shininess_bg = glGetUniformLocation(shaderProgram_bg, "material.shininess");
//	int index_uniform_light_pos_bg = glGetUniformLocation(shaderProgram_bg, "light.position");
//	int index_uniform_light_dir_bg = glGetUniformLocation(shaderProgram_bg, "light.direction");
//	int index_uniform_light_ambient_bg = glGetUniformLocation(shaderProgram_bg, "light.ambient");
//	int index_uniform_light_diffuse_bg = glGetUniformLocation(shaderProgram_bg, "light.diffuse");
//	int index_uniform_light_specular_bg = glGetUniformLocation(shaderProgram_bg, "light.specular");
//	int index_uniform_light_constant_bg = glGetUniformLocation(shaderProgram_bg, "light.constant");
//	int index_uniform_light_linear_bg = glGetUniformLocation(shaderProgram_bg, "light.linear");
//	int index_uniform_light_quadratic_bg = glGetUniformLocation(shaderProgram_bg, "light.quadratic");
//	int index_uniform_light_cutOff_bg = glGetUniformLocation(shaderProgram_bg, "light.cutOff");
//	int index_uniform_light_outerCutOff_bg = glGetUniformLocation(shaderProgram_bg, "light.outerCutOff");
	
	std::vector<std::vector<int>> arr_index_uniform_light_bg(lightNum);
	for (int i = 0; i < arr_index_uniform_light_bg.size(); i++)
	{
		char buf[50];
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "lightArr[%d].position", i);
		arr_index_uniform_light_bg[i].push_back(glGetUniformLocation(shaderProgram_bg, buf));
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "lightArr[%d].direction", i);
		arr_index_uniform_light_bg[i].push_back(glGetUniformLocation(shaderProgram_bg, buf));
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "lightArr[%d].ambient", i);
		arr_index_uniform_light_bg[i].push_back(glGetUniformLocation(shaderProgram_bg, buf));
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "lightArr[%d].diffuse", i);
		arr_index_uniform_light_bg[i].push_back(glGetUniformLocation(shaderProgram_bg, buf));
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "lightArr[%d].specular", i);
		arr_index_uniform_light_bg[i].push_back(glGetUniformLocation(shaderProgram_bg, buf));
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "lightArr[%d].constant", i);
		arr_index_uniform_light_bg[i].push_back(glGetUniformLocation(shaderProgram_bg, buf));
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "lightArr[%d].linear", i);
		arr_index_uniform_light_bg[i].push_back(glGetUniformLocation(shaderProgram_bg, buf));
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "lightArr[%d].quadratic", i);
		arr_index_uniform_light_bg[i].push_back(glGetUniformLocation(shaderProgram_bg, buf));
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "lightArr[%d].cutOff", i);
		arr_index_uniform_light_bg[i].push_back(glGetUniformLocation(shaderProgram_bg, buf));
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "lightArr[%d].outerCutOff", i);
		arr_index_uniform_light_bg[i].push_back(glGetUniformLocation(shaderProgram_bg, buf));
	}
	
	
    int index_uniform_model_cube = glGetUniformLocation(shaderProgram_cube, "model");
    int index_uniform_view_cube = glGetUniformLocation(shaderProgram_cube, "view");
    int index_uniform_projection_cube = glGetUniformLocation(shaderProgram_cube, "projection");
//    int index_uniform_lightingColor_cube = glGetUniformLocation(shaderProgram_cube, "lightingColor");
//    int index_uniform_lightingPos_cube = glGetUniformLocation(shaderProgram_cube, "lightingPos");
	int index_uniform_eyePos_cube = glGetUniformLocation(shaderProgram_cube, "eyePos");
	int index_uniform_material_ambient_cube = glGetUniformLocation(shaderProgram_cube, "material.ambient");
	int index_uniform_material_diffuse_cube = glGetUniformLocation(shaderProgram_cube, "material.diffuse");
	int index_uniform_material_specular_cube = glGetUniformLocation(shaderProgram_cube, "material.specular");
	int index_uniform_material_shininess_cube = glGetUniformLocation(shaderProgram_cube, "material.shininess");
	int index_uniform_texture_ambient_cube = glGetUniformLocation(shaderProgram_cube, "material.texture_ambient");
	int index_uniform_texture_specular_cube = glGetUniformLocation(shaderProgram_cube, "material.texture_specular");
//	int index_uniform_light_pos_cube = glGetUniformLocation(shaderProgram_cube, "light.position");
//	int index_uniform_light_dir_cube = glGetUniformLocation(shaderProgram_cube, "light.direction");
//	int index_uniform_light_ambient_cube = glGetUniformLocation(shaderProgram_cube, "light.ambient");
//	int index_uniform_light_diffuse_cube = glGetUniformLocation(shaderProgram_cube, "light.diffuse");
//	int index_uniform_light_specular_cube = glGetUniformLocation(shaderProgram_cube, "light.specular");
//	int index_uniform_light_constant_cube = glGetUniformLocation(shaderProgram_cube, "light.constant");
//	int index_uniform_light_linear_cube = glGetUniformLocation(shaderProgram_cube, "light.linear");
//	int index_uniform_light_quadratic_cube = glGetUniformLocation(shaderProgram_cube, "light.quadratic");
//	int index_uniform_light_cutOff_cube = glGetUniformLocation(shaderProgram_cube, "light.cutOff");
//	int index_uniform_light_outerCutOff_cube = glGetUniformLocation(shaderProgram_cube, "light.outerCutOff");
	
	std::vector<std::vector<int>> arr_index_uniform_light_cube(cubeNum);
	for (int i = 0; i < arr_index_uniform_light_cube.size(); i++)
	{
//		arr_index_uniform_light_cube[i].resize(10);
		char buf[50];
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "lightArr[%d].position", i);
		arr_index_uniform_light_cube[i].push_back(glGetUniformLocation(shaderProgram_cube, buf));
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "lightArr[%d].direction", i);
		arr_index_uniform_light_cube[i].push_back(glGetUniformLocation(shaderProgram_cube, buf));
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "lightArr[%d].ambient", i);
		arr_index_uniform_light_cube[i].push_back(glGetUniformLocation(shaderProgram_cube, buf));
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "lightArr[%d].diffuse", i);
		arr_index_uniform_light_cube[i].push_back(glGetUniformLocation(shaderProgram_cube, buf));
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "lightArr[%d].specular", i);
		arr_index_uniform_light_cube[i].push_back(glGetUniformLocation(shaderProgram_cube, buf));
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "lightArr[%d].constant", i);
		arr_index_uniform_light_cube[i].push_back(glGetUniformLocation(shaderProgram_cube, buf));
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "lightArr[%d].linear", i);
		arr_index_uniform_light_cube[i].push_back(glGetUniformLocation(shaderProgram_cube, buf));
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "lightArr[%d].quadratic", i);
		arr_index_uniform_light_cube[i].push_back(glGetUniformLocation(shaderProgram_cube, buf));
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "lightArr[%d].cutOff", i);
		arr_index_uniform_light_cube[i].push_back(glGetUniformLocation(shaderProgram_cube, buf));
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "lightArr[%d].outerCutOff", i);
		arr_index_uniform_light_cube[i].push_back(glGetUniformLocation(shaderProgram_cube, buf));
	}
	
	glUseProgram(shaderProgram_cube);
	glUniform1i(index_uniform_texture_ambient_cube, 0);			//注意这里传的是int，应该是sampler2D只是一个int标识，并不是实际的纹理，是纹理单元
	glUniform1i(index_uniform_texture_specular_cube, 1);
	
    while(!glfwWindowShouldClose(window))
    {
        handleCameraMove();
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glm::mat4 mat4_view = glm::lookAt(cameraPos, cameraPos + cameraDir, cameraUp);
        glm::mat4 mat4_projection = glm::perspective(glm::radians(45.0f), (float)Window_Width / (float)Window_Height, 0.1f, 100.0f);
        
        glUseProgram(shaderProgram_lighting);
        glUniformMatrix4fv(index_uniform_view_lighting, 1, GL_FALSE, glm::value_ptr(mat4_view));
        glUniformMatrix4fv(index_uniform_projection_lighting, 1, GL_FALSE, glm::value_ptr(mat4_projection));
        glBindVertexArray(vao_lighting);
		for (int i = 0; i < lightNum; i++)
		{
			glm::mat4 mat4_scale_lighting = glm::mat4(1.0f);
			glm::mat4 mat4_translate_lighting = glm::mat4(1.0f);
			mat4_scale_lighting = glm::scale(mat4_scale_lighting, glm::vec3(0.3f, 0.3f, 0.3f));
			mat4_translate_lighting = glm::translate(mat4_translate_lighting, lightArr[i].position);
			glm::mat4 mat4_model_lighting = mat4_translate_lighting * mat4_scale_lighting;
			glUniformMatrix4fv(index_uniform_model_lighting, 1, GL_FALSE, glm::value_ptr(mat4_model_lighting));
			glDrawElements(GL_TRIANGLES, sizeof(indicesData_lighting) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
		}
        glBindVertexArray(0);
        
        glUseProgram(shaderProgram_bg);
        glm::mat4 mat4_rotate_bg = glm::mat4(1.0f);
        mat4_rotate_bg = glm::rotate(mat4_rotate_bg, (float)glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 mat4_translate_bg = glm::mat4(1.0f);
        mat4_translate_bg = glm::translate(mat4_translate_bg, glm::vec3(0.0f, -1.0f, 0.0f));
        glm::mat4 mat4_scale_bg = glm::mat4(1.0f);
        mat4_scale_bg = glm::scale(mat4_scale_bg, glm::vec3(15.0f, 15.0f, 0.0f));
        glm::mat4 mat4_model_bg = mat4_translate_bg * mat4_rotate_bg * mat4_scale_bg;
        glUniformMatrix4fv(index_uniform_model_bg, 1, GL_FALSE, glm::value_ptr(mat4_model_bg));
        glUniformMatrix4fv(index_uniform_view_bg, 1, GL_FALSE, glm::value_ptr(mat4_view));
        glUniformMatrix4fv(index_uniform_projection_bg, 1, GL_FALSE, glm::value_ptr(mat4_projection));
//        glUniform3fv(index_uniform_lightingColor_bg, 1, glm::value_ptr(lightingColor));
//        glUniform3fv(index_uniform_lightingPos_bg, 1, glm::value_ptr(lightingPos));
        glUniform3fv(index_uniform_eyePos_bg, 1, glm::value_ptr(cameraPos));
		glUniform3fv(index_uniform_material_ambient_bg, 1, glm::value_ptr(glm::vec3(0.05f,0.0f,0.0f)));
		glUniform3fv(index_uniform_material_diffuse_bg, 1, glm::value_ptr(glm::vec3(0.5f,0.4f,0.4f)));
		glUniform3fv(index_uniform_material_specular_bg, 1, glm::value_ptr(glm::vec3(0.7f,0.04f,0.04f)));
		glUniform1f(index_uniform_material_shininess_bg, 10.0f);
		for (int i = 0; i < lightNum; i++)
		{
			glUniform3fv(arr_index_uniform_light_bg[i][0], 1, glm::value_ptr(lightArr[i].position));
			glUniform3fv(arr_index_uniform_light_bg[i][1], 1, glm::value_ptr(lightArr[i].direction));
			glUniform3fv(arr_index_uniform_light_bg[i][2], 1, glm::value_ptr(lightArr[i].colors[0]));
			glUniform3fv(arr_index_uniform_light_bg[i][3], 1, glm::value_ptr(lightArr[i].colors[1]));
			glUniform3fv(arr_index_uniform_light_bg[i][4], 1, glm::value_ptr(lightArr[i].colors[2]));
			glUniform1f(arr_index_uniform_light_bg[i][5], lightArr[i].constant);
			glUniform1f(arr_index_uniform_light_bg[i][6], lightArr[i].linear);
			glUniform1f(arr_index_uniform_light_bg[i][7], lightArr[i].quadratic);
			glUniform1f(arr_index_uniform_light_bg[i][8], lightArr[i].cutOff);
			glUniform1f(arr_index_uniform_light_bg[i][9], lightArr[i].outerCutOff);
		}
        //        glBindTexture(GL_TEXTURE_2D, hTexture);
        glBindVertexArray(vao_bg);
        //        glDrawElements(GL_TRIANGLES, sizeof(indicesData_bg) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(verticesData_bg) / (sizeof(float) * 9));
        glBindVertexArray(0);
		
        glUseProgram(shaderProgram_cube);
        glUniformMatrix4fv(index_uniform_view_cube, 1, GL_FALSE, glm::value_ptr(mat4_view));
        glUniformMatrix4fv(index_uniform_projection_cube, 1, GL_FALSE, glm::value_ptr(mat4_projection));
//        glUniform3fv(index_uniform_lightingColor_cube, 1, glm::value_ptr(lightingColor));
//        glUniform3fv(index_uniform_lightingPos_cube, 1, glm::value_ptr(lightingPos));
		glUniform3fv(index_uniform_eyePos_cube, 1, glm::value_ptr(cameraPos));
		//
//		glUniform3fv(index_uniform_material_ambient_cube, 1, glm::value_ptr(glm::vec3(0.0f,0.1f,0.06f)));
//		glUniform3fv(index_uniform_material_diffuse_cube, 1, glm::value_ptr(glm::vec3(0.0f,0.50980392f,0.50980392f)));
//		glUniform3fv(index_uniform_material_specular_cube, 1, glm::value_ptr(glm::vec3(0.50196078f,0.50196078f,0.50196078f)));
		//
		//
		glUniform3fv(index_uniform_material_ambient_cube, 1, glm::value_ptr(glm::vec3(1.0f,1.0f,1.0f)));
		glUniform3fv(index_uniform_material_diffuse_cube, 1, glm::value_ptr(glm::vec3(1.0f,1.0f,1.0f)));
		glUniform3fv(index_uniform_material_specular_cube, 1, glm::value_ptr(glm::vec3(1.0f,1.0f,1.0f)));
		//
		glUniform1f(index_uniform_material_shininess_cube, 16.0f);
		for (int i = 0; i < cubeNum; i++)
		{
			glUniform3fv(arr_index_uniform_light_cube[i][0], 1, glm::value_ptr(lightArr[i].position));
			glUniform3fv(arr_index_uniform_light_cube[i][1], 1, glm::value_ptr(lightArr[i].direction));
			glUniform3fv(arr_index_uniform_light_cube[i][2], 1, glm::value_ptr(lightArr[i].colors[0]));
			glUniform3fv(arr_index_uniform_light_cube[i][3], 1, glm::value_ptr(lightArr[i].colors[1]));
			glUniform3fv(arr_index_uniform_light_cube[i][4], 1, glm::value_ptr(lightArr[i].colors[2]));
			glUniform1f(arr_index_uniform_light_cube[i][5], lightArr[i].constant);		//衰减值，http://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
			glUniform1f(arr_index_uniform_light_cube[i][6], lightArr[i].linear);
			glUniform1f(arr_index_uniform_light_cube[i][7], lightArr[i].quadratic);
			glUniform1f(arr_index_uniform_light_cube[i][8], lightArr[i].cutOff);
			glUniform1f(arr_index_uniform_light_cube[i][9], lightArr[i].outerCutOff);
		}
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, hTexture_cube);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, hTexture_cube_specular);
        glBindVertexArray(vao_cube);
//        glDrawElements(GL_TRIANGLES, sizeof(indicesData_cube) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
		for (int i = 0; i < cubeNum; i++)
		{
			glm::mat4 mat4_translate_cube = glm::mat4(1.0f);
			glm::mat4 mat4_rotate_cube = glm::mat4(1.0f);
			mat4_translate_cube = glm::translate(mat4_translate_cube, cubePosArr[i]);
			mat4_rotate_cube = glm::rotate(mat4_rotate_cube, (float)glfwGetTime() + cubeRadiansArr[i], glm::vec3(1.0f, 0.0f, 0.0f));
			mat4_rotate_cube = glm::rotate(mat4_rotate_cube, (float)glfwGetTime() + cubeRadiansArr[i] + 10, glm::vec3(0.0f, 1.0f, 0.0f));
			mat4_rotate_cube = glm::rotate(mat4_rotate_cube, (float)glfwGetTime() + cubeRadiansArr[i] + 100, glm::vec3(0.0f, 0.0f, 1.0f));
			glm::mat4 mat4_model_cube = mat4_translate_cube * mat4_rotate_cube;
			glUniformMatrix4fv(index_uniform_model_cube, 1, GL_FALSE, glm::value_ptr(mat4_model_cube));
			glDrawArrays(GL_TRIANGLES, 0, sizeof(verticesData_cube) / (sizeof(float) * 9));
		}
        glBindVertexArray(0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &vao_bg);
    glDeleteBuffers(1, &vbo_bg);
    glDeleteProgram(shaderProgram_bg);
    glDeleteVertexArrays(1, &vao_lighting);
    glDeleteBuffers(1, &vbo_lighting);
    glDeleteBuffers(1, &ebo_lighting);
    glDeleteProgram(shaderProgram_lighting);
    glDeleteVertexArrays(1, &vao_cube);
    glDeleteBuffers(1, &vbo_cube);
    glDeleteProgram(shaderProgram_cube);
    glfwTerminate();
    
    return 0;
}

bool init()
{
    
    //    glm::vec4 v = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    //    glm::mat4 trans = glm::mat4(1.0f);
    //    trans = glm::translate(trans, glm::vec3(1.0f, 0.0f, 0.0f));
    //    v = trans * v;
    
    return true;
}

void handleCameraMove()
{
    if (map_keyPress[GLFW_KEY_W])
    {
        cameraPos += cameraSpeed * cameraDir;
    }
    else if (map_keyPress[GLFW_KEY_S])
    {
        cameraPos -= cameraSpeed * cameraDir;
    }
    else if (map_keyPress[GLFW_KEY_A])
    {
        glm::vec3 v = glm::cross(cameraDir, cameraUp);
        v = glm::normalize(v);
        cameraPos -= cameraSpeed * v;
    }
    else if (map_keyPress[GLFW_KEY_D])
    {
        glm::vec3 v = glm::cross(cameraDir, cameraUp);
        v = glm::normalize(v);
        cameraPos += cameraSpeed * v;
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
            std::cout << getRandom_0_1() << "  " << getRandom_0_1() << "  " << getRandom_0_1() << std::endl;
            lightingColor = glm::vec3(getRandom_0_1(), getRandom_0_1(), getRandom_0_1());
			lightingColors[0] = glm::vec3(getRandom_0_1(), getRandom_0_1(), getRandom_0_1());
			lightingColors[1] = glm::vec3(getRandom_0_1(), getRandom_0_1(), getRandom_0_1());
			lightingColors[2] = glm::vec3(getRandom_0_1(), getRandom_0_1(), getRandom_0_1());
        }
        else if (key == GLFW_KEY_SPACE)
        {
//            std::cout << myRandom.min() << "  " << myRandom.max() << std::endl;
//            std::cout << myRandom() << std::endl;
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
            glm::vec2 offset;
            offset.x = posX - pos_mouseR_moved.x;
            //            offset.y = posY - pos_mouseR_moved.y;
            offset.y = pos_mouseR_moved.y - posY;
            pos_mouseR_moved.x = posX;
            pos_mouseR_moved.y = posY;
            offset.x = offset.x * factor_mouseR_moved;
            offset.y = offset.y * factor_mouseR_moved;
            yaw += offset.x;
            pitch += offset.y;
            pitch = pitch > 89.0f ? 89.0f : pitch;
            pitch = pitch < -89.0f ? -89.0f : pitch;
            
            glm::vec3 dir;
            dir.x = glm::cos(glm::radians(pitch)) * glm::cos(glm::radians(yaw));
            dir.y = glm::sin(glm::radians(pitch));
            dir.z = glm::cos(glm::radians(pitch)) * glm::sin(glm::radians(yaw));
            cameraDir = glm::normalize(dir);
        }
    }
}

void updatePolygonMode()
{
    glPolygonMode(GL_FRONT_AND_BACK, g_polygonMode == PolygonMode::Fill ? GL_FILL : GL_LINE);
}

unsigned int getShader(const std::string& fullPath, GLenum shaderType)
{
    unsigned int nShader;
    nShader = glCreateShader(shaderType);
    std::string str_shaderSource = getStringFromFile(fullPath.c_str());
    const char* c_shaderSource = str_shaderSource.c_str();
    glShaderSource(nShader, 1, &c_shaderSource, nullptr);
    glCompileShader(nShader);
    if (!check_shader_compile_status(nShader))
    {
        return -1;
    }
    return nShader;
}

unsigned int getShaderProgram(const std::string& fullPath_vertex, const std::string& fullPath_fragment)
{
    unsigned int nShader_vertex = getShader(fullPath_vertex, GL_VERTEX_SHADER);
    if (nShader_vertex == -1)
    {
        return -1;
    }
    unsigned int nShader_fragment = getShader(fullPath_fragment, GL_FRAGMENT_SHADER);
    if (nShader_fragment == -1)
    {
        return -1;
    }
    unsigned int nShaderProgram;
    nShaderProgram = glCreateProgram();
    glAttachShader(nShaderProgram, nShader_vertex);
    glAttachShader(nShaderProgram, nShader_fragment);
    glLinkProgram(nShaderProgram);
    if (!check_program_link_status(nShaderProgram))
    {
        return -1;
    }
    glDeleteShader(nShader_vertex);
    glDeleteShader(nShader_fragment);
    return  nShaderProgram;
}

unsigned int loadTexture(char const * fullPath)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	
	int width, height, nrComponents;
	unsigned char *data = stbi_load(fullPath, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;
		
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << fullPath << std::endl;
		stbi_image_free(data);
	}
	
	return textureID;
}

bool check_shader_compile_status(GLuint obj)
{
    GLint status;
    glGetShaderiv(obj, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE)
    {
        GLint length;
        glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> log(length);
        glGetShaderInfoLog(obj, length, &length, &log[0]);
        std::cerr << &log[0];
        return false;
    }
    return true;
}

bool check_program_link_status(GLuint obj) {
    GLint status;
    glGetProgramiv(obj, GL_LINK_STATUS, &status);
    if(status == GL_FALSE)
    {
        GLint length;
        glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> log(length);
        glGetProgramInfoLog(obj, length, &length, &log[0]);
        std::cerr << &log[0];
        return false;
    }
    return true;
}
