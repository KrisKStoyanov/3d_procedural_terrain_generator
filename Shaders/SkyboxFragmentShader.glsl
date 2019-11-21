#version 450 core
precision highp float;
out vec4 FragColor;

in vec3 ExVertexUV;

uniform samplerCube u_skyboxCubemap;

void main() {
	FragColor = texture(u_skyboxCubemap, ExVertexUV);
}