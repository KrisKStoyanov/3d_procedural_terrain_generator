#pragma once
#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

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
	void Update();
	void Terminate();
	GLFWwindow* Window = NULL;

	void ConfigTerrain();
	void ConfigTrees();
	void ConfigClouds();
	void ConfigWater();
};

