#include "Renderer.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::Init(const char* _Title, const int _Width, const int _Height)
{
	glfwInit();
	m_Window = glfwCreateWindow(_Width, _Height, _Title, NULL, NULL);
	if (!m_Window) {
		glfwTerminate();
	}
	glfwMakeContextCurrent(m_Window);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glewExperimental = GL_TRUE;
	GLenum initState = glewInit();
	if (initState != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(initState));
	}

	glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	/*glGetIntegerv(GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS, &m_HW_GeoMaxOutputComp);
	m_MaxGeoVertexCount = floor(m_GeoMaxOutputComp / m_HW_GeoSetOutputComp);*/

	//glGetIntegerv(GL_PATCH_VERTICES, &m_HW_MaxTessVertsPerPatch);
	glPatchParameteri(GL_PATCH_VERTICES, 3);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glEnable(GL_BLEND);

	m_Camera = new Camera(glm::vec3(0.0f, 1.0f, 5.0f), 60, _Width, _Height);
	m_DirLight = new Light(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 0.0f, 0.0f));

	m_TerrainGen = new TerrainGen(33, -8.0f, glm::vec3(0.0f, 0.0f, 0.0f),
		"../Textures/snow.png",
		"../Textures/rock.png",
		"../Textures/grass.bmp",
		"../Textures/sand.png");

	m_WaterGen = new WaterGen(33, glm::vec3(0.0f, 0.0f, 0.0f), "../Textures/water.png");
	m_GrassGen = new GrassGen(m_TerrainGen->m_VertexCollection, m_TerrainGen->m_IndexCollection, glm::vec3(0.0f, 0.0f, 0.0f));
	m_CloudGen = new CloudGen(33, glm::vec3(0.0f, 20.0f, 0.0f), "../Textures/cloud.png");

	m_Skybox = new Skybox();

	OnUpdate();
}

void Renderer::OnUpdate()
{
	while (!glfwWindowShouldClose(m_Window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float time = (float)glfwGetTime();
		Timestep timestep = time - m_LastFrameTime;
		m_LastFrameTime = time;

		glfwPollEvents();
		if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(m_Window, GLFW_TRUE);
		}
		if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS) {
			m_Camera->UpdateTransformKeyboard(MovementType::FORWARD, timestep);
		}
		if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS) {
			m_Camera->UpdateTransformKeyboard(MovementType::BACKWARD, timestep);
		}
		if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS) {
			m_Camera->UpdateTransformKeyboard(MovementType::LEFT, timestep);
		}
		if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS) {
			m_Camera->UpdateTransformKeyboard(MovementType::RIGHT, timestep);
		}
		glfwGetCursorPos(m_Window, &m_CursorPosX, &m_CursorPosY);
		m_Camera->UpdateTransformMouse(m_CursorPosX, -m_CursorPosY);
		//m_TerrainGen->Draw(m_Camera, m_DirLight);
		m_GrassGen->Draw(m_Camera, m_DirLight);
		m_WaterGen->Draw(m_Camera, m_DirLight);
		m_CloudGen->Draw(m_Camera, m_DirLight, timestep);
		m_Skybox->Draw(m_Camera);

		glfwSwapBuffers(m_Window);
	}
	Terminate();
}

void Renderer::Terminate()
{
	if (m_Camera) {
		delete m_Camera;
	}

	if (m_TerrainGen) {
		delete m_TerrainGen;
	}
	if (m_WaterGen) {
		delete m_WaterGen;
	}
	if (m_CloudGen) {
		delete m_CloudGen;
	}
	if (m_TreeGen) {
		delete m_TreeGen;
	}
	if (m_GrassGen) {
		delete m_GrassGen;
	}

	if (m_Skybox) {
		delete m_Skybox;
	}

	glfwDestroyWindow(m_Window);
	glfwTerminate();
}
