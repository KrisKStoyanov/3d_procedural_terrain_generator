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
#include <time.h>
#include <fstream>

class CloudGen
{
public:
	CloudGen(int _mapSize, float _randRange, float _frequency, glm::vec3 _position, const char* _texturePath);
	~CloudGen();

	float ValueNoise(float*& _noiseMap, const float _xPos, const float _zPos, const int _mapSize);

	inline float Smoothstep(const float& _t) { return _t * _t * (3 - 2 * _t); }
	inline float Lerp(const float& _low, const float& _high, const float& _t) { return _low * (1 - _t) + _high * _t; }
	void Configure();
	void Draw(Camera*& _camera, Light*& _dirLight, float _deltaTime);

	void Clear();

	unsigned int m_VAO = 0, m_VBO = 0, m_IBO = 0;

	Shader* m_Shader = NULL;
	const char* m_VertexShaderSource = "../Shaders/CloudVertexShader.glsl";
	const char* m_FragmentShaderSource = "../Shaders/CloudFragmentShader.glsl";

	Material* m_Material = NULL;
	Transform* m_Transform = NULL;
	std::vector<Vertex> m_VertexCollection;
	std::vector<unsigned int> m_IndexCollection;
	std::vector<Texture*> m_TextureCollection;
};

