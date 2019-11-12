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

class Material
{
public:
	Material(glm::vec4 _AmbientC, glm::vec4 _DiffuseC, glm::vec4 _SpecularC, float _Shininess);
	~Material();
	glm::vec4 AmbientC;
	glm::vec4 DiffuseC;
	glm::vec4 SpecularC;
	float Shininess;
};

