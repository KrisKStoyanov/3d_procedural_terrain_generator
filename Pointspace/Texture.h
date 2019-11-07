#pragma once
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

GLint TextureFromFile(const char* _Path, std::string _Directory);

class Texture
{
public:
	Texture(const char* _Path, std::string _Directory);
	~Texture();
	GLuint ID;
	std::string Type;
};

