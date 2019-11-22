#version 450 core

in FS_Data{
	vec4 pos;
	vec3 normal;
	vec2 UV;
	vec4 color;
} fs_data;

void main(void)
{
	gl_FragColor = fs_data.color;
}