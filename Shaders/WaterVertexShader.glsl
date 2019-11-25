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

vec3 GerstnerWave(vec2 _direction, float _steepness, float _wavelength, vec4 _vertexPos, inout vec3 _tangent, inout vec3 _binormal);

void main(void)
{
	exCoords = VertexCoords;
	exUV = VertexUV;
	exTime = u_time;

	vec3 tangent = vec3(1.0f, 0.0f, 0.0f);
	vec3 binormal = vec3(0.0f, 0.0f, 1.0f);

	vec3 waveA = GerstnerWave(vec2(1.0f, 0.5f), 0.5f, 5.0f, VertexCoords, tangent, binormal);
	vec3 waveB = GerstnerWave(vec2(0.0f, 1.0f), 0.35f, 20.0f, VertexCoords, tangent, binormal);
	vec3 waveC = GerstnerWave(vec2(1.0f, 1.0f), 0.15f, 10.0f, VertexCoords, tangent, binormal);

	exNormal = normalize(cross(binormal, tangent));

	vec3 combinedWave = VertexCoords.xyz + waveA + waveB + waveC;

	gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix *
		vec4(combinedWave.x,
			combinedWave.y,
			combinedWave.z,
			VertexCoords.w);
}

vec3 GerstnerWave(vec2 _direction, float _steepness, float _wavelength, vec4 _vertexPos,
	inout vec3 _tangent, inout vec3 _binormal) {
	float waveCoef = 2 * PI / _wavelength;
	float phaseSpeed = sqrt(9.8f / waveCoef);
	_direction = normalize(_direction);
	float waveMomentum = waveCoef * (dot(_direction, vec2(_vertexPos.x, _vertexPos.z)) - phaseSpeed * u_time);
	float wavePressure = _steepness / waveCoef;

	_tangent += vec3(
		-_direction.x * _direction.x * (_steepness * sin(waveMomentum)),
		_direction.x * (_steepness * cos(waveMomentum)),
		-_direction.x * _direction.y * (_steepness * sin(waveMomentum))
	);

	_binormal += vec3(
		-_direction.x * _direction.y * (_steepness * sin(waveMomentum)),
		_direction.y * (_steepness * cos(waveMomentum)),
		-_direction.y * _direction.y * (_steepness * sin(waveMomentum))
	);

	return vec3(_direction.x * (wavePressure * cos(waveMomentum)), wavePressure * sin(waveMomentum), _direction.y * (wavePressure * cos(waveMomentum)));
}