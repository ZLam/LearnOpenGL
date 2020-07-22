#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec3 extern_color = color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	extern_color = color;
	gl_Position = projection * view * model * vec4(position, 1.0f);
}