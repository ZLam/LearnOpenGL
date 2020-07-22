#version 330 core

in vec3 outColor;
in vec2 outTexCoord;

out vec4 FragColor;

uniform float fAlpha;
uniform sampler2D mTexture;

void main()
{
	FragColor = texture(mTexture, outTexCoord) * vec4(outColor, fAlpha);
}