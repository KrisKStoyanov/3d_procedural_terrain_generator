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
	std::vector<Vertex> VertexCollection = {
	{ { 0.5f,  0.5f, 0.5f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
	{ { 0.5f, -0.5f, 0.5f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } },
	{ { -0.5f, -0.5f, 0.5f, 1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f } },
	{ { -0.5f,  0.5f, 0.5f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } },
	{ { 0.5f,  0.5f, -0.5f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
	{ { 0.5f, -0.5f, -0.5f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } },
	{ { -0.5f, -0.5f, -0.5f, 1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f } },
	{ { -0.5f,  0.5f, -0.5f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } }
	};

	std::vector<GLuint> IndexCollection = {
		0,1,3, 1,2,3,
		0,3,4, 4,7,3,
		1,2,5, 5,6,2,
		3,2,7, 7,2,6,
		0,1,4, 4,5,1,
		4,5,7, 5,6,7
	};

	TestCube = new Mesh(VertexCollection, IndexCollection, glm::vec3(0.0f, 0.0f, -5.0f));

	ModelShader = SetupShader("../Shaders/BasicVertexShader.glsl", "../Shaders/BasicFragmentShader.glsl");
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
	DirLight = new Light(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.5f, 0.5f, 0.5f));
	PointLight = new Light(glm::vec3(0.7f, 0.2f, 2.0f), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f);
	SpotLight = new Light(MainCamera->Position, MainCamera->Front, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(15.0f)));

	ConfigTerrain();
	Update();
}

void Renderer::ConfigTerrain()
{
	const int RowLength = 5;
	std::vector<Vertex> terrainVertices(RowLength * RowLength);
	const int numStripsRequired = RowLength - 1;
	const int verticesPerStrip = 2 * RowLength;
	std::vector<GLuint> terrainIndices(numStripsRequired * verticesPerStrip + (RowLength-2)*2);
	GLuint terrainIndexData[numStripsRequired][verticesPerStrip];

	float terrain[RowLength][RowLength] = {};
	for (int x = 0; x < RowLength; x++)
	{
		for (int z = 0; z < RowLength; z++)
		{
			terrain[x][z] = 0;
		}
	}

	srand(1);

	//Initialization:
	//terrain[0][0] = -1 + (float)rand() / ((float)RAND_MAX / 2);
	//terrain[0][MAP_SIZE-1] = -1 + (float)rand() / ((float)RAND_MAX / 2);
	//terrain[MAP_SIZE-1][0] = -1 + (float)rand() / ((float)RAND_MAX / 2);
	//terrain[MAP_SIZE - 1][MAP_SIZE - 1] = -1 + (float)rand() / ((float)RAND_MAX / 2);

	float step_size = RowLength - 1;
	float rand_max = 2;
	float H = 1;

	//while (step_size > 1) {
	//	for (int x = 0; x < RowLength - 1; x += step_size) {
	//		for (int y = 0; y < RowLength - 1; y += step_size) {
	//			float avgSum = (terrain[x][y] + terrain[(int)(x + step_size)][y] + terrain[x][(int)(y + step_size)] + terrain[(int)(x + step_size)][(int)(y + step_size)]) / 4;
	//			terrain[(int)(x + step_size / 2)][(int)(y + step_size / 2)] = avgSum + (-1 + (float)rand() / ((float)RAND_MAX / rand_max));
	//		}
	//	}
	//	for (int x = 0; x < RowLength - 1; x += step_size) {
	//		for (int y = 0; y < RowLength - 1; y += step_size) {
	//			float avgSum = (terrain[(int)(x + step_size/2)][y] + terrain[x][(int)(y + step_size/2)] + terrain[(int)(x + step_size)][(int)(y + step_size/2)] + terrain[(int)(x + step_size/2)][(int)(y + step_size)]) / 4;
	//			terrain[x][y] = avgSum + (-1 + (float)rand() / ((float)RAND_MAX / rand_max));
	//		}
	//	}
	//	rand_max = rand_max * glm::pow(2, -H);
	//	step_size = step_size / 2;
	//}
	// TODO: Add your code here to calculate the height values of the terrain using the Diamond square algorithm


	// Intialise vertex array
	int i = 0;
	for (int x = 0; x < RowLength; x++)
	{
		for (int z = 0; z < RowLength; z++)
		{
			// Set the coords (1st 4 elements) and a default colour of black (2nd 4 elements) 
			terrainVertices[i] = Vertex({ glm::vec4((float)x, terrain[x][z], (float)z, 1.0), glm::vec4(0.0, 0.0, 0.0, 1.0) });
			i++;
		}
	}

	i = 0;
	for (int z = 0; z < numStripsRequired; z++)
	{
		i = z * RowLength;
		for (int x = 0; x < verticesPerStrip; x += 2)
		{
			terrainIndexData[z][x] = i;
			i++;
		}
		for (int x = 1; x < RowLength * 2 + 1; x += 2)
		{
			terrainIndexData[z][x] = i;
			i++;
		}
	}

	i = 0;
	for (int z = 0; z < numStripsRequired; z++)
	{
		i = z * RowLength;
		for (int x = 0; x < verticesPerStrip; x+=2) {
			terrainIndices[x + z * verticesPerStrip] = i;
			i++;
		}
		for (int x = 1; x < verticesPerStrip + 1; x += 2) {
			terrainIndices[x + z * verticesPerStrip] = i;
			i++;
		}
		terrainIndices[verticesPerStrip + z * verticesPerStrip + 1] = terrainIndices[verticesPerStrip + z * verticesPerStrip];
	}

	//for (int i = 0; i < numStripsRequired * verticesPerStrip;) {
	//	terrainIndices[i] = a;
	//	terrainIndices[i + 1] = a + MAP_SIZE;
	//	a++;
	//	i += 2;
	//}

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

	_Shader->SetMat4("ProjectionMatrix", MainCamera->ProjectionMatrix);
	_Shader->SetMat4("ViewMatrix", MainCamera->ViewMatrix);
	_Shader->SetMat4("ModelMatrix", _Mesh->ModelMatrix);

	_Shader->SetVec3("ViewPos", MainCamera->Position);
	for (GLuint j = 0; j < _Mesh->TextureCollection.size(); ++j) {
		glActiveTexture(GL_TEXTURE0 + j);
		std::string type = _Mesh->TextureCollection[j].Type;
		_Shader->SetInt("g_Material." + type, j);
		glBindTexture(GL_TEXTURE_2D, _Mesh->TextureCollection[j].ID);
	}
	_Shader->SetFloat("g_Material.Shininess", 64.0f);

	_Shader->SetVec3("g_DirLight.Position", DirLight->Position);
	_Shader->SetVec3("g_DirLight.AmbientC", DirLight->AmbientC);
	_Shader->SetVec3("g_DirLight.DiffuseC", DirLight->DiffuseC);
	_Shader->SetVec3("g_DirLight.SpecularC", DirLight->SpecularC);

	_Shader->SetVec3("g_PointLight.Position", PointLight->Position);
	_Shader->SetVec3("g_PointLight.AmbientC", PointLight->AmbientC);
	_Shader->SetVec3("g_PointLight.DiffuseC", PointLight->DiffuseC);
	_Shader->SetVec3("g_PointLight.SpecularC", PointLight->SpecularC);
	_Shader->SetFloat("g_PointLight.ConstantA", PointLight->ConstantA);
	_Shader->SetFloat("g_PointLight.LinearA", PointLight->LinearA);
	_Shader->SetFloat("g_PointLight.QuadraticA", PointLight->QuadraticA);

	_Shader->SetVec3("g_SpotLight.Position", PointLight->Position);
	_Shader->SetVec3("g_SpotLight.AmbientC", PointLight->AmbientC);
	_Shader->SetVec3("g_SpotLight.DiffuseC", PointLight->DiffuseC);
	_Shader->SetVec3("g_SpotLight.SpecularC", PointLight->SpecularC);
	_Shader->SetFloat("g_SpotLight.ConstantA", PointLight->ConstantA);
	_Shader->SetFloat("g_SpotLight.LinearA", PointLight->LinearA);
	_Shader->SetFloat("g_SpotLight.QuadraticA", PointLight->QuadraticA);
	_Shader->SetFloat("g_SpotLight.CutOff", SpotLight->CutOff);
	_Shader->SetFloat("g_SpotLight.OuterCutOff", SpotLight->OuterCutOff);

	glBindVertexArray(_Mesh->VAO);
	glDrawElements(GL_TRIANGLES, _Mesh->IndexCollection.size(), GL_UNSIGNED_INT, 0);
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
		if (TestCube != NULL) {
			Draw(MainCamera, TestCube, ModelShader);
		}
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
		ModelShader->Clear();
		ModelShader = nullptr;
	}
	if (MainSkybox) {
		MainSkybox->Clear();
		MainSkybox = nullptr;
	}

	glfwDestroyWindow(Window);
	glfwTerminate();
}

Shader* Renderer::SetupShader(const GLchar* _VertexShaderPath, const GLchar* _FragmentShaderPath)
{
	Shader* TempShader = new Shader(_VertexShaderPath, _FragmentShaderPath);
	if (TempShader == NULL) {
		printf("Failed to create shader!\nVertex shader filepath:%s\nFragment shader filepath:%s\n", _VertexShaderPath, _FragmentShaderPath);
		return NULL;
	}
	return TempShader;
}
