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
	Vertex(glm::vec4 _Coords, glm::vec4 _Colors);
	Vertex();
	~Vertex();
	glm::vec4 Coords;
	glm::vec4 Colors;
};

