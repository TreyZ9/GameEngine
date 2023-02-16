#version 450 core

in vec2 textureCoords_fs;
in vec3 surfaceNormal_fs;

out vec4 FragColor;

uniform sampler2D texture_diffuse0;
uniform sampler2D texture_normal0;
uniform sampler2D texture_specular0;
uniform sampler2D texture_displacement0;
uniform sampler2D texture_cubeMap0;

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

void main(void) {
	vec4 textureColor;

	if (diffuseBound) {
		textureColor = texture(texture_diffuse0, textureCoords_fs);
	} else {
		textureColor = vec4(materialKd.r, materialKd.g, materialKd.b, materialD);
	}

	if (textureColor.a < 0.1)
		discard;

	FragColor = textureColor;
}