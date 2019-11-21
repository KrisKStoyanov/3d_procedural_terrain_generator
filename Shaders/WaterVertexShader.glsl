#version 450 core

#define PI 3.14159265359

layout(location = 0) in vec4 VertexCoords;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec2 VertexUV;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;
uniform mat3 u_normalMatrix;

uniform float u_time;
uniform float u_wavelength;
uniform float u_steepness;
uniform float u_depth;

out vec4 exCoords;
out vec3 exNormal;
out vec2 exUV;

out float exTime;

void main(void)
{
	exCoords = VertexCoords;
	exNormal = normalize(u_normalMatrix * VertexNormal);
	exUV = VertexUV;
	exTime = u_time;

	vec2 waveDir = normalize(vec2(1.0f, 1.0f));

	float waveCoef = 2 * PI / u_wavelength; //k
	float phaseSpeed = sqrt(9.8f / waveCoef); //c
	float waveMomentum = waveCoef * dot(waveDir, vec2(VertexCoords.x, VertexCoords.z) - phaseSpeed * u_time); //f
	float wavePressure = u_steepness / waveCoef; //a

	float xPos = VertexCoords.x + wavePressure * cos(waveMomentum);

	gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix *
		vec4(VertexCoords.x + waveDir.x * (wavePressure * cos(waveMomentum)), 
			wavePressure * sin(waveMomentum), 
			VertexCoords.z + waveDir.y * (wavePressure * cos(waveMomentum)),
			VertexCoords.w);
}