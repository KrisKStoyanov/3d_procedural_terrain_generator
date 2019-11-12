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
	Vertex(glm::vec4 _Coords);
	Vertex(float _x, float _y, float _z, float _normalX, float _normalY, float _normalZ, float _texCoordsX, float _texCoordsY);
	Vertex();
	~Vertex();
	glm::vec4 Coords;
	glm::vec3 Normal = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec2 TexCoords;
};

