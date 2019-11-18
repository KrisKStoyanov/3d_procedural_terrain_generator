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

#include "getbmp.h"

class Renderer
{
public:
	Renderer();
	~Renderer();
	bool Init(
		const char* _Title,
		const int _Width,
		const int _Height);
	void Setup();
	void Draw(Camera* _Camera, Mesh* _Mesh, Shader* _Shader);
	void DiamondStep(float ** _heightMap, int _x, int _z, int _stepSize, float _randomRange, int _mapSize);
	void SquareStep(float** _heightMap, int _x, int _z, int _stepSize, float _randomRange, int _mapSize);
	void SquareStepAlt(float** _heightMap, int _x, int _z, int _stepSize, float _randomRange, int _mapSize);
	void Update();
	void Terminate();

	Shader* ModelShader = NULL;

	GLFWwindow* Window = NULL;
	Camera* MainCamera = NULL;
	Mesh* TerrainMesh = NULL;
	Skybox* MainSkybox = NULL;

	Light* DirLight = NULL;

	glm::vec4 SceneAmbColor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
	glm::mat3 NormalMatrix = glm::mat3(1.0f);

	void ConfigTerrain();
	void ConfigTrees();
	void ConfigClouds();
	void ConfigWater();

	double PosX, PosY;
};

