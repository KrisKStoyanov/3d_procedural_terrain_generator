#pragma once
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Vertex
{
public:
	Vertex(float _X, float _Y, float _Z, float _W, float _R, float _G, float _B, float _A);
	~Vertex();
	float Coords[4];
	float Colors[4];
};

