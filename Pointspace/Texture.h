#pragma once
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "getbmp.h"
#include "stb_image.h"
#include "Shader.h"
#include <string>

class Texture
{
public:
	Texture(std::string _Path, Shader* _shader);
	~Texture();
	unsigned int ID, shaderTexLoc;
	BitMapFile* BMP = NULL;
	std::string Path;
};

