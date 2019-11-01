#include "Renderer.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
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
	//Mesh* TestCube = new Mesh(VertexCollection, IndexCollection, glm::vec3(0.0f, 0.0f, 0.0f));
	//Meshes[0] = TestCube;

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
	while (!glfwWindowShouldClose(Window)) {
		glfwSwapBuffers(Window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwPollEvents();
		if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(Window, GLFW_TRUE);
		}
		if (glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS) {
			MainCamera->UpdateTransform(MovementType::MOVE_FORWARD);
		}
		if (glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS) {
			MainCamera->UpdateTransform(MovementType::MOVE_BACKWARD);
		}
		if (glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS) {
			MainCamera->UpdateTransform(MovementType::MOVE_LEFT);
		}
		if (glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS) {
			MainCamera->UpdateTransform(MovementType::MOVE_RIGHT);
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
