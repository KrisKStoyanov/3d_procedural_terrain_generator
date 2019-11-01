#version 450 core

layout(location = 0) in vec4 VertexPos;
layout(location = 1) in vec4 VertexColor;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

out vec4 ExVertexColor;

void main() {
	ExVertexColor = VertexColor;
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * VertexPos;
	//gl_Position = VertexPos;
}