#pragma once
#include <iostream>
#include <fstream>

#include "Vertex.h"
#include "Shader.h"
#include "Material.h"
#include "Texture.h"
#include "Transform.h"

#include "Camera.h"
#include "Light.h"

class GrassGen
{
public:
	GrassGen(int _mapSize, float _randomRange, glm::vec3 _position);
	GrassGen(std::vector<Vertex> _vertexCol, std::vector<unsigned int> _indexCol, glm::vec3 _position);
	~GrassGen();

	void Configure();
	void Draw(Camera*& _camera, Light*& _dirLight);

	void Clear();

	unsigned int m_VAO = 0, m_VBO = 0, m_IBO = 0;

	Shader* m_Shader = NULL;
	const char* m_VertexShaderSource = "../Shaders/GrassVertexShader.glsl";
	const char* m_TCShaderSource = "../Shaders/GrassTCShader.glsl";
	const char* m_TEShaderSource = "../Shaders/GrassTEShader.glsl";
	const char* m_GeometryShaderSource = "../Shaders/GrassGeometryShader.glsl";
	const char* m_FragmentShaderSource = "../Shaders/GrassFragmentShader.glsl";

	Material* m_Material = NULL;
	Transform* m_Transform = NULL;
	std::vector<Vertex> m_VertexCollection;
	std::vector<unsigned int> m_IndexCollection;

	unsigned int m_InnerTL = 3;
	unsigned int m_OuterTL = 2;
};

