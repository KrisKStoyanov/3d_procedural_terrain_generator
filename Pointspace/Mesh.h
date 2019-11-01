#pragma once
#include <vector>
#include "Vertex.h"

class Mesh
{
public:
	Mesh(std::vector<Vertex> _VertexCollection, std::vector<GLuint> _IndexCollection, glm::vec3 _Position);
	~Mesh();

	std::vector<Vertex> VertexCollection;
	std::vector<GLuint> IndexCollection;

	GLuint ProgramID = 0, VAO = 0, VBO = 0, IBO = 0;
	GLint PositionAttribID = 0, ColorAttribID = 1;
	glm::mat4 ModelMatrix = glm::mat4(1.0f);
	glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
};

