#pragma once
#include <GL/glew.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
	Shader(
		const GLchar* _VertexShaderPath, 
		const GLchar* _FragmentShaderPath);
	Shader(
		const GLchar* _VertexShaderPath, 
		const GLchar* _GeometryShaderPath, 
		const GLchar* _FragmentShaderPath);
	Shader(
		const GLchar* _VertexShaderPath,
		const GLchar* _TCShaderPath,
		const GLchar* _TEShaderPath,
		const GLchar* _FragmentShaderPath);
	Shader(
		const GLchar* _VertexShaderPath, 
		const GLchar* _TCShaderPath, 
		const GLchar* _TEShaderPath,
		const GLchar* _GeometryShaderPath,
		const GLchar* _FragmentShaderPath);
	~Shader();

	GLuint 
		ProgramID, 
		VertexShaderID, 
		TessControlShaderID,
		GeometryShaderID, 
		TessEvalShaderID,
		FragmentShaderID;

	std::string ReadFile(const GLchar* _FilePath);

	inline void Activate() { glUseProgram(ProgramID); };
	void Clear();

	void PrintShaderLog(GLuint _ShaderID);
	void PrintProgramLog(GLuint _ProgramID);

	void SetBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ProgramID, name.c_str()), (int)value);
	}

	void SetInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ProgramID, name.c_str()), value);
	}

	void SetFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(ProgramID, name.c_str()), value);
	}

	void SetVec2(const std::string& name, const glm::vec2& value) const
	{
		glUniform2fv(glGetUniformLocation(ProgramID, name.c_str()), 1, &value[0]);

	}
	void SetVec2(const std::string& name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(ProgramID, name.c_str()), x, y);
	}

	void SetVec3(const std::string& name, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(ProgramID, name.c_str()), 1, &value[0]);

	}
	void SetVec3(const std::string& name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(ProgramID, name.c_str()), x, y, z);
	}

	void SetVec4(const std::string& name, const glm::vec4& value) const
	{
		glUniform4fv(glGetUniformLocation(ProgramID, name.c_str()), 1, &value[0]);
	}

	void SetVec4(const std::string& name, float x, float y, float z, float w)
	{
		glUniform4f(glGetUniformLocation(ProgramID, name.c_str()), x, y, z, w);
	}

	void SetMat2(const std::string& name, const glm::mat2& mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(ProgramID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void SetMat3(const std::string& name, const glm::mat3& mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(ProgramID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void SetMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ProgramID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
};

