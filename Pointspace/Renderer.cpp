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
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

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
	DirLight = new Light(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 0.0f, 0.0f));
	//PointLight = new Light(glm::vec3(0.7f, 0.2f, 2.0f), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f);
	//SpotLight = new Light(MainCamera->Position, MainCamera->Front, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(15.0f)));

	ConfigTerrain();
	Update();
}

void Renderer::ConfigTerrain()
{
	//5, 33, 65
	const int RowLength = 33;
	float MaxRandVal = 1;
	const int NumStrips = RowLength - 1;
	const int VerticesPerStrip = 2 * RowLength;
	const int IndexCount = NumStrips * VerticesPerStrip + (RowLength - 2) * 2;
	std::vector<Vertex> TerrainVertexCollection(RowLength * RowLength);
	std::vector<unsigned int> TerrainIndexCollection(IndexCount);
	
	//Build Height Map
	float HeightMap[RowLength][RowLength] = {};
	for (int x = 0; x < RowLength; x++)
	{
		for (int z = 0; z < RowLength; z++)
		{
			HeightMap[x][z] = 0;
		}
	}

	//Perform Diamond Square Algorithm
	srand(4);

	HeightMap[0][0] = (-MaxRandVal + (float)(rand() / (float)RAND_MAX / MaxRandVal * 2));
	HeightMap[0][RowLength - 1] = (-MaxRandVal + (float)(rand() / (float)RAND_MAX / MaxRandVal * 2));
	HeightMap[RowLength - 1][0] = (-MaxRandVal + (float)(rand() / (float)RAND_MAX / MaxRandVal * 2));
	HeightMap[RowLength - 1][RowLength - 1] = (-MaxRandVal + (float)(rand() / (float)RAND_MAX / MaxRandVal * 2));

	int StepSize = RowLength - 1;
	//int CompatibleLength = RowLength % 2 == 0 ? RowLength : RowLength - 1;
	int CompatibleLength = RowLength - 1;
	int Counter = 0;
	int H = 1;

	float H1 = 0, H3 = 0, H5 = 0, H7 = 0, H9 = 0;
	float H2 = 0, H4 = 0, H6 = 0, H8 = 0;

	//[H1][-][H2][-][H3]
	//[-][-][-][-][-]
	//[H4][-][H5][-][H6]
	//[-][-][-][-][-]
	//[H7][-][H8][-][H9]

	while (StepSize > 1) {
		//Perform Diamond step
		for (int x = 0; x < CompatibleLength; x += StepSize) {
			for (int z = 0; z < CompatibleLength; z += StepSize) {
				H1 = HeightMap[x][z];
				H3 = HeightMap[x + StepSize][z];
				H7 = HeightMap[x][z + StepSize];
				H9 = HeightMap[x + StepSize][z + StepSize];
				H5 = (H1 + H3 + H7 + H9) / 4 + (-MaxRandVal + (float)rand() / ((float)RAND_MAX / MaxRandVal));
				HeightMap[x + StepSize / 2][z + StepSize / 2] += H5;
				//	HeightMap[x][z] + HeightMap[(int)(x+StepSize)][z] + HeightMap[x][(int)(z+StepSize)] + HeightMap[(int)(x+StepSize)][(int)(z+StepSize)]
				//	+ ((float)rand() / ((float)RAND_MAX / 1));
			}
		}

		//Perform Square step
		for (int x = 0; x < RowLength - 1; x += StepSize) {
			for (int z = 0; z < RowLength - 1; z += StepSize) {

				H2 = (H1 + H3 + H5) / 3 + (-MaxRandVal + (float)rand() / ((float)RAND_MAX / MaxRandVal));
				H4 = (H1 + H5 + H7) / 3 + (-MaxRandVal + (float)rand() / ((float)RAND_MAX / MaxRandVal));
				H6 = (H3 + H5 + H9) / 3 + (-MaxRandVal + (float)rand() / ((float)RAND_MAX / MaxRandVal));
				H8 = (H7 + H5 + H9) / 3 + (-MaxRandVal + (float)rand() / ((float)RAND_MAX / MaxRandVal));

				HeightMap[x + StepSize / 2][z] += H2;
				HeightMap[x][z + StepSize / 2] += H4;
				HeightMap[x + StepSize][z + StepSize / 2] += H6;
				HeightMap[x + StepSize / 2][z + StepSize] += H8;
				//float TopLeftValue = HeightMap[x][z];
				//float TopRightValue = HeightMap[(int)(x + StepSize)][z];
				//float BottomLeftValue = HeightMap[x][(int)(z + StepSize)];
				//float BottomRightValue = HeightMap[(int)(x + StepSize)][(int)(z + StepSize)];
				//float MiddleValue = HeightMap[(int)(x + StepSize / 2)][(int)(z + StepSize / 2)];
				/*Counter = 0;
				H1 = HeightMap[x][z];
				Counter++;
				H2 = HeightMap[x + StepSize][z];
				Counter++;
				H3 = HeightMap[x + StepSize / 2][z + StepSize / 2];
				Counter++;
				if (z - StepSize / 2 < 0) {
					H4 = 0;
				}
				else {
					H4 = HeightMap[x][z - StepSize / 2];
					Counter++;
				}
				HeightMap[x + (int)(StepSize / 2)][z] = (H1 + H2 + H3 + H4) / (float)(Counter) + ((float)rand() / ((float)RAND_MAX / 1));


				if (z + StepSize / 2 < 0) {
					HeightMap[x][z + StepSize / 2];
				}
				else {
					HeightMap[x][z - StepSize / 2];
				}

				if (z - StepSize / 2 < 0) {
					HeightMap[x][z];
				}

				if (x + StepSize / 2 > RowLength) {
					HeightMap[x][z];
				}*/
				/*HeightMap[x + (int)(StepSize / 2)][z] = (TopLeftValue + TopRightValue + MiddleValue) / 3 + ((float)rand() / ((float)RAND_MAX / 1));
				HeightMap[x + (int)(StepSize/2)][z] = (TopLeftValue + TopRightValue + MiddleValue) / 3 + ((float)rand() / ((float)RAND_MAX / 1));
				HeightMap[x][z + (int)(StepSize / 2)] = (TopLeftValue + BottomLeftValue + MiddleValue) / 3 + ((float)rand() / ((float)RAND_MAX / 1));
				HeightMap[(int)(x + StepSize)][z + (int)(StepSize / 2)] = (TopRightValue + BottomRightValue + MiddleValue) / 3 + ((float)rand() / ((float)RAND_MAX / 1));
				HeightMap[x + (int)(StepSize / 2)][z + (int)(StepSize)] = (BottomLeftValue + BottomRightValue + MiddleValue) / 3 + ((float)rand() / ((float)RAND_MAX / 1));*/
				//}
			}
		}
		MaxRandVal = MaxRandVal * pow(2, -H);
		StepSize /= 2;
	}

	// Build Vertex Coords;
	int i = 0;
	for (int z = 0; z < RowLength; z++)
	{
		for (int x = 0; x < RowLength; x++)
		{
			// Set the coords (1st 4 elements) and a default colour of black (2nd 4 elements) 
			TerrainVertexCollection[i] = Vertex(
				glm::vec4((float)x, HeightMap[z][x], (float)z, 1.0));
			i++;
		}
	}

	struct PlaneTriangle {
		int FirstVertexIndex;
		int SecondVertexIndex;
		int ThirdVertexIndex;
		glm::vec4 FirstVertexCoords;
		glm::vec4 SecondVertexCoords;
		glm::vec4 ThirdVertexCoords;
		glm::vec3 TriNormal;
	};

	//Build Index Collection:
	std::vector<glm::vec3> TriNormalCollection;
	std::vector<PlaneTriangle> TriGroupCollection;
	std::vector<std::vector<glm::vec3>> TriCollection;
	std::vector<int> QuadVertIndexCollection;
	int IndexLow = 0;
	int IndexHigh = RowLength;
	bool Tweaked = false;
	int QuadCounter = 0;


	for (int i = 0; i < IndexCount; i += 2) {
		if (IndexHigh % RowLength == 0 && IndexHigh > RowLength && !Tweaked) {
			TerrainIndexCollection[i] = TerrainIndexCollection[i - 1];
			TerrainIndexCollection[i + 1] = IndexLow;
			Tweaked = true;
			QuadVertIndexCollection.clear();
		}
		else {
			TerrainIndexCollection[i] = IndexLow;
			TerrainIndexCollection[i + 1] = IndexHigh;
			QuadVertIndexCollection.push_back(IndexLow);
			QuadVertIndexCollection.push_back(IndexHigh);
			IndexLow++;
			IndexHigh++;
			Tweaked = false;
		}
		if (QuadVertIndexCollection.size() == 4) {

			PlaneTriangle FirstQuadTri;
			FirstQuadTri.FirstVertexIndex = QuadVertIndexCollection[0];
			FirstQuadTri.SecondVertexIndex = QuadVertIndexCollection[1];
			FirstQuadTri.ThirdVertexIndex = QuadVertIndexCollection[2];
			FirstQuadTri.FirstVertexCoords = TerrainVertexCollection[FirstQuadTri.FirstVertexIndex].Coords;
			FirstQuadTri.SecondVertexCoords = TerrainVertexCollection[FirstQuadTri.SecondVertexIndex].Coords;
			FirstQuadTri.ThirdVertexCoords = TerrainVertexCollection[FirstQuadTri.ThirdVertexIndex].Coords;
			
			PlaneTriangle SecondQuadTri;
			SecondQuadTri.FirstVertexIndex = QuadVertIndexCollection[1];
			SecondQuadTri.SecondVertexIndex = QuadVertIndexCollection[2];
			SecondQuadTri.ThirdVertexIndex = QuadVertIndexCollection[3];
			SecondQuadTri.FirstVertexCoords = TerrainVertexCollection[SecondQuadTri.FirstVertexIndex].Coords;
			SecondQuadTri.SecondVertexCoords = TerrainVertexCollection[SecondQuadTri.SecondVertexIndex].Coords;
			SecondQuadTri.ThirdVertexCoords = TerrainVertexCollection[SecondQuadTri.ThirdVertexIndex].Coords;
			
			FirstQuadTri.TriNormal = 
				glm::cross(
					glm::vec3(FirstQuadTri.FirstVertexCoords - FirstQuadTri.SecondVertexCoords),
					glm::vec3(FirstQuadTri.SecondVertexCoords - FirstQuadTri.ThirdVertexCoords));

			SecondQuadTri.TriNormal = 
				glm::cross(
					glm::vec3(SecondQuadTri.FirstVertexCoords - SecondQuadTri.SecondVertexCoords),
					glm::vec3(SecondQuadTri.SecondVertexCoords - SecondQuadTri.ThirdVertexCoords));
			
			TriGroupCollection.push_back(FirstQuadTri);
			TriGroupCollection.push_back(SecondQuadTri);

			//TriNormalCollection.push_back(FirstQuadTriNormal);
			//TriNormalCollection.push_back(SecondQuadTriNormal);

			//std::vector<glm::vec3> FirstTri(3);
			//FirstTri[0] = glm::vec3(TerrainVertexCollection[QuadVertIndexCollection[0]].Coords);
			//FirstTri[1] = glm::vec3(TerrainVertexCollection[QuadVertIndexCollection[1]].Coords);
			//FirstTri[2] = glm::vec3(TerrainVertexCollection[QuadVertIndexCollection[2]].Coords);
			//std::vector<glm::vec3> SecondTri(3);
			//SecondTri[0] = glm::vec3(TerrainVertexCollection[QuadVertIndexCollection[1]].Coords);
			//SecondTri[1] = glm::vec3(TerrainVertexCollection[QuadVertIndexCollection[2]].Coords);
			//SecondTri[2] = glm::vec3(TerrainVertexCollection[QuadVertIndexCollection[3]].Coords);
			//TriCollection.push_back(FirstTri);
			//TriCollection.push_back(SecondTri);
			//glm::vec3 FirstTriNormal = glm::cross(glm::vec3(FirstTri[0] - FirstTri[1]), glm::vec3(FirstTri[1] - FirstTri[2]));
			//FirstTriNormal = glm::normalize(FirstTriNormal);
			//TriNormalCollection.push_back(FirstTriNormal);
			//glm::vec3 SecondTriNormal = glm::cross(glm::vec3(SecondTri[0] - SecondTri[1]), glm::vec3(SecondTri[1] - SecondTri[2]));
			//SecondTriNormal = glm::normalize(SecondTriNormal);
			//TriNormalCollection.push_back(SecondTriNormal);

			QuadVertIndexCollection[0] = QuadVertIndexCollection[2];
			QuadVertIndexCollection[1] = QuadVertIndexCollection[3];
			QuadVertIndexCollection.pop_back();
			QuadVertIndexCollection.pop_back();
		}
	}

	//Build Vertex Normals:
	//For each triangle check if the index is part of it and add the normal of the triangle to the vertex
	for (int i = 0; i < TerrainIndexCollection.size(); ++i) {
		for (int j = 0; j < TriGroupCollection.size(); ++j) {
			if (i == TriGroupCollection[j].FirstVertexIndex ||
				i == TriGroupCollection[j].SecondVertexIndex ||
				i == TriGroupCollection[j].ThirdVertexIndex) {
				TerrainVertexCollection[i].Normal += TriGroupCollection[j].TriNormal;
			}
		}
	}

	//Normalize the normal value of each vertex
	for (int i = 0; i < TerrainVertexCollection.size(); ++i) {
		/*float temp = glm::dot(-TerrainVertexCollection[i].Normal, WorldUp);*/
		glm::vec3 tempVec = TerrainVertexCollection[i].Normal; 
		if (tempVec.y < 0) {
			TerrainVertexCollection[i].Normal = -tempVec;
		}
		TerrainVertexCollection[i].Normal = glm::normalize(TerrainVertexCollection[i].Normal);
	}

	TerrainMesh = new Mesh(TerrainVertexCollection, TerrainIndexCollection,glm::vec3(0.0f, 0.0f, 0.0f));
	TerrainMesh->m_Material = new Material(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 50.0f);
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_Shader->Activate();
	_Mesh->ModelMatrix = glm::mat4(1.0f);
	_Mesh->ModelMatrix = glm::translate(_Mesh->ModelMatrix, _Mesh->Position);

	_Shader->SetMat4("projMat", MainCamera->ProjectionMatrix);
	_Shader->SetMat4("viewMat", MainCamera->ViewMatrix);
	_Shader->SetMat4("modelMat", _Mesh->ModelMatrix);

	_Shader->SetVec4("light0.ambCols", DirLight->AmbientC);
	_Shader->SetVec4("light0.difCols", DirLight->DiffuseC);
	_Shader->SetVec4("light0.specCols", DirLight->SpecularC);
	_Shader->SetVec4("light0.coords", DirLight->Coords);

	_Shader->SetVec4("terrainFandB.ambRefl", _Mesh->m_Material->AmbientC);
	_Shader->SetVec4("terrainFandB.difRefl", _Mesh->m_Material->DiffuseC);
	_Shader->SetVec4("terrainFandB.specRefl", _Mesh->m_Material->SpecularC);
	_Shader->SetFloat("terrainFandB.shininess", _Mesh->m_Material->Shininess);

	_Shader->SetVec4("globAmb", SceneAmbColor);

	//NormalMatrix = glm::transpose(glm::inverse(glm::mat3(MainCamera->ViewMatrix * _Mesh->ModelMatrix)));
	_Shader->SetMat3("normalMat", NormalMatrix);

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
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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
