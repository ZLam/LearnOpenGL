#version 330 core

#define lightNum 3

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;

	sampler2D texture_diffuse;
	sampler2D texture_specular;
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

in vec3 vColor_io;
in vec3 vNormal_io;
in vec3 vFragmentPos_o;
in vec2 vTextureCoord_io;

out vec4 fragColor;

//uniform vec3 lightingPos;
//uniform vec3 lightingColor;
uniform vec3 eyePos;
uniform Material material;
uniform Light lightArr[lightNum];

void main()
{
	vec3 result = vec3(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < lightNum; i++)
	{
		Light light = lightArr[i];
		int nLightingType;
		float distance_fragmentToLight;
		float fAttenuation;			//衰减
		float fTheta;
		float fIntensity;			//强度
		vec3 vLightingDir;
		vec3 ambient = vec3(0.0f, 0.0f, 0.0f);
		vec3 diffuse = vec3(0.0f, 0.0f, 0.0f);
		vec3 specular = vec3(0.0f, 0.0f, 0.0f);
		vec3 normal = normalize(vNormal_io);
		vec3 vEyeDir = normalize(eyePos - vFragmentPos_o);
		if ((light.direction.x != 0.0f || light.direction.y != 0.0f || light.direction.z != 0.0f) && (light.cutOff == 0.0f))
		{
			//direction lighting
			vLightingDir = normalize(-light.direction);
			distance_fragmentToLight = -1.0f;
			fAttenuation = 1.0f;
			fTheta = 0.0f;
			fIntensity = 1.0f;
			nLightingType = 2;
		}
		else
		{
			vLightingDir = normalize(light.position - vFragmentPos_o);
			distance_fragmentToLight = length(light.position - vFragmentPos_o);
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
				fTheta = dot(vLightingDir, normalize(-light.direction));
				fIntensity = clamp((fTheta - light.outerCutOff) / (light.cutOff - light.outerCutOff), 0.0f, 1.0f);
			}
		}
		vec3 vReflect = reflect(-vLightingDir, normal);

		//ambient
		// float ambientStrength = 0.1;
		// ambient = ambientStrength * lightingColor;
		// ambient = material.ambient * light.ambient;
		ambient = (material.ambient * vec3(texture(material.texture_diffuse, vTextureCoord_io))) * light.ambient;
		ambient *= fAttenuation;

		if ((nLightingType != 3) || (nLightingType == 3 && fTheta > light.outerCutOff))
		{
			//diffuse
			float diff = max(dot(normal, vLightingDir), 0.0);
			// diffuse = (diff * material.diffuse) * light.diffuse;
			diffuse = (material.diffuse * vec3(texture(material.texture_diffuse, vTextureCoord_io)) * diff) * light.diffuse;
			diffuse *= fAttenuation;
			diffuse *= fIntensity;

			//specular
			// float specularStrength = 0.7;
			// int nShininess = 64;
			float spec = pow(max(dot(vEyeDir, vReflect), 0.0), material.shininess);
			// specular = specularStrength * spec * lightingColor;
			// specular = (spec * material.specular) * light.specular;
			specular = (spec * material.specular * vec3(texture(material.texture_specular, vTextureCoord_io))) * light.specular;
			specular *= fAttenuation;
			specular *= fIntensity;
		}

		result += (ambient + diffuse + specular) * vColor_io;
	}

	fragColor = vec4(result, 1.0);
}