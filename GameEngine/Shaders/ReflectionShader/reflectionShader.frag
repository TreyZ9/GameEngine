#version 450 core

in vec2 textureCoords_fs;
in vec3 surfaceNormal_fs;
in vec3 fragmentPosition_fs;
in vec3 tangentLightPos_fs[4];
in vec3 tangentViewPos_fs;
in vec3 tangentFragPos_fs;
in vec3 reflectNormal_fs;

out vec4 FragColor;

uniform sampler2D texture_diffuse0;
uniform sampler2D texture_normal0;
uniform sampler2D texture_specular0;
uniform sampler2D texture_displacement0;
uniform samplerCube texture_cubeMap0;

uniform bool diffuseBound;
uniform bool normalBound;
uniform bool specularBound;
uniform bool displacementBound;
uniform bool cubeMapBound;

uniform vec3 materialKa; // Ambient
uniform vec3 materialKd; // Diffuse
uniform vec3 materialKs; // Specular
uniform vec3 materialKe; // Emission
uniform float materialNi; // Optical Index
uniform float materialD; // Alpha
uniform int materialIllum; // Illumination Info

uniform vec3 lightColors[4];
uniform vec3 cameraPosition;

void main(void) {
	float transparency = materialD;
	float refractiveIndex = 1.00f / 1.52f;
	float refractivity = 1.0f;
	float ambientFactor = 0.1f;
	float normalSmoothing = 0.1f;


	// sample texture
	vec4 color;
	if (diffuseBound) {
		color = texture(texture_diffuse0, textureCoords_fs);
	} else {
		color = vec4(materialKd.r, materialKd.g, materialKd.b, materialD);
	}

	// discard transparent
	if (color.a < 0.1) {
		discard;
	}

	// normal mapping
	if (normalBound) {
		vec3 normal = texture(texture_normal0, textureCoords_fs).xyz;
		normal = normalize(normal * 2.0 - 1.0);
		vec3 ambient = ambientFactor * color.rgb;

		vec3 lightDir = normalize(tangentLightPos_fs[0] - tangentFragPos_fs);
		float diff = max(dot(lightDir, normal), 0.0f);
		vec3 diffuse = diff * color.rgb * lightColors[0];

		vec3 viewDir = normalize(tangentViewPos_fs - tangentFragPos_fs);
		vec3 reflectDir = reflect(-lightDir, normal);
		vec3 halfwayDir = normalize(lightDir + viewDir);
		
		float spec = pow(max(dot(normal, halfwayDir), 0.0f), 32.0f);
		vec3 specular;
		if (specularBound) {
			specular = spec * vec3(texture(texture_specular0, textureCoords_fs));
		}
		else {
			specular = vec3(0.5f) * spec;
		}

		// cube map reflectivity/refractivity
		vec4 cubeMapColor = vec4(1.0f);
		if (cubeMapBound) {
			vec3 reflectNormalVector = normalize(reflectNormal_fs + normalSmoothing * normal);
			vec3 I = normalize(fragmentPosition_fs - cameraPosition);

			vec3 R = reflect(I, normalize(reflectNormalVector));
			vec4 cubeMapReflectColor = texture(texture_cubeMap0, normalize(R));

			R = refract(I, normalize(reflectNormalVector), refractiveIndex);
			vec4 cubeMapRefractColor = texture(texture_cubeMap0, normalize(R));

			//refractivity = dot(viewDir, reflectNormalVector);
			cubeMapColor = mix(cubeMapReflectColor, cubeMapRefractColor, refractivity);
			color = mix(color, cubeMapColor, transparency);
		}
		else {
			color = vec4(ambient + diffuse + specular, 1.0f);
		}
	}

	FragColor = color;
}