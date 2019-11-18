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
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

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

void Renderer::DiamondStep(float** _heightMap, int _x, int _z, int _stepSize, float _randomRange, int _rowLength)
{
	int halfstepSize = _stepSize / 2;
	float avg = 0;
	int counter = 0;

	//Top left
	if (_x - halfstepSize >= 0 && _z - halfstepSize >= 0) {
		avg += _heightMap[_x - halfstepSize][_z - halfstepSize];
		counter++;
	}

	//Top right
	if (_x + halfstepSize < _rowLength && _z - halfstepSize >= 0) {
		avg += _heightMap[_x + halfstepSize][_z - halfstepSize];
		counter++;
	}

	//Bottom left
	if (_x - halfstepSize >= 0 && _z + halfstepSize < _rowLength) {
		avg += _heightMap[_x - halfstepSize][_z + halfstepSize];
		counter++;
	}

	//Bottom right
	if (_x + halfstepSize < _rowLength && _z + halfstepSize < _rowLength) {
		avg += _heightMap[_x + halfstepSize][_z + halfstepSize];
		counter++;
	}
	_heightMap[_x][_z] += avg / counter + (_randomRange - static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (_randomRange + _randomRange)));
}

void Renderer::SquareStep(float** _heightMap, int _x, int _z, int _stepSize, float _randomRange, int _rowLength)
{
	int halfstepSize = _stepSize / 2;
	float avg = 0;
	int counter = 0;

	//Right
	if (_x + halfstepSize < _rowLength) {
		avg += _heightMap[_x + halfstepSize][_z];
		counter++;
	}

	//Left
	if (_x - halfstepSize >= 0) {
		avg += _heightMap[_x - halfstepSize][_z];
		counter++;
	}

	//Bottom
	if (_z + halfstepSize < _rowLength) {
		avg += _heightMap[_x][_z + halfstepSize];
		counter++;
	}

	//Top
	if (_z - halfstepSize >= 0) {
		avg += _heightMap[_x][_z - halfstepSize];
		counter++;
	}
	_heightMap[_x][_z] += avg / counter + (_randomRange - static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (_randomRange + _randomRange)));
}

