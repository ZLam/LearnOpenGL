#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 textureCoord;
layout (location = 3) in vec3 normal;

out vec3 extern_color;
out vec2 extern_textureCoord;
out vec3 extern_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	extern_color = color;
	extern_textureCoord = textureCoord;
	extern_normal = mat3(transpose(inverse(model))) * normal;
	gl_Position = projection * view * model * vec4(position, 1.0f);
}