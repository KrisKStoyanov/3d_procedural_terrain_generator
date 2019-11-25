#version 450 core

in VS_Data{
	vec4 pos;
	vec3 normal;
	vec2 UV;
} vs_data;

void main(void)
{
	gl_FragColor = vec4(0.55f, 0.27f, 0.075f, 1.0f);
}