#version 450 core
precision highp float;
out vec4 FragColor;

in vec3 ExVertexUV;

uniform samplerCube SkyboxCubemap;

void main() {
	FragColor = texture(SkyboxCubemap, ExVertexUV);
}