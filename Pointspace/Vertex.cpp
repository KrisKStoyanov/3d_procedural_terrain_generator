#include "Vertex.h"

Vertex::Vertex(glm::vec3 _Coords, glm::vec3 _Normal, glm::vec2 _TexCoords) {
	Coords = _Coords;
	Normal = _Normal;
	TexCoords = _TexCoords;
}

Vertex::Vertex() {

}

Vertex::~Vertex(){

}
