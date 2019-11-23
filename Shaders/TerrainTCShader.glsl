#version 450 core

layout(vertices = 6) out;

in VS_Data{
	vec4 pos;
	vec3 normal;
	vec2 UV;
} vs_data[];

out TCS_Data{
	vec4 pos;
	vec3 normal;
	vec2 UV;
} tcs_data[];

uniform int u_innerTL;
uniform int u_outerTL;

void main(void)
{
	tcs_data[gl_InvocationID].pos = vs_data[gl_InvocationID].pos;
	tcs_data[gl_InvocationID].normal = vs_data[gl_InvocationID].normal;
	tcs_data[gl_InvocationID].UV = vs_data[gl_InvocationID].UV;

	gl_TessLevelInner[0] = 1.0f;
	gl_TessLevelInner[1] = 1.0f;

	gl_TessLevelOuter[0] = 1.0f;
	gl_TessLevelOuter[1] = 1.0f;
	gl_TessLevelOuter[2] = 1.0f;
	gl_TessLevelOuter[3] = 1.0f;

	//gl_TessLevelOuter[0] = 2.0;
	//gl_TessLevelOuter[1] = 4.0;
	//gl_TessLevelOuter[2] = 6.0;

	//gl_TessLevelInner[0] = 8.0;
	//gl_TessLevelInner[1] = 8.0;

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}