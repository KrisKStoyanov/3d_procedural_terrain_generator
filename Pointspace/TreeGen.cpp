#include "TreeGen.h"

TreeGen::TreeGen(std::vector<Vertex> _vertexCol, const int _trunkLevels, float _heightThreshold,
	const float _trunkHeight, const float _trunkRadius, glm::vec3 _position)
{
	if (_vertexCol.size() > 10) {
		for (int i = 0; i < 10; ++i) {
			m_InstancedPositions[i] = glm::vec3(0.0f, 0.0f, 0.0f);
		}
		for (int i = 0; i < 10;) {
			int randId = static_cast<int> (rand()) % _vertexCol.size() + 1;
			glm::vec3 randCoords = glm::vec3(_vertexCol[randId].Coords.x, _vertexCol[randId].Coords.y, _vertexCol[randId].Coords.z);
			if (randCoords.y > _heightThreshold) {
				m_InstancedPositions[i] = randCoords;
				i++;
			}
			else {
				_heightThreshold -= 0.25f;
			}
		}
	}

	const float halfTrunkHeight = _trunkHeight * (1.f / 2.0f);
	const float halfTrunkRadius = _trunkRadius * (1.f / 2.0f);
	float trunkHeight = halfTrunkHeight + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (_trunkHeight - halfTrunkHeight));
	float trunkRadius = halfTrunkRadius + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (_trunkRadius - halfTrunkRadius));

	const float branchHeight = trunkHeight - trunkRadius;
	const float branchWidth = trunkHeight * (1.f / 2.0f);
	const float branchRadius = trunkRadius * (1.f / 2.0f);

	for (int i = 0; i < _trunkLevels; ++i) {
		CreateTrunk(m_VertexCollection, i * trunkHeight, trunkHeight + i * trunkHeight, trunkRadius);
		CreateBranch(m_VertexCollection, trunkHeight + i * trunkHeight, branchWidth, trunkRadius);
		CreateBranch(m_VertexCollection, trunkHeight + i * trunkHeight, -branchWidth, -trunkRadius);
		CreateBranch(m_VertexCollection, trunkHeight + i * trunkHeight, branchWidth, trunkRadius, false);
		CreateBranch(m_VertexCollection, trunkHeight + i * trunkHeight, -branchWidth, -trunkRadius, false);
		CreateTrunk(m_VertexCollection, (i + 1) * trunkHeight, trunkHeight + (i + 1) * trunkHeight, trunkRadius);
		if (trunkHeight > trunkRadius * 2.0f) {
			trunkHeight -= i * (trunkRadius - (int)trunkRadius);
		}

		if (trunkRadius >= 0.05f) {
			trunkRadius -= trunkRadius * (trunkRadius - (int)trunkRadius);
		}
		else {
			trunkRadius += trunkRadius * (trunkRadius - (int)trunkRadius);
		}
	}

	m_Transform = new Transform(_position);

	Configure();
}

TreeGen::TreeGen(std::vector<Vertex> _vertexCol, std::vector<unsigned int> _indexCol, glm::vec3 _position)
{
	m_VertexCollection = _vertexCol;
	m_IndexCollection = _indexCol;

	m_Transform = new Transform(_position);

	Configure();
}

TreeGen::~TreeGen()
{
	Clear();
}

void TreeGen::Configure()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_VertexCollection.size() * sizeof(Vertex), m_VertexCollection.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(m_VertexCollection[0]), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(m_VertexCollection[0]), (GLvoid*)sizeof(m_VertexCollection[0].Coords));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(m_VertexCollection[0]), (GLvoid*)(sizeof(m_VertexCollection[0].Coords) + sizeof(m_VertexCollection[0].Normal)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	m_Shader = new Shader(m_VertexShaderSource, m_FragmentShaderSource);
}

void TreeGen::Draw(Camera*& _camera, Light*& _dirLight)
{
	m_Shader->Activate();
	m_Transform->Translate(m_Transform->GetPosition());

	m_Shader->SetMat4("u_projectionMatrix", _camera->ProjectionMatrix);
	m_Shader->SetMat4("u_viewMatrix", _camera->ViewMatrix);
	m_Shader->SetMat4("u_modelMatrix", m_Transform->GetModelMatrix());
	for (unsigned int i = 0; i < 10; ++i) {
		std::stringstream ss;
		std::string id;
		ss << i;
		id = ss.str();
		m_Shader->SetVec3(("u_instancedPos[" + id + "]").c_str(), m_InstancedPositions[i]);
	}
	m_Shader->SetFloat("u_time", glfwGetTime());
	glBindVertexArray(m_VAO);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, m_VertexCollection.size(), 10);
	glBindVertexArray(0);
}

