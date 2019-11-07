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
	Material();
	~Material();
	glm::vec3 AmbientC;
	glm::vec3 DiffuseC;
	glm::vec3 SpecularC;
	float Shininess;
};

