#version 450 core

layout(triangles, equal_spacing, cw) in;

in TCS_Data{
	vec4 pos;
	vec3 normal;
	vec2 UV;
} tcs_data[];

out TES_Data{
	vec4 pos;
	vec3 normal;
	vec2 UV;
} tes_data;

vec4 interpolate(in vec4 v0, in vec4 v1, in vec4 v2)
{
	vec4 a = mix(v0, v1, gl_TessCoord.x);
	vec4 b = mix(v2, v1, gl_TessCoord.x);
	return mix(a, b, gl_TessCoord.y);
}

void main()
{
	tes_data.pos =
		(gl_TessCoord.x * gl_in[0].gl_Position) +
		(gl_TessCoord.y * gl_in[1].gl_Position) +
		(gl_TessCoord.z * gl_in[2].gl_Position);
	tes_data.normal =
		(tcs_data[0].normal * gl_TessCoord.x) +
		(tcs_data[1].normal * gl_TessCoord.y) +
		(tcs_data[2].normal * gl_TessCoord.z);
	tes_data.UV =
		(tcs_data[0].UV * gl_TessCoord.x) +
		(tcs_data[1].UV * gl_TessCoord.y) *
		(tcs_data[2].UV * gl_TessCoord.z);

	gl_Position = tes_data.pos;

	//gl_Position = interpolate(
	//	gl_in[0].gl_Position,
	//	gl_in[1].gl_Position,
	//	gl_in[2].gl_Position);
}