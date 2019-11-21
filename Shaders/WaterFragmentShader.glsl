#version 450 core

in vec4 exCoords;
in vec3 exNormal;
in vec2 exUV;

in float exTime;

out vec4 FragColor;

struct Light
{
	vec4 ambCols;
	vec4 difCols;
	vec4 specCols;
	vec4 coords;
};
uniform Light u_dirLight;

struct Material
{
	vec4 ambRefl;
	vec4 difRefl;
	vec4 specRefl;
	vec4 emitCols;
	float shininess;
};
uniform Material u_material;

vec3 normal, lightDirection;
vec4 diffuseColor;

uniform sampler2D u_texSampler;

void main(void)
{
	float procX = exUV.x + exTime;
	vec4 fieldTexColor = texture(u_texSampler, vec2(procX , exUV.y));
	FragColor = fieldTexColor;
}

