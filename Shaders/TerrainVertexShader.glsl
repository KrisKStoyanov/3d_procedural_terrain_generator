#version 450 core

layout(location = 0) in vec4 VertexPos;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec2 VertexUV;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;
uniform mat3 u_normalMatrix;

out VS_Data{
	vec4 pos;
	vec3 normal;
	vec2 UV;
} vs_data;

void main(void)
{
	vs_data.pos = VertexPos;
	vs_data.normal = normalize(u_normalMatrix * VertexNormal);
	vs_data.UV = VertexUV;

	//vs_out.pos = VertexPos;
	//vs_out.UV = VertexUV;
	//mat3 normalMatrix = mat3(transpose(inverse(u_viewMatrix * u_modelMatrix)));
	//exNormal = normalize(vec3(u_projectionMatrix * vec4(normalMatrix * VertexNormal, 0.0f)));
	//vs_out.normal = normalize(vec3(u_projectionMatrix * vec4(normalMatrix * VertexNormal, 0.0f)));

    gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * VertexPos;
}