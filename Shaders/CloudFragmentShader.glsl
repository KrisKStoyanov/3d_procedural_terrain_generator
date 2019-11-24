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
	vec4 cloudTexColor = texture(u_texSampler, vec2(exUV.x + 0.25f * exTime, exUV.y));

	//vec4 cloudTexColor = vec4(0.63f, 0.5f, 0.96f, 1.0f);//texture(u_texSampler, vec2(exUV.x + 0.25f * exTime - phaseSpeed, exUV.y));
	//cloudTexColor += amplitude * sin(exTime - 10.0f);

	//if (exCoords.y < 0.5f) {
	//	discard;
	//}

	//if (cloudTexColor.r > 0.8f || cloudTexColor.g > 0.85f) {
	//	discard;
	//}

	FragColor = cloudTexColor;
}