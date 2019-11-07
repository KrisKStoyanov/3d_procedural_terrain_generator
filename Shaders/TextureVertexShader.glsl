#version 450 core

layout(location = 0) in vec3 VertexPos;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec2 VertexUV;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

out vec3 ExFragPos;
out vec3 ExNormal;
out vec2 ExVertexUV;

void main() {
	ExFragPos = vec3(ModelMatrix * vec4(VertexPos, 1.0f));
	ExNormal = mat3(transpose(inverse(ModelMatrix))) * VertexNormal;
	ExVertexUV = VertexUV;
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(VertexPos, 1.0f);
	//gl_Position = VertexPos;
}