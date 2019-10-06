
#version 450 core

in vec3 worldVertexPosition_fs;
in vec2 worldTextureCoords_fs;
in vec3 worldNormal_fs;

out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform float gamma;
uniform float blackPoint;

uniform sampler2D texture_diffuse0;
uniform sampler2D texture_displacement0;

void main()
{
	vec3 unitNormal = normalize(worldNormal_fs);
	vec3 unitLightVector = normalize(lightPosition - worldVertexPosition_fs);
	
	float dotResult = dot(unitNormal, unitLightVector);
	float brightness = max(dotResult, blackPoint);
	vec3 diffuse = brightness * lightColor;
	
    FragColor = vec4(diffuse, 1.0) * texture(texture_diffuse0, worldTextureCoords_fs);
	FragColor.rgb = pow(FragColor.rgb, vec3(1.0/gamma));
}