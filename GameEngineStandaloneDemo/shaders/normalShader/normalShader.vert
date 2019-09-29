
#version 450 core

layout (location = 0) in vec3 position_vs;
layout (location = 1) in vec2 textureCoords_vs;
layout (location = 2) in vec3 normal_vs;
layout (location = 3) in vec3 tangent_vs;
layout (location = 4) in vec3 bitangent_vs;

out vec3 fragPos_fs;
out vec2 texCoords_fs;
out vec3 tangentLightPos_fs;
out vec3 tangentViewPos_fs;
out vec3 tangentFragPos_fs;

uniform mat4 transformationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

uniform vec3 cameraPosition;
uniform vec3 lightPosition;

void main(void) {
	fragPos_fs = vec3(transformationMatrix * vec4(position_vs, 1.0));   
    texCoords_fs = textureCoords_vs;
    
    mat3 normalMatrix = transpose(inverse(mat3(transformationMatrix)));
    vec3 T = normalize(normalMatrix * tangent_vs);
    vec3 N = normalize(normalMatrix * normal_vs);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    
    mat3 TBN = transpose(mat3(T, B, N));    
    tangentLightPos_fs = TBN * lightPosition;
    tangentViewPos_fs  = TBN * cameraPosition;
    tangentFragPos_fs  = TBN * fragPos_fs;
        
    gl_Position = projectionMatrix * viewMatrix * transformationMatrix * vec4(position_vs, 1.0);
}
