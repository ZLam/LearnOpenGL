#version 330 core

layout (location = 0) in vec3 vPos_i;
layout (location = 1) in vec3 vColor_i;
layout (location = 2) in vec3 vNormal_i;
layout (location = 3) in vec2 vTextureCoord_i;

out vec3 vColor_io;
out vec3 vNormal_io;
out vec3 vFragmentPos_o;
out vec2 vTextureCoord_io;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vColor_io = vColor_i;
	vTextureCoord_io = vTextureCoord_i;
	vNormal_io = mat3(transpose(inverse(model))) * vNormal_i;
	vFragmentPos_o = vec3(model * vec4(vPos_i, 1.0));
	gl_Position = projection * view * model * vec4(vPos_i, 1.0);
}