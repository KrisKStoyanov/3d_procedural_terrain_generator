#include "Vertex.h"

Vertex::Vertex(float _X, float _Y, float _Z, float _W, float _R, float _G, float _B, float _A) {
	Coords[0] = _X;
	Coords[1] = _Y;
	Coords[2] = _Z;
	Coords[3] = _W;

	Colors[0] = _R;
	Colors[1] = _G;
	Colors[2] = _B;
	Colors[3] = _A;
}

Vertex::~Vertex(){

}