void TreeGen::CreateTrunk(std::vector<Vertex>& _vertexCol, const float _trunkStartH, const float _trunkEndH, const float _trunkR)
{
	float height = 4.0f;
	float radius = 0.25f;
	float doublePI = 2 * 3.14159265f;
	int slices = 20;
	float edgeScalar = doublePI * (1.f / slices);
	for (size_t i = 0; i < slices; ++i) {
		_vertexCol.push_back(Vertex(glm::vec4(0.0f, _trunkStartH, 0.0f, 1.0f)));
		_vertexCol.push_back(
			Vertex(glm::vec4(_trunkR * cos(i * edgeScalar), _trunkStartH, _trunkR * sin(i * edgeScalar), 1.0f)));
		_vertexCol.push_back(
			Vertex(glm::vec4(_trunkR * cos((i + 1) * edgeScalar), _trunkStartH, _trunkR * sin((i + 1) * edgeScalar), 1.0f)));
		_vertexCol.push_back(
			Vertex(glm::vec4(_trunkR * cos(i * edgeScalar), _trunkEndH, _trunkR * sin(i * edgeScalar), 1.0f)));
		_vertexCol.push_back(
			Vertex(glm::vec4(_trunkR * cos((i + 1) * edgeScalar), _trunkEndH, _trunkR * sin((i + 1) * edgeScalar), 1.0f)));
		_vertexCol.push_back(Vertex(glm::vec4(0.0f, _trunkEndH, 0.0f, 1.0f)));
	}
}

void TreeGen::CreateBranch(std::vector<Vertex>& _vertexCollection, const float _branchH, const float _branchW, const float _branchR, bool _xDir)
{
	float doublePI = 2 * 3.14159265f;
	int slices = 20;
	float edgeScalar = doublePI * (1.f / slices);

	if (_xDir) {
		for (size_t i = 0; i < slices; ++i) {
			_vertexCollection.push_back(
				Vertex(glm::vec4(_branchR, _branchH, 0.0f, 1.0f)));
			_vertexCollection.push_back(
				Vertex(glm::vec4(_branchR, _branchH + _branchR * cos(i * edgeScalar), _branchR * sin(i * edgeScalar), 1.0f)));
			_vertexCollection.push_back(
				Vertex(glm::vec4(_branchR, _branchH + _branchR * cos((i + 1) * edgeScalar), _branchR * sin((i + 1) * edgeScalar), 1.0f)));
			_vertexCollection.push_back(
				Vertex(glm::vec4(_branchR + _branchW, _branchH + _branchR * cos(i * edgeScalar), _branchR * sin(i * edgeScalar), 1.0f)));
			_vertexCollection.push_back(
				Vertex(glm::vec4(_branchR + _branchW, _branchH + _branchR * cos((i + 1) * edgeScalar), _branchR * sin((i + 1) * edgeScalar), 1.0f)));
			_vertexCollection.push_back(
				Vertex(glm::vec4(_branchR + _branchW, _branchH, 0.0f, 1.0f)));
		}
	}
	else {
		for (size_t i = 0; i < slices; ++i) {
			_vertexCollection.push_back(
				Vertex(glm::vec4(0.0f, _branchH, _branchR, 1.0f)));
			_vertexCollection.push_back(
				Vertex(glm::vec4(_branchR * sin(i * edgeScalar), _branchH + _branchR * cos(i * edgeScalar), _branchR, 1.0f)));
			_vertexCollection.push_back(
				Vertex(glm::vec4(_branchR * sin((i + 1) * edgeScalar), _branchH + _branchR * cos((i + 1) * edgeScalar), _branchR, 1.0f)));
			_vertexCollection.push_back(
				Vertex(glm::vec4(_branchR * sin(i * edgeScalar), _branchH + _branchR * cos(i * edgeScalar), _branchR + _branchW, 1.0f)));
			_vertexCollection.push_back(
				Vertex(glm::vec4(_branchR * sin((i + 1) * edgeScalar), _branchH + _branchR * cos((i + 1) * edgeScalar), _branchR + _branchW, 1.0f)));
			_vertexCollection.push_back(
				Vertex(glm::vec4(0.0f, _branchH, _branchR + _branchW, 1.0f)));
		}
	}
}

void TreeGen::Clear()
{

}