void Renderer::ConfigTerrain()
{
	//5, 33, 65
	const int rowLength = 33;
	//Max corresponds to inverse min value
	float minRandVal = 2.0f;
	const int numStrips = rowLength - 1;
	const int verticesPerStrip = 2 * rowLength;
	const int indexCount = numStrips * verticesPerStrip + (rowLength - 2) * 2;
	std::vector<Vertex> terrainVertexCollection(rowLength * rowLength);
	std::vector<unsigned int> terrainIndexCollection(indexCount);

	//Build Height Map
	float** heightMap = new float* [rowLength];
	for (int i(0); i < rowLength; ++i) {
		heightMap[i] = new float[rowLength];
	}

	for (int x(0); x < rowLength; ++x) {
		for (int z(0); z < rowLength; ++z) {
			heightMap[x][z] = 0;
		}
	}
	//Perform Diamond Square Algorithm
	srand(323);

	float test = (minRandVal - static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (minRandVal + minRandVal)));

	int stepSize = rowLength - 1;
	int compatibleLength = rowLength - 1;
	float H = 1;
	//heightMap[0][0] = (-minRandVal + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (minRandVal + minRandVal)));
	//heightMap[stepSize][0] = (-minRandVal + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (minRandVal + minRandVal)));
	//heightMap[0][stepSize] = (-minRandVal + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (minRandVal + minRandVal)));
	//heightMap[stepSize][stepSize] = (-minRandVal + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (minRandVal + minRandVal)));
	while (stepSize > 1) {
		for (int x = 0; x < compatibleLength; x += stepSize) {
			for (int z = 0; z < compatibleLength; z += stepSize) {
				DiamondStep(heightMap, x + stepSize/2, z + stepSize/2, stepSize, minRandVal, rowLength);
			}
		}

		for (int x = 0; x < compatibleLength; x += stepSize) {
			for (int z = 0; z < compatibleLength; z += stepSize) {
				SquareStep(heightMap, x + stepSize / 2, z, stepSize, minRandVal, rowLength);
				SquareStep(heightMap, x + stepSize / 2, z + stepSize, stepSize, minRandVal, rowLength);
				SquareStep(heightMap, x, z + stepSize / 2, stepSize, minRandVal, rowLength);
				SquareStep(heightMap, x + stepSize, z + stepSize / 2, stepSize, minRandVal, rowLength);
			}
		}
		minRandVal *= pow(2, -H);
		stepSize /= 2;
	}

	// Build Vertex Coords;
	float fTextureS = float(rowLength) * 0.1f;
	float fTextureT = float(rowLength) * 0.1f;

	int i = 0;
	for (int x = 0; x < rowLength; x++)
	{
		for (int z = 0; z < rowLength; z++)
		{
			float fScaleC = (float)(x) / (float)(rowLength - 1);
			float fScaleR = (float)(z) / (float)(rowLength - 1);
			// Set the coords (1st 4 elements) and a default colour of black (2nd 4 elements) 
			terrainVertexCollection[i] = Vertex(
				glm::vec4((float)x, heightMap[x][z], (float)z, 1.0),
				glm::vec2(fTextureS * fScaleC, fTextureT * fScaleR));
			i++;
		}
	}

	//for (int i = 0; i < terrainVertexCollection.size(); ++i) {
	//	if (terrainVertexCollection[i].Coords.y < 1) {
	//		float lul = 1;
	//	}
	//}

	struct QuadTriangle {
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
	std::vector<QuadTriangle> TriGroupCollection;
	std::vector<std::vector<glm::vec3>> TriCollection;
	std::vector<int> QuadVertIndexCollection;
	int IndexLow = 0;
	int IndexHigh = rowLength;
	bool Tweaked = false;
	int QuadCounter = 0;


	for (int i = 0; i < indexCount; i += 2) {
		if (IndexHigh % rowLength == 0 && IndexHigh > rowLength && !Tweaked) {
			terrainIndexCollection[i] = terrainIndexCollection[i - 1];
			terrainIndexCollection[i + 1] = IndexLow;
			Tweaked = true;
			QuadVertIndexCollection.clear();
		}
		else {
			terrainIndexCollection[i] = IndexLow;
			terrainIndexCollection[i + 1] = IndexHigh;
			//Push poinds for quad formation on non-degenerate triangles
			QuadVertIndexCollection.push_back(IndexLow);
			QuadVertIndexCollection.push_back(IndexHigh);
			IndexLow++;
			IndexHigh++;
			Tweaked = false;
		}
		if (QuadVertIndexCollection.size() == 4) {

			//Create the quad out of two triangles through the obtained indecies
			QuadTriangle UpperQuadTri;
			UpperQuadTri.FirstVertexIndex = QuadVertIndexCollection[0];
			UpperQuadTri.SecondVertexIndex = QuadVertIndexCollection[1];
			UpperQuadTri.ThirdVertexIndex = QuadVertIndexCollection[2];
			UpperQuadTri.FirstVertexCoords = terrainVertexCollection[UpperQuadTri.FirstVertexIndex].Coords;
			UpperQuadTri.SecondVertexCoords = terrainVertexCollection[UpperQuadTri.SecondVertexIndex].Coords;
			UpperQuadTri.ThirdVertexCoords = terrainVertexCollection[UpperQuadTri.ThirdVertexIndex].Coords;
			
			QuadTriangle LowerQuadTri;
			LowerQuadTri.FirstVertexIndex = QuadVertIndexCollection[1];
			LowerQuadTri.SecondVertexIndex = QuadVertIndexCollection[2];
			LowerQuadTri.ThirdVertexIndex = QuadVertIndexCollection[3];
			LowerQuadTri.FirstVertexCoords = terrainVertexCollection[LowerQuadTri.FirstVertexIndex].Coords;
			LowerQuadTri.SecondVertexCoords = terrainVertexCollection[LowerQuadTri.SecondVertexIndex].Coords;
			LowerQuadTri.ThirdVertexCoords = terrainVertexCollection[LowerQuadTri.ThirdVertexIndex].Coords;
			
			UpperQuadTri.TriNormal =
				glm::cross(
					glm::vec3(UpperQuadTri.ThirdVertexCoords - UpperQuadTri.FirstVertexCoords),
					glm::vec3(UpperQuadTri.FirstVertexCoords - UpperQuadTri.SecondVertexCoords));

			LowerQuadTri.TriNormal =
				glm::cross(
					glm::vec3(LowerQuadTri.FirstVertexCoords - LowerQuadTri.ThirdVertexCoords),
					glm::vec3(LowerQuadTri.ThirdVertexCoords - LowerQuadTri.SecondVertexCoords));
			
			TriGroupCollection.push_back(UpperQuadTri);
			TriGroupCollection.push_back(LowerQuadTri);

			QuadVertIndexCollection[0] = QuadVertIndexCollection[2];
			QuadVertIndexCollection[1] = QuadVertIndexCollection[3];
			QuadVertIndexCollection.pop_back();
			QuadVertIndexCollection.pop_back();
		}
	}

	//Build Vertex Normals:
	//For each triangle check if the index is part of it and
	//add the normal of the triangle to the vertex which the index represents
	for (int i = 0; i < terrainIndexCollection.size(); ++i) {
		for (int j = 0; j < TriGroupCollection.size(); ++j) {
			if (i == TriGroupCollection[j].FirstVertexIndex ||
				i == TriGroupCollection[j].SecondVertexIndex ||
				i == TriGroupCollection[j].ThirdVertexIndex) {
				terrainVertexCollection[i].Normal = TriGroupCollection[j].TriNormal;
			}
		}
	}

	//Normalize the normal value of each vertex
	for (int i = 0; i < terrainVertexCollection.size(); ++i) {
		/*float temp = glm::dot(-terrainVertexCollection[i].Normal, WorldUp);*/
		glm::vec3 tempVec = terrainVertexCollection[i].Normal; 
		/*if (tempVec.y < 0) {
			terrainVertexCollection[i].Normal = -tempVec;
		}*/
		terrainVertexCollection[i].Normal = glm::normalize(-terrainVertexCollection[i].Normal);
	}

	TerrainMesh = new Mesh(terrainVertexCollection, terrainIndexCollection,glm::vec3(0.0f, 0.0f, 0.0f));
	TerrainMesh->m_Material = new Material(
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 
		50.0f);
	TerrainMesh->m_Texture = new Texture("../Textures/grass.bmp", ModelShader);
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
	NormalMatrix = glm::transpose(glm::inverse(glm::mat3(MainCamera->ViewMatrix * _Mesh->ModelMatrix)));

	_Shader->SetMat4("projMat", MainCamera->ProjectionMatrix);
	_Shader->SetMat4("viewMat", MainCamera->ViewMatrix);
	_Shader->SetMat4("modelMat", _Mesh->ModelMatrix);
	_Shader->SetMat3("normalMat", NormalMatrix);

	_Shader->SetVec4("light0.ambCols", DirLight->AmbientC);
	_Shader->SetVec4("light0.difCols", DirLight->DiffuseC);
	_Shader->SetVec4("light0.specCols", DirLight->SpecularC);
	_Shader->SetVec4("light0.coords", DirLight->Coords);

	_Shader->SetVec4("terrainFandB.ambRefl", _Mesh->m_Material->AmbientC);
	_Shader->SetVec4("terrainFandB.difRefl", _Mesh->m_Material->DiffuseC);
	_Shader->SetVec4("terrainFandB.specRefl", _Mesh->m_Material->SpecularC);
	_Shader->SetFloat("terrainFandB.shininess", _Mesh->m_Material->Shininess);

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
		if (TerrainMesh != NULL) {
			Draw(MainCamera, TerrainMesh, ModelShader);
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
		delete ModelShader;
	}
	if (MainSkybox) {
		delete MainSkybox;
	}

	glfwDestroyWindow(Window);
	glfwTerminate();
}
