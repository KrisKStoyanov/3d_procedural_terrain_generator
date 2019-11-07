#pragma once
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "Shader.h"

#include "stb_image.h"

class Skybox
{
public:
	Skybox(std::vector<std::string> _FaceTexturePaths, const GLchar* _VertexShaderPath, const GLchar* _FragmentShaderPath);
	~Skybox();
	Shader* SkyboxShader = NULL;
	GLuint VAO = 0, VBO = 0, IBO = 0, TextureID = 0;
	void Draw(Camera* _Camera);
	void Clear();
};

