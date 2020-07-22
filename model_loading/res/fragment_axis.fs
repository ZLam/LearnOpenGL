#version 330

in vec3 extern_color;

out vec4 fragColor;

void main()
{
	fragColor = vec4(extern_color, 1.0f);
}