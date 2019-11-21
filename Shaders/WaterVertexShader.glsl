#version 450 core

#define PI 3.14159265359

layout(location = 0) in vec4 VertexCoords;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec2 VertexUV;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;
uniform mat3 u_normalMatrix;

uniform float u_amplitude;
uniform float u_speed;

uniform float u_time;
uniform float u_wavelength;
uniform float u_steepness;
uniform float u_depth;

out vec4 exCoords;
out vec3 exNormal;
out vec2 exUV;

out float exTime;

vec3 GerstnerWave(vec2 _direction, float _steepness, float _wavelength, vec4 _vertexPos);

void main(void)
{
	exCoords = VertexCoords;
	exNormal = normalize(u_normalMatrix * VertexNormal);
	exUV = VertexUV;
	exTime = u_time;

	vec3 waveA = GerstnerWave(vec2(1.0f, 1.0f), 0.2f, 10, VertexCoords);
	vec3 waveB = GerstnerWave(vec2(1.0f, 1.0f), 0.4f, 20, VertexCoords);
	vec3 waveC = GerstnerWave(vec2(1.0f, 1.0f), 0.9f, 15, VertexCoords);

	vec3 combinedWave = VertexCoords.xyz + waveA + waveB + waveC;

	gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix *
		vec4(combinedWave.x,
			combinedWave.y,
			VertexCoords.z,
			VertexCoords.w);
}

vec3 GerstnerWave(vec2 _direction, float _steepness, float _wavelength, vec4 _vertexPos) {
	float waveCoef = 2 * PI / _wavelength;
	float phaseSpeed = sqrt(9.8f / waveCoef);
	_direction = normalize(_direction);
	float waveMomentum = waveCoef * (dot(_direction, vec2(_vertexPos.x, _vertexPos.z)) - phaseSpeed * u_time);
	float wavePressure = _steepness / waveCoef;
	return vec3(_direction.x * (wavePressure * cos(waveMomentum)), wavePressure * sin(waveMomentum), _direction.y * (wavePressure * cos(waveMomentum)));
}