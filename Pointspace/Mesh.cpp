#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> _VertexCollection, std::vector<unsigned int> _IndexCollection, glm::vec3 _Position)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, _VertexCollection.size() * sizeof(Vertex), _VertexCollection.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _IndexCollection.size() * sizeof(unsigned int), _IndexCollection.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(_VertexCollection[0]), (void*)0);
	glEnableVertexAttribArray(0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(_VertexCollection[0]), (GLvoid*)sizeof(_VertexCollection[0].Coords));
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(_VertexCollection[0]), (GLvoid*)(sizeof(_VertexCollection[0].Coords) + sizeof(_VertexCollection[0].Normal)));
	//glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	//glDisableVertexAttribArray(1);
	//glDisableVertexAttribArray(2);

	VertexCollection = _VertexCollection;
	IndexCollection = _IndexCollection;

	Position = _Position;
}

Mesh::~Mesh()
{
	glDisableVertexAttribArray(0);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IBO);
}
