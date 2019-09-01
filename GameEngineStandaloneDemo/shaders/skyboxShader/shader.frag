#version 450 core

in vec3 textureCoords_fs;

out vec4 FragColor;

uniform samplerCube texture_cubeMap0;

void main(void) {
	FragColor = texture(texture_cubeMap0, textureCoords_fs);
}
