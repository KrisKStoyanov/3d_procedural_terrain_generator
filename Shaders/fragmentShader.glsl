#version 450 core

in vec3 normalExport;
in vec2 texCoordsExport;

out vec4 FragColor;

struct Light
{
   vec4 ambCols;
   vec4 difCols;
   vec4 specCols;
   vec4 coords;
};
uniform Light light0;

uniform vec4 globAmb;
  
struct Material
{
   vec4 ambRefl;
   vec4 difRefl;
   vec4 specRefl;
   vec4 emitCols;
   float shininess;
};
uniform Material terrainFandB;

vec3 normal, lightDirection;
vec4 fAndBDif;

uniform sampler2D texSampler;

void main(void)
{
	lightDirection = normalize(vec3(light0.coords));
	fAndBDif = max(dot(normalExport, lightDirection), 0.0f) * (light0.difCols *
		terrainFandB.difRefl);

	vec4 fieldTexColor = texture(texSampler, texCoordsExport);
	FragColor = fieldTexColor * fAndBDif;
}