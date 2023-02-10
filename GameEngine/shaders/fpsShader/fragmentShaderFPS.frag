#version 400 core

in vec2 textureCoords_fs;

out vec4 FragColor;

uniform sampler2D text;

void main()
{
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, textureCoords_fs).r);
	FragColor = sampled;
}