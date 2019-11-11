#version 420 core

layout(location=0) in vec4 vCoords;
//layout(location=1) in vec3 vNormal;
//layout(location = 2) in vec2 vTexCoords;

uniform mat4 projMat;
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat3 normalMat;

out vec3 normalExport;

void main(void)
{
	//normalExport = vNormal;
	//normalExport = normalize(normalMat * normalExport);
   
   gl_Position = projMat * viewMat * modelMat * vCoords;
}