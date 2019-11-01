#pragma once
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Vertex.h"
#include "Mesh.h"
#include "Camera.h"

class Renderer
{
public:
	Renderer();
	~Renderer();
	bool Init(
		const char* _Title,
		const int _Width,
		const int _Height);
	void Setup();
	void Draw(GLuint _VAO, GLuint _ElementCount);
	void Update();
	void Terminate();

	GLFWwindow* Window = NULL;
	Camera* MainCamera = NULL;
	Mesh* TestCube = NULL;

	void ConfigTerrain();
	void ConfigTrees();
	void ConfigClouds();
	void ConfigWater();
};

