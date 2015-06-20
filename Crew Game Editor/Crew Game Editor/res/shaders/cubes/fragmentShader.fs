#version 330 core

in vec2 UV;
in vec4 cubeColor;

out vec4 FragColor;

uniform sampler2D textureSampler;

void main(){
	//vec4 texColor = texture(textureSampler, UV);
	FragColor = cubeColor;//texColor * cubeColor;
}