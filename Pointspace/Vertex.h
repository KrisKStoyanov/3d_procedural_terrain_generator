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
	Vertex(glm::vec3 _Coords, glm::vec3 _Normal, glm::vec2 _TexCoords);
	Vertex();
	~Vertex();
	glm::vec3 Coords;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

