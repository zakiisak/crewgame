#version 330 core

layout(location = 0) in vec3 cubeVertices;
layout(location = 1) in vec3 position;
layout(location = 2) in vec4 color;
layout(location = 3) in vec4 uv;


out vec2 UV;
out vec4 cubeColor;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;


void main()
{
	gl_Position = projectionMatrix * viewMatrix * vec4(cubeVertices + position, 1.0f);

	UV = cubeVertices.xy + vec2(0.5, 0.5);
	if(UV.x == 0.0) {UV.x = uv.x;}
	if(UV.y == 0.0) {UV.y = uv.y;}
	if(UV.x == 1.0) {UV.x = uv.z;}
	if(UV.y == 1.0) {UV.y = uv.w;}
	
	cubeColor = color;
}

