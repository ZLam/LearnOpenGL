#version 330 core

out vec4 fragColor;

uniform float near;
uniform float far;
uniform int testingType;

float getLinearDepth();

void main()
{
	float depth = gl_FragCoord.z;
	if (testingType == 1)
	{
		depth = getLinearDepth();		
	}
	fragColor = vec4(vec3(depth), 1.0f);
}

float getLinearDepth()
{
	float z_real = (far * near) / (gl_FragCoord.z * (near - far) + far);
	return (z_real - near) / (far - near);
}