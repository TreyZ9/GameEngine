#version 400 core

in vec2 textureCoords_fs;

out vec4 FragColor;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, textureCoords_fs).r);
	FragColor = vec4(textColor.rgb, 1.0) * sampled;
}