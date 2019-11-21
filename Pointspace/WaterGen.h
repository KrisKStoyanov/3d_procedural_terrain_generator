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

class WaterGen
{
public:
	WaterGen(int _mapSize, glm::vec3 _position, const char* _texturePath);
	~WaterGen();

	void Configure();
	void Draw(Camera*& _camera, Light*& _dirLight, float _deltaTime);

	void Clear();

	unsigned int m_VAO = 0, m_VBO = 0, m_IBO = 0;
	float m_Amplitude = 0.5f;
	float m_Wavelength = 20.0f;
	float m_Speed = 1.0f;
	float m_Steepness = 0.5f; //range limit: 0.0f-1.0f

	Shader* m_Shader = NULL;
	const char* m_VertexShaderSource = "../Shaders/WaterVertexShader.glsl";
	const char* m_FragmentShaderSource = "../Shaders/WaterFragmentShader.glsl";

	Material* m_Material = NULL;
	Transform* m_Transform = NULL;
	std::vector<Vertex> m_VertexCollection;
	std::vector<unsigned int> m_IndexCollection;
	std::vector<Texture*> m_TextureCollection;
};

