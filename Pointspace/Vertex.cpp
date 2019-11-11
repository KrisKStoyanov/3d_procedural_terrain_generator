#include "Vertex.h"

Vertex::Vertex(glm::vec4 _Coords) {
	Coords = _Coords;
}

Vertex::Vertex(float _x, float _y, float _z, float _normalX, float _normalY, float _normalZ, float _texCoordsX, float _texCoordsY)
{
	Coords.x = _x;
	Coords.y = _y;
	Coords.z = _z;

	Normal.x = _normalX;
	Normal.y = _normalY;
	Normal.z = _normalZ;

	TexCoords.x = _texCoordsX;
	TexCoords.y = _texCoordsY;
}

Vertex::Vertex() {

}

Vertex::~Vertex(){

}
