#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Renderer.h"

Renderer R;

int main(int argc, char* argv[]) {
	if (!R.Init("Pointspace", 1280, 720)) {
		return -1;
	}

	return 0;
}