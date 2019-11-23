#version 450 core

layout(vertices = 3) out;

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

	gl_TessLevelInner[0] = u_innerTL;
	gl_TessLevelInner[1] = u_innerTL;
	
	gl_TessLevelOuter[0] = u_outerTL;
	gl_TessLevelOuter[1] = u_outerTL;
	gl_TessLevelOuter[2] = u_outerTL;
	gl_TessLevelOuter[3] = u_outerTL;

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}