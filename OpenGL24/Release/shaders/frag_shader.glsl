#version 330 core

in vec2 UV;

out vec3 FragColor;

uniform sampler2D myTextureSampler;

void main()
{
	FragColor = texture(myTextureSampler, UV).rgb;
}