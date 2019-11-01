#version 450 core
out vec4 FragColor;

in vec3 TextureCoords;

uniform samplerCube Skybox;

void main(){
	FragColor = texture(Skybox, TextureCoords);
}