#version 450 core

layout(location = 0) in vec4 VertexPos;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec2 VertexUV;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

uniform float u_time;

out VS_Data{
	vec4 pos;
	vec3 normal;
	vec2 UV;
} vs_data;

void main(void)
{
	mat3 normalMatrix = mat3(transpose(inverse(u_viewMatrix * u_modelMatrix)));
	vs_data.pos = VertexPos;
	vs_data.normal = normalize(normalMatrix * VertexNormal);
	vs_data.UV = VertexUV;

	gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * VertexPos;
}