#version 450 core

layout (location = 0) in vec3 position_vs;
layout (location = 1) in vec2 textureCoords_vs;
layout (location = 2) in vec3 normal_vs;
layout (location = 3) in vec3 tangent_vs;
layout (location = 4) in vec3 bitangent_vs;

out vec2 textureCoords_fs;
out vec3 surfaceNormal_fs;

uniform mat4 transformationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main(void) {
	vec4 worldPosition = transformationMatrix * vec4(position_vs.xyz, 1.0f);
	gl_Position = projectionMatrix * viewMatrix * worldPosition;
	
	textureCoords_fs = textureCoords_vs;

	surfaceNormal_fs = mat3(transpose(inverse(transformationMatrix))) * normal_vs;
}