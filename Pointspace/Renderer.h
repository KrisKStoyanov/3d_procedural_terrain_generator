#pragma once
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <time.h>

#include "Shader.h"
#include "Vertex.h"
#include "Mesh.h"
#include "Camera.h"
#include "Skybox.h"
#include "Light.h"
#include "Timestep.h"

#include "getbmp.h"

constexpr int PI = 3.14159265;

struct QuadTriangle {
	int FirstVertexIndex;
	int SecondVertexIndex;
	int ThirdVertexIndex;
	glm::vec4 FirstVertexCoords;
	glm::vec4 SecondVertexCoords;
	glm::vec4 ThirdVertexCoords;
	glm::vec3 TriNormal;
};

class Renderer
{
public:
	Renderer();
	~Renderer();
	void Init(
		const char* _Title,
		const int _Width,
		const int _Height);
	void Setup();
	void Draw(Camera*& _Camera, Mesh*& _Mesh, Shader*& _Shader);
	void DiamondStep(float ** _heightMap, int _x, int _z, int _stepSize, float _randomRange, int _mapSize);
	void SquareStep(float** _heightMap, int _x, int _z, int _stepSize, float _randomRange, int _mapSize);
	void UpdateWaterMesh(Mesh*& _Mesh, float _deltaTime);
	void OnUpdate();
	void Terminate();

	Shader* ModelShader = NULL;

	Mesh* TerrainMesh = NULL;
	Mesh* WaterMesh = NULL;

	GLFWwindow* Window = NULL;
	Camera* MainCamera = NULL;
	Skybox* MainSkybox = NULL;

	Light* DirLight = NULL;

	glm::vec4 SceneAmbColor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
	glm::mat3 NormalMatrix = glm::mat3(1.0f);

	void ConfigTerrain();
	void ConfigWater();
	void ConfigTrees();
	void ConfigClouds();

	double m_CursorPosX, m_CursorPosY;
	float m_LastFrameTime = 0.0f;
};

