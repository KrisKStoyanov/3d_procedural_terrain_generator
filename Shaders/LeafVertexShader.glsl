#version 450 core

layout(location = 0) in vec4 VertexPos;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec2 VertexUV;

out VS_Data{
	vec4 pos;
	vec3 normal;
	vec2 UV;
} vs_data;

void main(void)
{
	vs_data.pos = VertexPos;
	vs_data.normal = VertexNormal;
	vs_data.UV = VertexUV;

	gl_Position = VertexPos;
}