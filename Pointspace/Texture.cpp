#include "Texture.h"

Texture::Texture(std::string _path, Shader* _shader)
{
	Path = _path;
	BMP = getbmp(_path);
	glGenTextures(1, &ID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, BMP->sizeX, BMP->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, BMP->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	shaderTexLoc = glGetUniformLocation(_shader->ProgramID, "texSample");
	glUniform1i(shaderTexLoc, 0);
	//ID = TextureFromFile(_Path);
}

Texture::~Texture()
{
}
