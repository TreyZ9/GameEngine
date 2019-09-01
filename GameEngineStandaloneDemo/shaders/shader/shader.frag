#version 450 core

in vec2 textureCoords_fs;
in vec3 surfaceNormal_fs;

out vec4 FragColor;

uniform sampler2D texture_diffuse0;

void main(void) {
	FragColor = texture(texture_diffuse0, textureCoords_fs);
}
