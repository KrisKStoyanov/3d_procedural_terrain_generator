#include "Vertex.h"

Vertex::Vertex(glm::vec4 _coords, glm::vec2 _texCoords) {
	Coords = _coords;
	TexCoords = _texCoords;
}

Vertex::Vertex(glm::vec4 _coords)
{
	Coords = _coords;
}


Vertex::Vertex() {

}

Vertex::~Vertex(){

}
