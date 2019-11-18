#include "Vertex.h"

Vertex::Vertex(glm::vec4 _coords, glm::vec2 _texCoords) {
	Coords = _coords;
	TexCoords = _texCoords;
}


Vertex::Vertex() {

}

Vertex::~Vertex(){

}
