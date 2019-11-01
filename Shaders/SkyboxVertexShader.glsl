#version 450 core
layout (location = 0) in vec3 VertexPos;

out vec3 TextureCoords;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;

void main(){
	TextureCoords = VertexPos;
	gl_Position = ProjectionMatrix * ViewMatrix * vec4(VertexPos, 1.0);
}