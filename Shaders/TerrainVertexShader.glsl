#version 450 core

layout(location = 0) in vec4 VertexPos;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec2 VertexUV;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat3 NormalMatrix;

out vec3 NormalExport;
out vec2 UVExport;

void main(void)
{
	NormalExport = VertexNormal;
	NormalExport = normalize(NormalMatrix * NormalExport);

	UVExport = VertexUV;
   
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * VertexPos;
}