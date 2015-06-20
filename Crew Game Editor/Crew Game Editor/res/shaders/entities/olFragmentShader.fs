#version 330 core

in float visibility;

layout (location = 0) out vec4 color;

uniform vec3 skyColor;

void main(void) {
	color = vec4(0, 0, 0, 1);//vec4(totalDiffuse, 1.0) * textureColor + vec4(totalSpecular, 1.0);
	color = mix(vec4(skyColor, 1.0), color, visibility);
}