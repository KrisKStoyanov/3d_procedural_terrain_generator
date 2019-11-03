#include "Renderer.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void mouse_callback(GLFWwindow* _Window, double _PosX, double _PosY) {
	//update MainCamera 
	Renderer R;
	R.MainCamera->UpdateTransformMouse(_PosX, _PosY);
}

bool Renderer::Init(const char* _Title, const int _Width, const int _Height)
{
	if (!glfwInit()) {
		return false;
	}
	Window = glfwCreateWindow(_Width, _Height, _Title, NULL, NULL);
	if (!Window) {
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(Window);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glewExperimental = GL_TRUE;
	GLenum initState = glewInit();
	if (initState != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(initState));
	}

	glClearColor(0.35f, 0.35f, 0.35f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	MainCamera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f), 60, _Width, _Height);
	MainCamera->SetupShader("../Shaders/BasicVertexShader.glsl", "../Shaders/BasicFragmentShader.glsl", ShaderType::BASIC);
	MainCamera->SetupShader("../Shaders/SkyboxVertexShader.glsl", "../Shaders/SkyboxFragmentShader.glsl", ShaderType::SKYBOX);
	MainCamera->BasicShader->Activate();
	//glfwSetCursorPosCallback(Window, [](GLFWwindow* _Window, double _CursorX, double _CursorY) {
	//		MainCamera->UpdateTransformMouse(_CursorX, _CursorY);
	//	});

	//glfwSetCursorPosCallback(Window, [](GLFWwindow* _Window, double _CursorX, double _CursorY){

	//});

	/*glfwSetCursorPosCallback(Window, );*/
	Setup();
	return true;
}

void Renderer::Setup()
{
	std::vector<Vertex> VertexCollection = {
	{ 0.5f,  0.5f, 0.5f, 1.0f , 1.0f, 0.0f, 0.0f, 1.0f },
	{ 0.5f, -0.5f, 0.5f, 1.0f , 1.0f, 1.0f, 0.0f, 1.0f },
	{ -0.5f, -0.5f, 0.5f, 1.0f , 1.0f, 0.0f, 1.0f, 1.0f },
	{ -0.5f,  0.5f, 0.5f, 1.0f , 0.0f, 1.0f, 1.0f, 1.0f },
	{ 0.5f,  0.5f, -0.5f, 1.0f , 1.0f, 0.0f, 0.0f, 1.0f },
	{ 0.5f, -0.5f, -0.5f, 1.0f , 1.0f, 1.0f, 0.0f, 1.0f },
	{ -0.5f, -0.5f, -0.5f, 1.0f , 1.0f, 0.0f, 1.0f, 1.0f },
	{ -0.5f,  0.5f, -0.5f, 1.0f , 0.0f, 1.0f, 1.0f, 1.0f }
	};

	std::vector<GLuint> IndexCollection = {
		0,1,3, 1,2,3,
		0,3,4, 4,7,3,
		1,2,5, 5,6,2,
		3,2,7, 7,2,6,
		0,1,4, 4,5,1,
		4,5,7, 5,6,7
	};

	TestCube = new Mesh(VertexCollection, IndexCollection, glm::vec3(0.0f, 0.0f, 0.0f));
	
	float terrain[5][5] = {};
	for (int x = 0; x < 5; ++x) {
		for (int z = 0; z < 5; ++z) {
			terrain[x][z] = 0;
		}
	}

	glm::vec4 terrainVertPos[5 * 5] = {};
	int i = 0;
	for (int z = 0; z < 5; ++z) {
		for (int x = 0; x < 5; ++x) {
			terrainVertPos[i] = glm::vec4((float)x, terrain[x][z], (float)z, 1.0);
			i++;
		}
	}
	i = 0;
	unsigned int terrainIndexData[4][10];

	for (int z = 0; z < 5 - 1; z++) {
		i = z * 5;
		for (int x = 0; x < 5 * 2; x += 2) {
			terrainIndexData[z][x] = i;
			i++;
		}
		for (int x = 1; x < 5 * 2 + 1; x += 2) {
			terrainIndexData[z][x] = i;
			i++;
		}
	}

	Update();
}

void Renderer::Draw(GLuint _VAO, GLuint _ElementCount)
{
	glBindVertexArray(_VAO);
	glDrawElements(GL_TRIANGLES, _ElementCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Renderer::Update()
{
	float DeltaTime = 0.0f;
	float LastFrame = 0.0f;
	while (!glfwWindowShouldClose(Window)) {
		float CurrentFrame = glfwGetTime();
		DeltaTime = CurrentFrame - LastFrame;
		LastFrame = CurrentFrame;
		glfwSwapBuffers(Window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwPollEvents();
		if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(Window, GLFW_TRUE);
		}
		if (glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS) {
			MainCamera->UpdateTransformKeyboard(MovementType::MOVE_FORWARD, DeltaTime);
		}
		if (glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS) {
			MainCamera->UpdateTransformKeyboard(MovementType::MOVE_BACKWARD, DeltaTime);
		}
		if (glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS) {
			MainCamera->UpdateTransformKeyboard(MovementType::MOVE_LEFT, DeltaTime);
		}
		if (glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS) {
			MainCamera->UpdateTransformKeyboard(MovementType::MOVE_RIGHT, DeltaTime);
		}
		TestCube->ModelMatrix = glm::mat4(1.0f);
		TestCube->ModelMatrix = glm::translate(TestCube->ModelMatrix, TestCube->Position);
		TestCube->ModelMatrix = glm::rotate(TestCube->ModelMatrix, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		MainCamera->UpdateScene(TestCube->ModelMatrix);
		Draw(TestCube->VAO, TestCube->IndexCollection.size());
		//system("PAUSE");
		//Meshes[0]->ModelMatrix = glm::mat4(1.0f);
		//Meshes[0]->ModelMatrix = glm::translate(Meshes[0]->ModelMatrix, Meshes[0]->Position);
		//MainCamera->Update(Meshes[0]->ModelMatrix);

	}
	Terminate();
}

void Renderer::Terminate()
{
	if (MainCamera) {
		if (MainCamera->BasicShader) {
			MainCamera->BasicShader->Clear();
		}
		if (MainCamera->SkyboxShader) {
			MainCamera->SkyboxShader->Clear();
		}
		delete MainCamera;
	}

	glfwDestroyWindow(Window);
	glfwTerminate();
}
