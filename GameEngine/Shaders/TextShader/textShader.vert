#version 400 core

layout (location = 0) in vec4 vertex_vs;

out vec2 textureCoords_fs;

uniform mat4 transformationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main(void) {
	vec4 worldPosition = transformationMatrix * vec4(vertex_vs.xy, 0.0, 1.0);
	gl_Position = projectionMatrix * viewMatrix * worldPosition;
	textureCoords_fs = vertex_vs.zw;
}