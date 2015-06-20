#version 330 core

layout (location = 0) out vec4 color;

uniform vec4 colour;
uniform vec2 light_pos;
uniform float awesomeFont = 0.0f;

in DATA
{
	vec4 position;
	vec2 uv;
	float tid;
	vec4 color;
} fs_in;

uniform sampler2D textures[32];
 
void main(void)
{
	if(awesomeFont >= 0.5) {
		vec4 texColor;
		if (fs_in.tid > 0.0)
		{
			int tid = int(fs_in.tid - 0.5);
			texColor = texture(textures[tid], fs_in.uv);
		}

		texColor.x = fs_in.color.x;
		texColor.y = fs_in.color.y;
		texColor.z = fs_in.color.z;
		texColor.w *= fs_in.color.w;
		color = texColor;;
	}
	else {
		vec4 texColor = fs_in.color;
		if (fs_in.tid > 0.0)
		{
			int tid = int(fs_in.tid - 0.5);
			texColor = fs_in.color * texture(textures[tid], fs_in.uv);
		}
		color = texColor;
	}
}