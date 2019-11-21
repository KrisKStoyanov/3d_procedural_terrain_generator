#version 450 core

in vec4 exPos;
in vec3 exNormal;
in vec2 exUV;

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

uniform sampler2D u_snowTexSampler;
uniform sampler2D u_rockTexSampler;
uniform sampler2D u_grassTexSampler;
uniform sampler2D u_sandTexSampler;

float snowAltitude = 6.0f;
float rockAltitude = 3.0f;
float grassAltitude = 0.0f;
float sandAltitude = -1.0f;

void main(void)
{
	lightDirection = normalize(vec3(u_dirLight.coords));
	diffuseColor = max(dot(exNormal, lightDirection), 0.0f) * (u_dirLight.difCols *
		u_material.difRefl);
	
	vec4 snowTexColor = texture(u_snowTexSampler, exUV);
	vec4 rockTexColor = texture(u_rockTexSampler, exUV);
	vec4 grassTexColor = texture(u_grassTexSampler, exUV);
	vec4 sandTexColor = texture(u_sandTexSampler, exUV);

	vec4 texColor = sandTexColor;
	if (exPos.y > sandAltitude) {
		texColor = mix(sandTexColor, grassTexColor, abs(clamp((exPos.y - sandAltitude) / (grassAltitude - sandAltitude), 0.0f, 1.0f)));
	}
	if (exPos.y > grassAltitude) {
		texColor = mix(grassTexColor, rockTexColor, abs(clamp((exPos.y - grassAltitude) / (rockAltitude - grassAltitude), 0.0f, 1.0f)));
	}
	if (exPos.y > rockAltitude) {
		texColor = mix(rockTexColor, snowTexColor, abs(clamp((exPos.y - rockAltitude) / (snowAltitude - rockAltitude), 0.0f, 1.0f)));
	}

	FragColor = texColor * diffuseColor;
}