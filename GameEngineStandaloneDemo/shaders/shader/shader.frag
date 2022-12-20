#version 450 core

in vec2 textureCoords_fs;
in vec3 surfaceNormal_fs;

out vec4 FragColor;

uniform sampler2D texture_diffuse0;
// uniform sampler2D texture_normal0;
// uniform sampler2D texture_specular0;

void main(void) {
	vec4 textureColor = texture(texture_diffuse0, textureCoords_fs);
	if (textureColor.a < 0.1)
		discard;
	FragColor = textureColor;
}
