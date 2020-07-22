#version 330 core

in vec3 extern_color;
in vec2 extern_textureCoord;
in vec3 extern_normal;

out vec4 fragColor;

uniform sampler2D texture_diffuse0;

void main()
{
	// fragColor = vec4(vec3(texture(texture_diffuse0, extern_textureCoord)) * extern_color, 1.0f);
	fragColor = texture(texture_diffuse0, extern_textureCoord);
}