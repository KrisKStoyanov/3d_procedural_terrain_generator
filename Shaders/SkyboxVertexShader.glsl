#version 450 core

layout(location = 0) in vec3 VertexPos;

uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

out vec3 ExVertexUV;

void main() {
	ExVertexUV = VertexPos;
	vec4 pos = ProjectionMatrix * ViewMatrix * vec4(VertexPos, 1.0);
	gl_Position = pos.xyww;
}