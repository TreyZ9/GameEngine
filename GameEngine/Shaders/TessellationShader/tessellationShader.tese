
#version 450 core

layout(triangles, equal_spacing, ccw) in;

in vec3 worldVertexPosition_es[];
in vec2 worldTextureCoords_es[];
in vec3 worldNormal_es[];

out vec3 worldVertexPosition_fs;
out vec2 worldTextureCoords_fs;
out vec3 worldNormal_fs;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

uniform sampler2D texture_displacement0;

vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2)
{
	return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2;
}

vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2)
{
   	return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
}

void main()
{
	worldTextureCoords_fs = interpolate2D(worldTextureCoords_es[0], worldTextureCoords_es[1], worldTextureCoords_es[2]);
	worldNormal_fs = interpolate3D(worldNormal_es[0], worldNormal_es[1], worldNormal_es[2]);
	worldNormal_fs = normalize(worldNormal_fs);
	worldVertexPosition_fs = interpolate3D(worldVertexPosition_es[0], worldVertexPosition_es[1], worldVertexPosition_es[2]);
	
	float displacement = texture(texture_displacement0, worldTextureCoords_fs.xy).r * 0.5f;
	worldVertexPosition_fs += worldNormal_fs * displacement;
	
	gl_Position = projectionMatrix * viewMatrix * vec4(worldVertexPosition_fs.xyz, 1.0);
}
