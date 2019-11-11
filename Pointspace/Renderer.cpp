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

	glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	MainCamera = new Camera(glm::vec3(0.0f, 1.0f, 5.0f), 60, _Width, _Height);

	Setup();
	return true;
}

void Renderer::Setup()
{
	ModelShader = new Shader("../Shaders/vertexShader.glsl", "../Shaders/fragmentShader.glsl");
	std::vector<std::string> SkyboxCubemapFaces
	{
		"../External Resources/Skybox/miramar_ft.tga",
		"../External Resources/Skybox/miramar_bk.tga",
		"../External Resources/Skybox/miramar_up.tga",
		"../External Resources/Skybox/miramar_dn.tga",
		"../External Resources/Skybox/miramar_rt.tga",
		"../External Resources/Skybox/miramar_lf.tga"
	};
	MainSkybox = new Skybox(SkyboxCubemapFaces, "../Shaders/SkyboxVertexShader.glsl", "../Shaders/SkyboxFragmentShader.glsl");
	
	//PointLight = new Light(glm::vec3(0.7f, 0.2f, 2.0f), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f);
	//SpotLight = new Light(MainCamera->Position, MainCamera->Front, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(15.0f)));

	ConfigTerrain();
	Update();
}

void Renderer::ConfigTerrain()
{
	const int rowLength = 10;
	const int numStripsRequired = rowLength - 1;
	const int verticesPerStrip = 2 * rowLength;
	const int indexCount =  numStripsRequired * verticesPerStrip + (rowLength - 2) * 2;
	std::vector<Vertex> terrainVertices(rowLength * rowLength);
	std::vector<unsigned int> terrainIndices(indexCount);
	
	float terrain[rowLength][rowLength] = {};
	for (int x = 0; x < rowLength; x++)
	{
		for (int z = 0; z < rowLength; z++)
		{
			terrain[x][z] = 0;
		}
	}

	srand(1);

	float step_size = rowLength - 1;
	float rand_max = 2;
	float H = 1;
	int i = 0;

	// Intialise vertex array
	for (int z = 0; z < rowLength; z++)
	{
		for (int x = 0; x < rowLength; x++)
		{
			// Set the coords (1st 4 elements) and a default colour of black (2nd 4 elements) 
			terrainVertices[i] = Vertex(glm::vec4((float)x, terrain[z][x], (float)z, 1.0));
			i++;
		}
	}

	int IndexLow = 0;
	int IndexHigh = rowLength;
	bool tweaked = false;
	for (int i = 0; i < indexCount; i += 2) {
		if (IndexHigh % rowLength == 0 && IndexHigh > rowLength && !tweaked) {
			terrainIndices[i] = terrainIndices[i - 1];
			terrainIndices[i + 1] = IndexLow;
			tweaked = true;
		}
		else {
			terrainIndices[i] = IndexLow;
			terrainIndices[i + 1] = IndexHigh;
			IndexLow++;
			IndexHigh++;
			tweaked = false;
		}
	}
	
	TerrainMesh = new Mesh(terrainVertices, terrainIndices ,glm::vec3(0.0f, 0.0f, 0.0f));
}

void Renderer::ConfigTrees()
{
}

void Renderer::ConfigClouds()
{
}

void Renderer::ConfigWater()
{
}

void Renderer::Draw(Camera* _Camera, Mesh* _Mesh, Shader* _Shader)
{
	_Shader->Activate();
	_Mesh->ModelMatrix = glm::mat4(1.0f);
	_Mesh->ModelMatrix = glm::translate(_Mesh->ModelMatrix, _Mesh->Position);

	_Shader->SetMat4("projMat", MainCamera->ProjectionMatrix);
	_Shader->SetMat4("viewMat", MainCamera->ViewMatrix);
	_Shader->SetMat4("modelMat", _Mesh->ModelMatrix);

	//_Shader->SetVec3("light0.ambCols", DirLight->AmbientC);
	//_Shader->SetVec3("light0.difCols", DirLight->DiffuseC);
	//_Shader->SetVec3("light0.specCols", DirLight->SpecularC);
	//_Shader->SetVec3("light0.coords", DirLight->Direction);

	//_Shader->SetVec3("terrainFandB.ambRefl", _Mesh->m_Material->AmbientC);
	//_Shader->SetVec3("terrainFandB.difRefl", _Mesh->m_Material->DiffuseC);
	//_Shader->SetVec3("terrainFandB.specRefl", _Mesh->m_Material->SpecularC);
	//_Shader->SetFloat("terrainFandB.shininess", _Mesh->m_Material->Shininess);

	glBindVertexArray(_Mesh->VAO);
	glDrawElements(GL_TRIANGLE_STRIP, _Mesh->IndexCollection.size(), GL_UNSIGNED_INT, 0);
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
			MainCamera->UpdateTransformKeyboard(MovementType::FORWARD, DeltaTime);
		}
		if (glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS) {
			MainCamera->UpdateTransformKeyboard(MovementType::BACKWARD, DeltaTime);
		}
		if (glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS) {
			MainCamera->UpdateTransformKeyboard(MovementType::LEFT, DeltaTime);
		}
		if (glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS) {
			MainCamera->UpdateTransformKeyboard(MovementType::RIGHT, DeltaTime);
		}
		glfwGetCursorPos(Window, &PosX, &PosY);
		MainCamera->UpdateTransformMouse(PosX, -PosY);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		if (TerrainMesh != NULL) {
			Draw(MainCamera, TerrainMesh, ModelShader);
		}
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDepthFunc(GL_LEQUAL);
		MainSkybox->Draw(MainCamera);
		glDepthFunc(GL_LESS);
	}
	Terminate();
}

void Renderer::Terminate()
{
	if (MainCamera) {
		MainCamera = nullptr;
	}

	if (ModelShader) {
		delete ModelShader;
	}
	if (MainSkybox) {
		delete MainSkybox;
	}

	glfwDestroyWindow(Window);
	glfwTerminate();
}
