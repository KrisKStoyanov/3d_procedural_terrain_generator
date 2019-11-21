#version 450 core

layout(location = 0) in vec4 VertexPos;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec2 VertexUV;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;
uniform mat3 u_normalMatrix;

out vec4 exPos;
out vec3 exNormal;
out vec2 exUV;

void main(void)
{
	exPos = VertexPos;
	exNormal = normalize(u_normalMatrix * VertexNormal);
	exUV = VertexUV;

    gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * VertexPos;
}