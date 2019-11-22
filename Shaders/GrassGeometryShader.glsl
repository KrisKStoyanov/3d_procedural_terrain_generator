#version 450 core

layout(points) in;
layout(triangle_strip) out;
layout(max_vertices = 60) out; //compile time constant (hardware dependant)

in VS_Data{
	vec4 pos;
	vec3 normal;
	vec2 UV;
} vs_data[];

out FS_Data{
	vec4 pos;
	vec3 normal;
	vec2 UV;
} fs_data;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

void GrassGen(int _index);

void main(void)
{

	for (int i = 0; i < gl_in.length(); ++i) {

		fs_data.pos = vs_data[i].pos;
		fs_data.normal = vs_data[i].normal;
		fs_data.UV = vs_data[i].UV;

		if (vs_data[i].pos.y > 0.0f) {
			GrassGen(i);
		}

		//vec4 offset = vec4(-1.0, 1.0, 0.0, 0.0);
		//vec4 vertexPos = offset + gl_in[0].gl_Position;
		//gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vertexPos;

		//gl_Position = gl_in[i].gl_Position +
		//	vec4(0.5f, 0.0f, 0.0f, 0.0f);
		//EmitVertex();
	}
	//exPos = gs_in[0].pos;
	//exNormal = gs_in[0].normal;
	//exUV = gs_in[0].UV;

	//gl_Position = gl_in[0].gl_Position;
	//EmitVertex();
	//EndPrimitive();
	//gl_Position = gl_in[_index].gl_Position;
	//EmitVertex();
	//gl_Position = gl_in[_index].gl_Position * magnitude/* vec4(gs_in[_index].normal, 0.0f) * magnitude;*/;
	//EmitVertex();
	//EndPrimitive();
}

void GrassGen(int _index) {

	vec4 vertexOrigin = gl_in[_index].gl_Position;

	//-------------------------------------------
	vec4 vertexPos = vertexOrigin + vec4(0.5f, 0.0f, 0.0f, 0.0f);
	gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vertexPos;
	EmitVertex();

	vertexPos = vertexOrigin + vec4(-0.5f, 0.0f, 0.0f, 0.0f);
	gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vertexPos;
	EmitVertex();

	vertexPos = vertexOrigin + vec4(0.5f, 2.0f, 0.5f, 0.0f);
	gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vertexPos;
	EmitVertex();

	vertexPos = vertexOrigin + vec4(-0.5f, 2.0f, 0.5f, 0.0f);
	gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vertexPos;
	EmitVertex();

	//-------------------------------------------

	EndPrimitive();
}