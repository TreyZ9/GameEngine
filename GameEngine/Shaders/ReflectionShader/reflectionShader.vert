#version 450 core

layout (location = 0) in vec3 position_vs;
layout (location = 1) in vec2 textureCoords_vs;
layout (location = 2) in vec3 normal_vs;
layout (location = 3) in vec3 tangent_vs;
layout (location = 4) in vec3 bitangent_vs;

out vec2 textureCoords_fs;
out vec3 surfaceNormal_fs;
out vec3 fragmentPosition_fs;
out vec3 tangentLightPos_fs[4];
out vec3 tangentViewPos_fs;
out vec3 tangentFragPos_fs;
out vec3 reflectNormal_fs;

uniform mat4 transformationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform vec3 lightPositions[4];
uniform vec3 cameraPosition;

void main(void) {
	fragmentPosition_fs = vec3(transformationMatrix * vec4(position_vs, 1.0));
	textureCoords_fs = textureCoords_vs;

	mat3 normalMatrix = transpose(inverse(mat3(transformationMatrix)));
    vec3 T = normalize(normalMatrix * tangent_vs);
    vec3 N = normalize(normalMatrix * normal_vs);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    mat3 TBN = transpose(mat3(T, B, N));

    surfaceNormal_fs = normalMatrix * normal_vs;
      
    for (int i = 0; i < 4; i++)
    {
        tangentLightPos_fs[i] = TBN * lightPositions[i];
    }
    tangentViewPos_fs  = TBN * cameraPosition;
    tangentFragPos_fs  = TBN * fragmentPosition_fs;

    reflectNormal_fs = mat3(transpose(inverse(transformationMatrix))) * normal_vs;

    gl_Position = projectionMatrix * viewMatrix * transformationMatrix * vec4(position_vs.xyz, 1.0);
}