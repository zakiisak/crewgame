#version 330 core

layout(location = 0) in vec3 squareVertices;
layout(location = 1) in vec4 xyzs;
layout(location = 2) in vec4 color;
layout(location = 3) in vec4 uv;
layout(location = 4) in float texture;


out vec2 UV;
out vec4 particleColor;
out float tid;

uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;


void main()
{
	float particleSize = xyzs.w;
	vec3 particleCenter_wordspace = xyzs.xyz;
	
	vec3 vertexPosition_worldspace = 
		particleCenter_wordspace
		+ CameraRight_worldspace * squareVertices.x * particleSize
		+ CameraUp_worldspace * squareVertices.y * particleSize;

	gl_Position = projectionMatrix * viewMatrix * vec4(vertexPosition_worldspace, 1.0f);

	UV = squareVertices.xy + vec2(0.5, 0.5);
	if(UV.x == 0.0) {UV.x = uv.x;}
	if(UV.y == 0.0) {UV.y = uv.y;}
	if(UV.x == 1.0) {UV.x = uv.z;}
	if(UV.y == 1.0) {UV.y = uv.w;}
	
	
	particleColor = color;
	tid = texture;
}

