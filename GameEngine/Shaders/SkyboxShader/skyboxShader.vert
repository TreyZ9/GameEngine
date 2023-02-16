
#version 450 core

layout (location = 0) in vec3 position_vs;

out vec3 textureCoords_fs;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main(void) {
	gl_Position = projectionMatrix * viewMatrix * vec4(position_vs.xyz, 1.0f);
	textureCoords_fs = position_vs;
}