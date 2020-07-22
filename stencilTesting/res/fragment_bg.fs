#version 330 core

#define lightNum 3

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light
{
	vec3 position;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;

	float cutOff;
	float outerCutOff;
};

in vec3 outColor;
in vec3 fragmentPos;
in vec3 vNormal;
// in vec2 outTexCoord;

out vec4 fragColor;

// uniform vec3 lightingPos;
// uniform vec3 lightingColor;
uniform vec3 eyePos;
uniform Material material;
uniform Light lightArr[lightNum];
// uniform sampler2D mTexture;

void main()
{
	vec3 result = vec3(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < lightNum; i++)
	{
		Light light = lightArr[i];
		int nLightingType;
		vec3 lightingDir;
		float distance_fragmentToLight;
		float fAttenuation;
		float fTheta;
		float fIntensity;
		vec3 ambient = vec3(0.0f, 0.0f, 0.0f);
		vec3 diffuse = vec3(0.0f, 0.0f, 0.0f);
		vec3 specular = vec3(0.0f, 0.0f, 0.0f);
		vec3 normal = normalize(vNormal);
		vec3 vEyeDir = normalize(eyePos - fragmentPos);
		if ((light.direction.x != 0.0f || light.direction.y != 0.0f || light.direction.z != 0.0f) && (light.cutOff == 0.0f))
		{
			//direction lighting
			lightingDir = normalize(-light.direction);
			distance_fragmentToLight = -1.0f;
			fAttenuation = 1.0f;
			fTheta = 0.0f;
			fIntensity = 1.0f;
			nLightingType = 2;
		}
		else
		{
			vec3 tV = light.position - fragmentPos;
			lightingDir = normalize(tV);
			distance_fragmentToLight = length(tV);
			fAttenuation = 1.0f / (light.constant + light.linear * distance_fragmentToLight + light.quadratic * distance_fragmentToLight * distance_fragmentToLight);
			if (light.cutOff == 0.0f)
			{
				//point lighting
				nLightingType = 1;
				fTheta = 0.0f;
				fIntensity = 1.0f;
			}
			else
			{
				//spot lighting
				nLightingType = 3;
				fTheta = dot(normalize(-light.direction), lightingDir);
				fIntensity = clamp((fTheta - light.outerCutOff) / (light.cutOff - light.outerCutOff), 0.0f, 1.0f);
			}
		}
		vec3 vReflect = reflect(-lightingDir, normal);

		//ambient
		// float ambientStrength = 0.1;
		// ambient = ambientStrength * lightingColor;
		// ambient = lightingColor * material.ambient;
		ambient = light.ambient * material.ambient;
		ambient *= fAttenuation;

		if ((nLightingType != 3) || (nLightingType == 3 && fTheta > light.outerCutOff))
		{
			//diffuse
			// vec3 lightingDir = normalize(lightingPos - fragmentPos);
			float diff = max(dot(normal, lightingDir), 0.0);
			// diffuse = diff * lightingColor;
			// diffuse = lightingColor * (diff * material.diffuse);
			diffuse = light.diffuse * (diff * material.diffuse);
			diffuse *= fAttenuation;
			diffuse *= fIntensity;

			//specular
			// float specularStrength = 0.2;
			// int nShininess = 8;
			// float spec = pow(max(dot(vEyeDir, vReflect), 0.0), nShininess);
			float spec = pow(max(dot(vEyeDir, vReflect), 0.0), material.shininess);
			// specular = specularStrength * spec * lightingColor;
			// specular = lightingColor * (spec * material.specular);
			specular = light.specular * (spec * material.specular);
			specular *= fAttenuation;
			specular *= fIntensity;
		}

		result += (ambient + diffuse + specular) * outColor;
		// vec3 result = ambient + diffuse + specular;
	}

	fragColor = vec4(result, 1.0);
}