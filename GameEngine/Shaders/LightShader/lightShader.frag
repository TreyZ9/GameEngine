
#version 450 core

in vec2 textureCoords_fs;
in vec3 surfaceNormal_fs;
in vec3 toLightVector_fs;

out vec4 FragColor;

uniform sampler2D texture_diffuse0;
uniform vec3 lightColor;
uniform float gamma;

void main(void) {
	vec3 unitNormal = normalize(surfaceNormal_fs);
	vec3 unitLightVector = normalize(toLightVector_fs);

	float nDotl = dot(unitNormal, unitLightVector);
	float brightness = max(nDotl, 0);
	vec3 diffuse = brightness * lightColor;

	FragColor = vec4(diffuse, 1.0) * texture(texture_diffuse0, textureCoords_fs);
	FragColor.rgb = pow(FragColor.rgb, vec3(1.0/gamma));
}
