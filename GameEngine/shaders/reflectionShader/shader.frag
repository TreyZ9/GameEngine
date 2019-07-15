
#version 450 core

in vec3 fragPos_fs;
in vec2 texCoords_fs;
in vec3 tangentLightPos_fs;
in vec3 tangentViewPos_fs;
in vec3 tangentFragPos_fs;
in vec3 reflectNormal_fs;

out vec4 FragColor;

uniform sampler2D texture_diffuse0;
uniform sampler2D texture_normal0;
uniform samplerCube texture_cubeMap0;
uniform sampler2D texture_specular0;

uniform bool useSpecularMap;
uniform vec3 cameraPosition;
uniform vec3 lightColor;
uniform float gamma;


void main(void) {
	float transparency = 1.0f;
	float refractiveIndex = 1.00f / 1.52f;
	float refractivity = 1.0f;
	float ambientFactor = 0.1f;
	float normalSmoothing = 0.1f;

	vec3 normalMapVector = texture(texture_normal0, texCoords_fs).rgb;
	normalMapVector = normalize(normalMapVector * 2.0 - 1.0);

	vec3 diffuseColor = texture(texture_diffuse0, texCoords_fs).rgb;
	vec3 ambientColor = ambientFactor * diffuseColor;

	vec3 lightDir = normalize(tangentLightPos_fs - tangentFragPos_fs);
	float diff = max(dot(lightDir, normalMapVector), 0.0f);
	diffuseColor = diff * diffuseColor * lightColor;

	vec3 viewDir = normalize(tangentViewPos_fs - tangentFragPos_fs);
	vec3 reflectDir = reflect(-lightDir, normalMapVector);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normalMapVector, halfwayDir), 0.0f), 32.0f);
	vec3 specularColor;
	if (useSpecularMap)
	{
		specularColor = spec * vec3(texture(texture_specular0, texCoords_fs));
	}
	else
	{
		specularColor = vec3(0.2f) * spec;
	}

	vec3 reflectNormalVector = normalize(reflectNormal_fs + normalSmoothing * normalMapVector);
	vec3 I = normalize(fragPos_fs - cameraPosition);

	vec3 R = reflect(I, normalize(reflectNormalVector));
	vec4 cubeMapReflectColor = texture(texture_cubeMap0, normalize(R));

	R = refract(I, normalize(reflectNormalVector), refractiveIndex);
	vec4 cubeMapRefractColor = texture(texture_cubeMap0, normalize(R));

	//refractivity = dot(viewDir, reflectNormalVector);
	vec4 cubeMapColor = mix(cubeMapReflectColor, cubeMapRefractColor, refractivity);

	vec4 modelColor = vec4(ambientColor + diffuseColor + specularColor * cubeMapColor.xyz, 1.0f);

	FragColor = mix(modelColor, cubeMapColor, transparency);
}
