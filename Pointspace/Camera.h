#pragma once
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.h"

class Camera
{
public:
	Camera(glm::vec3 _Position, float _FOV, float _ProjWidth, float _ProjHeight);
	~Camera();

	glm::vec3 Position, Direction, Target, Front, Up, Right, WorldUp;
	GLfloat Yaw, Pitch, MovementSpeed, MouseSensitivity;
	glm::mat4 ViewMatrix, ProjectionMatrix;
	GLuint ProjectionMatrixLoc, ViewMatrixLoc, ModelMatrixLoc;

	void SetupShader(const GLchar* _VertexShaderPath, const GLchar* _FragmentShaderPath);
	Shader* MainShader = NULL;

	void Update(glm::mat4 _ModelMatrix);
};

