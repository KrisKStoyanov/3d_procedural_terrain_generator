#pragma once
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

GLint TextureFromFile(const char* _Path);

enum class TextureType {
	DIFFUSE,
	SPECULAR
};

class Texture
{
public:
	Texture(const char* _Path, TextureType _Type);
	~Texture();
	GLuint ID;
	TextureType Type;
	std::string Path;
};

