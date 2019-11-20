#pragma once
#include <iostream>
#include <fstream>

#include "Vertex.h"
#include "Shader.h"
#include "Material.h"
#include "Texture.h"
#include "Transform.h"

#include "Camera.h"

constexpr int PI = 3.14159265;

class WaterGen
{
public:
	WaterGen();
	~WaterGen();

	void OnUpdate(float _deltaTime);

	void Configure();
	void Draw(Camera*& _camera);

	void Clear();

	unsigned int VAO = 0, VBO = 0, IBO = 0;

	Shader* m_Shader = NULL;
	const char* m_VertexShaderSource = "../Shaders/TerrainVertexShader.glsl";
	const char* m_FragmentShaderSource = "../Shaders/TerrainFragmentShader.glsl";

	Material* m_Material = NULL;
	Transform* m_Transform = NULL;
	std::vector<Vertex> m_VertexCollection;
	std::vector<unsigned int> m_IndexCollection;
	std::vector<Texture*> m_TextureCollection;
};

