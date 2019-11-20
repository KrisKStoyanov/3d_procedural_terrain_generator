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

GLuint TextureFromFile(const char* _Path);

class Texture
{
public:
	Texture(const char* _Path);
	~Texture();
	unsigned int m_ID;
	const char* m_Path;
};

