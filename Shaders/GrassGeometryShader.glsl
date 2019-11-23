#version 450 core

#define PI 3.14159265359

layout(triangles) in;
layout(triangle_strip) out;
layout(max_vertices = 60) out; //compile time constant (hardware dependant)

//in VS_Data{
//	vec4 pos;
//	vec3 normal;
//	vec2 UV;
//} vs_data[];

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

//void GrassGen(int _index);

float rand(vec2 _co);

void main(void)
{
	for (int i = 0; i < gl_in.length(); ++i) {

		fs_data.color = vec4(0.01f, 0.85f, 0.31f, 1.0f);
		fs_data.pos = u_projectionMatrix * u_viewMatrix * u_modelMatrix * tes_data[i].pos;
		fs_data.normal = tes_data[i].normal;
		fs_data.UV = tes_data[i].UV;
		gl_Position = fs_data.pos;
		EmitVertex();
		//if (tes_data[i].pos.y > 0.0f && tes_data[i].pos.y < 2.0f) {
		//	GrassGen(i);
		//}
	}
}

//void GrassGen(int _index) {
//
//	vec4 vertexOrigin = gl_in[_index].gl_Position;
//
//	//-------------------------------------------
//
//	fs_data.color = vec4(0.01f, 0.85f, 0.31f, 1.0f);
//	vec4 vertexPos = vertexOrigin + vec4(0.1f, 0.0f, 0.0f, 0.0f);
//	gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vertexPos;
//	EmitVertex();
//
//	fs_data.color = vec4(0.01f, 0.85f, 0.31f, 1.0f);
//	vertexPos = vertexOrigin + vec4(-0.1f, 0.0f, 0.0f, 0.0f);
//	gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vertexPos;
//	EmitVertex();
//
//	fs_data.color = vec4(0.01f, 0.85f, 0.31f, 1.0f);
//	vertexPos = vertexOrigin + vec4(0.075f + 0.125f * sin(2 * PI * u_time - vertexOrigin.y), 0.5f, 0.0f, 0.0f);
//	gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vertexPos;
//	EmitVertex();
//
//	fs_data.color = vec4(0.01f, 0.85f, 0.31f, 1.0f);
//	vertexPos = vertexOrigin + vec4(-0.075f + 0.125f * sin(2 * PI * u_time - vertexOrigin.y), 0.5f, 0.0f, 0.0f);
//	gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vertexPos;
//	EmitVertex();
//
//	fs_data.color = vec4(0.33f, 1.0f, 0.4f, 1.0f);
//	vertexPos = vertexOrigin + vec4(0.0f + 0.25f * sin(2 * PI * u_time - vertexOrigin.y), 1.0f, 0.0f, 0.0f);
//	gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vertexPos;
//	EmitVertex();
//
//	//-------------------------------------------
//
//	EndPrimitive();
//}

float rand(vec2 _co) {
	return fract(sin(dot(_co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}