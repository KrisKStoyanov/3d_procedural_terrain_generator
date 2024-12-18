#pragma once
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <time.h>

#include "Camera.h"
#include "Skybox.h"
#include "Light.h"
#include "Timestep.h"

#include "TerrainGen.h"
#include "WaterGen.h"
#include "CloudGen.h"
#include "TreeGen.h"
#include "GrassGen.h"

class Renderer
{
public:
	Renderer();
	~Renderer();
	void Init(
		const char* _Title,
		const int _Width,
		const int _Height);
	void OnUpdate();
	void Terminate();

	GLFWwindow*		m_Window;

	TerrainGen*		m_TerrainGen;
	WaterGen*		m_WaterGen;
	CloudGen*		m_CloudGen;
	TreeGen*		m_TreeGen;
	GrassGen*		m_GrassGen;

	Skybox*			m_Skybox;
	Camera*			m_Camera;
	Light*			m_DirLight;

	double m_CursorPosX, m_CursorPosY;
	float m_LastFrameTime;
};

