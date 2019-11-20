#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Renderer.h"

Renderer R;

int main(int argc, char* argv[]) {
	R.Init("Pointspace", 1280, 720);

	return 0;
}