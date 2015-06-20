#version 330 core

in vec2 UV;
in vec4 particleColor;
in float tid;

out vec4 FragColor;

uniform sampler2D textures[32];

void main(){
	vec4 texColor;
	if (tid > 0.0)
	{
		int textureID = int(tid - 0.5);
		texColor = texture(textures[textureID], UV);
	}
	else {
		texColor = vec4(1.0, 1.0, 1.0, 1.0);
	}
	FragColor = texColor * particleColor;

}