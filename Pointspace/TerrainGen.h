#pragma once
#include "Vertex.h"
#include "Shader.h"
#include "Material.h"
#include "Texture.h"
#include "Transform.h"

#include "Camera.h"
#include "Light.h"

#include <chrono>
#include <thread>

#include <time.h>

class TerrainGen
{
public:
	TerrainGen(int _mapSize, float _randomRange, glm::vec3 _position, 
		const char* _snowTexPath, 
		const char* _rockTexPath,
		const char* _grassTexPath, 
		const char* _sandTexPath);
	~TerrainGen();
	
	void DiamondStep(float**& _heightMap, int _x, int _z, int _stepSize, float _randomRange, int _mapSize);
	void SquareStep(float**& _heightMap, int _x, int _z, int _stepSize, float _randomRange, int _mapSize);

	void Configure();
	void Draw(Camera*& _camera, Light*& _dirLight);

	void Clear();

	unsigned int m_VAO = 0, m_VBO = 0, m_IBO = 0;

	Shader* m_Shader = NULL;
	const char* m_VertexShaderSource = "../Shaders/TerrainVertexShader.glsl";
	const char* m_TCShaderSource = "../Shaders/TerrainTCShader.glsl";
	const char* m_TEShaderSource = "../Shaders/TerrainTEShader.glsl";
	const char* m_GeometryShaderSource = "../Shaders/TerrainGeometryShader.glsl";
	const char* m_FragmentShaderSource = "../Shaders/TerrainFragmentShader.glsl";

	float** m_HeightMap = NULL;

	Material* m_Material = NULL;
	Transform* m_Transform = NULL;
	std::vector<Vertex> m_VertexCollection;
	std::vector<unsigned int> m_IndexCollection;

	Texture* m_SnowTexture = NULL;
	Texture* m_RockTexture = NULL;
	Texture* m_GrassTexture = NULL;
	Texture* m_SandTexture = NULL;

	unsigned int m_InnerTL = 1;
	unsigned int m_OuterTL = 1;
};

