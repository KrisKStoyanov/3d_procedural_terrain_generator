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

class TreeGen
{
public:
	TreeGen(int _mapSize, float _randomRange, glm::vec3 _position);
	TreeGen(std::vector<Vertex> _vertexCol, std::vector<unsigned int> _indexCol, glm::vec3 _position);
	~TreeGen();

	void Configure();
	void Draw(Camera*& _camera, Light*& _dirLight);

	void Clear();

	unsigned int m_VAO = 0, m_VBO = 0, m_IBO = 0;

	Shader* m_Shader = NULL;
	const char* m_VertexShaderSource = "../Shaders/TreeVertexShader.glsl";
	const char* m_GeometryShaderSource = "../Shaders/TreeGeometryShader.glsl";
	const char* m_FragmentShaderSource = "../Shaders/TreeFragmentShader.glsl";

	Material* m_Material = NULL;
	Transform* m_Transform = NULL;
	std::vector<Vertex> m_VertexCollection;
	std::vector<unsigned int> m_IndexCollection;
};

