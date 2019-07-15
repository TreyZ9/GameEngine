#version 400 core

in vec3 position_vs;
in vec2 textureCoords_vs;

out vec2 textureCoords_fs;

uniform mat4 transformationMatrix;

void main(void) {
	gl_Position = transformationMatrix * vec4(position_vs.xyz, 1.0f);
	textureCoords_fs = textureCoords_vs;
}