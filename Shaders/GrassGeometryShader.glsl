#version 450 core

#define PI 3.14159265359

layout(triangles) in;
layout(invocations = 1) in;
layout(triangle_strip) out;
layout(max_vertices = 60) out; //compile time constant (hardware dependant)

in TES_Data{
	vec4 pos;
	vec3 normal;
	vec2 UV;
} tes_data[];

out FS_Data{
	vec4 pos;
	vec3 normal;
	vec2 UV;
	vec4 color;
} fs_data;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

uniform float u_time;

float swaySpeed = 0.5f;
float swayAmplitude = 0.125f;

void GrassGen(int _index);
float rand(vec2 _co);

void main(void)
{
	for (int i = 0; i < gl_in.length(); ++i) {

		if (tes_data[i].pos.y > -2.0f && tes_data[i].pos.y < 4.0f) {
			GrassGen(i);
		}
	}
}

void GrassGen(int _index) {

	vec4 vertexOrigin = tes_data[_index].pos;
	mat4 transformMatrix = u_projectionMatrix * u_viewMatrix * u_modelMatrix;
	float swayCoef = (2 * PI) * (swaySpeed * u_time - vertexOrigin.y);

	fs_data.color = vec4(0.33f, 1.0f, 0.4f, 1.0f);
	fs_data.pos = transformMatrix * (vertexOrigin + vec4(0.1f, 0.0f, 0.0f, 0.0f));
	fs_data.normal = tes_data[_index].normal;
	fs_data.UV = tes_data[_index].UV;
	gl_Position = fs_data.pos;
	EmitVertex();

	fs_data.color = vec4(0.33f, 1.0f, 0.4f, 1.0f);
	fs_data.pos = transformMatrix * (vertexOrigin + vec4(-0.1f, 0.0f, 0.0f, 0.0f));
	fs_data.normal = tes_data[_index].normal;
	fs_data.UV = tes_data[_index].UV;
	gl_Position = fs_data.pos;
	EmitVertex();

	fs_data.color = vec4(0.33f, 0.9f, 0.4f, 1.0f);
	fs_data.pos = transformMatrix * (vertexOrigin + vec4(0.05f + swayAmplitude * sin(swayCoef), 0.5f, 0.0f, 0.0f));
	fs_data.normal = tes_data[_index].normal;
	fs_data.UV = tes_data[_index].UV;
	gl_Position = fs_data.pos;
	EmitVertex();

	fs_data.color = vec4(0.33f, 0.9f, 0.4f, 1.0f);
	fs_data.pos = transformMatrix * (vertexOrigin + vec4(-0.05f + swayAmplitude * sin(swayCoef), 0.5f, 0.0f, 0.0f));
	fs_data.normal = tes_data[_index].normal;
	fs_data.UV = tes_data[_index].UV;
	gl_Position = fs_data.pos;
	EmitVertex();

	fs_data.color = vec4(0.26f, 0.91f, 0.48f, 1.0f);
	fs_data.pos = transformMatrix * (vertexOrigin + vec4(0.0f + (swayAmplitude + swayAmplitude) * sin(swayCoef), 1.0f, 0.0f, 0.0f));
	fs_data.normal = tes_data[_index].normal;
	fs_data.UV = tes_data[_index].UV;
	gl_Position = fs_data.pos;
	EmitVertex();

	EndPrimitive();
}

float rand(vec2 _co) {
	return fract(sin(dot(_co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}