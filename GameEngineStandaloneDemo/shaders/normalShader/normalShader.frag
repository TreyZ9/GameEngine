
#version 450 core

in vec3 fragPos_fs;
in vec2 texCoords_fs;
in vec3 tangentLightPos_fs;
in vec3 tangentViewPos_fs;
in vec3 tangentFragPos_fs;

out vec4 FragColor;

uniform sampler2D texture_diffuse0;
uniform sampler2D texture_normal0;

uniform vec3 lightColor;
uniform float gamma;

void main(void) {
	vec3 normal = texture(texture_normal0, texCoords_fs).rgb;
	normal = normalize(normal * 2.0 - 1.0);

	vec3 color = texture(texture_diffuse0, texCoords_fs).rgb;
	vec3 ambient = 0.1 * color;

	vec3 lightDir = normalize(tangentLightPos_fs - tangentFragPos_fs);
	float diff = max(dot(lightDir, normal), 0.0f);
	vec3 diffuse = diff * color * lightColor;

	vec3 viewDir = normalize(tangentViewPos_fs - tangentFragPos_fs);
	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0f), 32.0f);
	vec3 specular = vec3(0.2f) * spec;

	FragColor = vec4(ambient + diffuse + specular, 1.0f);
}
