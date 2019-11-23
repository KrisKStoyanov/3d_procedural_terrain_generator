#version 450 core

in FS_Data{
	vec4 pos;
	vec3 normal;
	vec2 UV;
} fs_data;

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

float snowAltitude = 7.0f;
float rockAltitude = 5.0f;
float grassAltitude = 4.0f;
float sandAltitude = -3.0f;

void main(void)
{
	lightDirection = normalize(vec3(u_dirLight.coords));
	diffuseColor = max(dot(fs_data.normal, lightDirection), 0.0f) * (u_dirLight.difCols *
		u_material.difRefl);
	
	vec4 snowTexColor = texture(u_snowTexSampler, fs_data.UV);
	vec4 rockTexColor = texture(u_rockTexSampler, fs_data.UV);
	vec4 grassTexColor = texture(u_grassTexSampler, fs_data.UV);
	vec4 sandTexColor = texture(u_sandTexSampler, fs_data.UV);

	vec4 texColor = sandTexColor;
	if (fs_data.pos.y > sandAltitude) {
		texColor = mix(sandTexColor, grassTexColor, abs(clamp((fs_data.pos.y - sandAltitude) / (grassAltitude - sandAltitude), 0.0f, 1.0f)));
	}
	if (fs_data.pos.y > grassAltitude) {
		texColor = mix(grassTexColor, rockTexColor, abs(clamp((fs_data.pos.y - grassAltitude) / (rockAltitude - grassAltitude), 0.0f, 1.0f)));
	}
	if (fs_data.pos.y > rockAltitude) {
		texColor = mix(rockTexColor, snowTexColor, abs(clamp((fs_data.pos.y - rockAltitude) / (snowAltitude - rockAltitude), 0.0f, 1.0f)));
	}

	gl_FragColor = texColor * diffuseColor;
}