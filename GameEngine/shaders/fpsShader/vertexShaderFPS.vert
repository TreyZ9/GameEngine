#version 400 core

layout (location = 0) in vec4 vertex;

out vec2 textureCoords_fs;

uniform mat4 projectionMatrix;

void main(void) {
	gl_Position = projectionMatrix * vec4(vertex.xy, 0.0, 1.0);
	textureCoords_fs = vertex.zw;
}