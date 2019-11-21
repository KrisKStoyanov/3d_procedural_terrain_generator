#version 450 core

layout(location = 0) in vec3 VertexPos;

uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

out vec3 ExVertexUV;

void main() {
	ExVertexUV = VertexPos;
	vec4 pos = u_projectionMatrix * u_viewMatrix * vec4(VertexPos, 1.0);
	gl_Position = pos.xyww;
}