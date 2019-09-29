#version 400 core

in vec2 textureCoords_fs;

out vec4 FragColor;

uniform sampler2D textureSampler;

void main(void) {
	vec4 color = texture(textureSampler, textureCoords_fs);
	if (color.r > 0.5 && color.b > 0.5 && color.g > 0.5) {
		discard;
	}
	FragColor = color;
}