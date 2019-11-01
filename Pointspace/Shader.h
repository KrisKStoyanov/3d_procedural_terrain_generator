#pragma once
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Shader
{
public:
	Shader(const GLchar* _VertexShaderPath, const GLchar* _FragmentShaderPath);
	~Shader();

	std::string ReadFile(const GLchar* _FilePath);
	void PrintShaderLog(GLuint _ShaderID);
	void PrintProgramLog(GLuint _ProgramID);

	void Activate();
	void Deactivate();
	
	void Clear();

	GLuint ProgramID, VertexShaderID, FragmentShaderID;
};

