#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>

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
	TreeGen(std::vector<Vertex> _vertexCol, 
		const int _numPoints, const int _maxLevel, const float _r, glm::vec3 _position);
	TreeGen(std::vector<Vertex> _vertexCol, std::vector<unsigned int> _indexCol, glm::vec3 _position);
	~TreeGen();

	void Configure();
	void Draw(Camera*& _camera, Light*& _dirLight);

	std::vector<Vertex> CreateTree(const float _trunkH, const float _trunkR, 
		const int _numPoints, const int _maxLevel, const float _r);
	void ComputeBranch(
		float _r, int _depth, float _angle, 
		float _x0, float _y0, float _z0, 
		float _x1, float _y1, float _z1, 
		float& _x2, float& _y2, float& _z2);
	void RecurComputeBranch(
		std::vector<Vertex>& _vertexCollection,
		float _r, int _maxLevel, int _depth,
		int _index, float _angle,
		std::vector<glm::vec3> _basePts, 
		std::vector<glm::vec3> _brPts);


	void Clear();

	unsigned int m_VAO = 0, m_VBO = 0, m_IBO = 0, m_InstanceBO;

	Shader* m_Shader = NULL;
	const char* m_VertexShaderSource = "../Shaders/TreeVertexShader.glsl";
	const char* m_GeometryShaderSource = "../Shaders/TreeGeometryShader.glsl";
	const char* m_FragmentShaderSource = "../Shaders/TreeFragmentShader.glsl";

	Material* m_Material = NULL;
	Transform* m_Transform = NULL;
	std::vector<Vertex> m_VertexCollection;
	std::vector<unsigned int> m_IndexCollection;
	
	glm::vec3 m_InstancedPositions[10];
	//std::vector<glm::vec4> m_InstancedPositions;
};

