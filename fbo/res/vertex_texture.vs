#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 textureCoord;

out vec2 textureCoord_io;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	textureCoord_io = textureCoord;
	gl_Position = projection * view * model * vec4(pos, 1.0f);
	// gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);
}