#version 450 core

in FS_Data{
	vec4 pos;
	vec3 normal;
	vec2 UV;
} fs_data;

void main(void)
{
	gl_FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
}