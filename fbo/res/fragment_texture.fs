#version 330 core

in vec2 textureCoord_io;

out vec4 fragmentColor;

uniform sampler2D smp2D;
uniform int nEffect;

const float offset = 1.0f / 300.0f;

void main()
{
	if (nEffect == 1)
	{
		//反相
		fragmentColor = vec4((1.0f - vec3(texture(smp2D, textureCoord_io))), 1.0f);
	}
	else if (nEffect == 2 || nEffect == 3)
	{
		vec3 tColor = vec3(texture(smp2D, textureCoord_io));
		float avg = 1.0f;
		if (nEffect == 2)
		{
			//灰度化
			avg = tColor.r + tColor.g + tColor.b / 3.0f;
		}
		else
		{
			//带权重灰度化
			avg = 0.2126f * tColor.r + 0.7152f * tColor.g + 0.0722f * tColor.b / 3.0f;
		}
		fragmentColor = vec4(avg, avg, avg, 1.0f);
	}
	else if (nEffect == 4 || nEffect == 5 || nEffect == 6)
	{
		//4：核效果，5：模糊，6：边缘检测
		vec2 offsetArr[9] = vec2[](
			vec2(-offset, offset),
			vec2(0.0f, offset),
			vec2(offset, offset),
			vec2(-offset, 0.0f),
			vec2(0.0f, 0.0f),
			vec2(offset, 0.0f),
			vec2(-offset, -offset),
			vec2(0.0f, -offset),
			vec2(offset, -offset)
		);
		float kernel[9];
		if (nEffect == 4)
		{
			kernel = float[](
				-1.0f, -1.0f, -1.0f,
				-1.0f, 9.0f, -1.0f,
				-1.0f, -1.0f, -1.0f
			);
		}
		else if (nEffect == 5)
		{
			kernel = float[](
				1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
				2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
				1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
			);
		}
		else
		{
			kernel = float[](
				1.0f, 1.0f, 1.0f,
				1.0f, -8.0f, 1.0f,
				1.0f, 1.0f, 1.0f
			);
		}
		vec3 colorArr[9];
		vec3 tColor = vec3(0.0f);
		for (int i = 0; i < 9; i++)
		{
			colorArr[i] = vec3(texture(smp2D, textureCoord_io.st + offsetArr[i]));
		}
		for (int i = 0; i < 9; i++)
		{
			tColor += kernel[i] * colorArr[i];
		}
		fragmentColor = vec4(tColor, 1.0f);
	}
	else
	{
		//正常
		fragmentColor = vec4(vec3(texture(smp2D, textureCoord_io)), 1.0);
	}
}