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
	Skybox();
	~Skybox();

	Shader* m_Shader = NULL;
	const char* m_VertexShaderSource = "../Shaders/SkyboxVertexShader.glsl";
	const char* m_FragmentShaderSource = "../Shaders/SkyboxFragmentShader.glsl";

	std::vector<std::string> m_CubeMapTextures
	{
		"../External Resources/Skybox/miramar_ft.tga",
		"../External Resources/Skybox/miramar_bk.tga",
		"../External Resources/Skybox/miramar_up.tga",
		"../External Resources/Skybox/miramar_dn.tga",
		"../External Resources/Skybox/miramar_rt.tga",
		"../External Resources/Skybox/miramar_lf.tga"
	};

	unsigned int VAO = 0, VBO = 0, IBO = 0, TextureID = 0;

	void Draw(Camera*& _Camera);
	void Clear();
};

