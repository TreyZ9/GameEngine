
#version 450 core

layout(vertices = 3) out;

in vec3 worldVertexPosition_cs[];
in vec2 worldTextureCoords_cs[];
in vec3 worldNormal_cs[];

out vec3 worldVertexPosition_es[];
out vec2 worldTextureCoords_es[];
out vec3 worldNormal_es[];

uniform vec3 eyePos;

float getTessLevel(float distance0, float distance1)
{
	float avgDistance = (distance0 + distance1) / 2.0;
	avgDistance = (100 - avgDistance) / 20;
	if (avgDistance < 1) {
		avgDistance = 1;
	}
	return avgDistance;
}

void main()
{
	worldTextureCoords_es[gl_InvocationID] = worldTextureCoords_cs[gl_InvocationID];
	worldNormal_es[gl_InvocationID] = worldNormal_cs[gl_InvocationID];
	worldVertexPosition_es[gl_InvocationID] = worldVertexPosition_cs[gl_InvocationID];
	
	float eyeToVertexDistance0 = distance(eyePos, worldVertexPosition_es[0]);
	float eyeToVertexDistance1 = distance(eyePos, worldVertexPosition_es[1]);
	float eyeToVertexDistance2 = distance(eyePos, worldVertexPosition_es[2]);
	
	gl_TessLevelOuter[0] = getTessLevel(eyeToVertexDistance1, eyeToVertexDistance2);
	gl_TessLevelOuter[1] = getTessLevel(eyeToVertexDistance2, eyeToVertexDistance0);
	gl_TessLevelOuter[2] = getTessLevel(eyeToVertexDistance0, eyeToVertexDistance1);
	gl_TessLevelInner[0] = gl_TessLevelOuter[2];
}
