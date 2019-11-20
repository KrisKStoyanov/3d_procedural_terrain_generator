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
	TerrainGen(int _mapSize, float _randomRange, int _threadCount);
	~TerrainGen();
	
	void DiamondStep(float** _heightMap, int _x, int _z, int _stepSize, float _randomRange, int _mapSize);
	void SquareStep(float** _heightMap, int _x, int _z, int _stepSize, float _randomRange, int _mapSize);

	void Configure();
	void Draw(Camera*& _camera, Light*& _dirLight);

	void Clear();

	unsigned int m_VAO = 0, m_VBO = 0, m_IBO = 0;

	Shader* m_Shader = NULL;
	const char* m_VertexShaderSource = "../Shaders/TerrainVertexShader.glsl";
	const char* m_FragmentShaderSource = "../Shaders/TerrainFragmentShader.glsl";

	Material* m_Material = NULL;
	Transform* m_Transform = NULL;
	std::vector<Vertex> m_VertexCollection;
	std::vector<unsigned int> m_IndexCollection;
	std::vector<Texture*> m_TextureCollection;
};

