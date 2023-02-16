
#version 450 core

layout (location = 0) in vec3 position_vs;
layout (location = 1) in vec2 textureCoords_vs;
layout (location = 2) in vec3 normal_vs;
layout (location = 3) in vec3 tangent_vs;
layout (location = 4) in vec3 bitangent_vs;

out vec3 worldVertexPosition_cs;
out vec2 worldTextureCoords_cs;
out vec3 worldNormal_cs;

uniform mat4 transformationMatrix;

void main()
{
	worldVertexPosition_cs = (transformationMatrix * vec4(position_vs, 1.0)).xyz;
	worldTextureCoords_cs = textureCoords_vs;
	worldNormal_cs = mat3(transpose(inverse(transformationMatrix))) * normal_vs;
}
