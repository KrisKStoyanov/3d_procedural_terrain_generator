#version 450 core

layout(points) in;
layout(triangle_strip, max_vertices = 9) out;

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

//const float magnitude = 0.4f;

//void LineGen(int _index);

void main(void)
{
	for (int i = 0; i < gl_in.length(); ++i) {

		fs_data.pos = vs_data[i].pos;
		fs_data.normal = vs_data[i].normal;
		fs_data.UV = vs_data[i].UV;

		if (vs_data[i].pos.y > 0.0f) {
			gl_Position = gl_in[i].gl_Position +
				vec4(0.5f, 0.0f, 0.0f, 0.0f);
			EmitVertex();

			gl_Position = gl_in[i].gl_Position +
				vec4(-0.5f, 0.0f, 0.0f, 0.0f);
			EmitVertex();

			gl_Position = gl_in[i].gl_Position + vec4(0.0f, 1.0f, 0.0f, 0.0f) * 2.0f;
			EmitVertex();
		}
	}
	EndPrimitive();
	//exPos = gs_in[0].pos;
	//exNormal = gs_in[0].normal;
	//exUV = gs_in[0].UV;

	//gl_Position = gl_in[0].gl_Position;
	//EmitVertex();
	//EndPrimitive();
	//LineGen(0);
	//LineGen(1);
	//LineGen(2);
}

//void LineGen(int _index) {
//	gl_Position = gl_in[_index].gl_Position;
//	EmitVertex();
//	gl_Position = gl_in[_index].gl_Position * magnitude/* vec4(gs_in[_index].normal, 0.0f) * magnitude;*/;
//	EmitVertex();
//	EndPrimitive();
//}