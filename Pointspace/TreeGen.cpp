#include "TreeGen.h"

TreeGen::TreeGen(std::vector<Vertex> _vertexCol, float _heightThreshold,
	const float _trunkHeight, const float _trunkRadius, glm::vec3 _position)
{
	if (_vertexCol.size() > 10) {
		int instanceID = 0;
		int countBoundary = 10;
		for (int i = 0; i < countBoundary; ++i) {
			int randId = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (_vertexCol.size()));
			glm::vec3 randCoords = glm::vec3(_vertexCol[randId].Coords.x, _vertexCol[randId].Coords.y, _vertexCol[randId].Coords.z);
			if (randCoords.y > _heightThreshold) {
				m_InstancedPositions[instanceID] = randCoords;
				instanceID++;
			}
			else {
				_heightThreshold -= 1.0f;
				countBoundary++;
			}
		}
	}

	const float halfTrunkHeight = _trunkHeight / 2.0f;
	const float halfTrunkRadius = _trunkRadius / 2.0f;
	const float trunkHeight = halfTrunkHeight + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (_trunkHeight - halfTrunkHeight));
	const float trunkRadius = halfTrunkRadius + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (_trunkRadius - halfTrunkRadius));

	const float branchHeight = trunkHeight - trunkRadius;
	const float branchWidth = trunkHeight / 2.0f;
	const float branchRadius = trunkRadius / 2.0f;
	
	m_VertexCollection = CreateTree(trunkHeight, trunkRadius);
	
	CreateBranch(m_VertexCollection, branchHeight, branchWidth, branchRadius);
	CreateBranch(m_VertexCollection, branchHeight, -branchWidth, branchRadius);
	CreateBranch(m_VertexCollection, branchHeight, branchWidth, branchRadius, false);
	CreateBranch(m_VertexCollection, branchHeight, -branchWidth, branchRadius, false);

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
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindVertexArray(m_VAO);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, m_VertexCollection.size(), 10);
	glBindVertexArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

std::vector<Vertex> TreeGen::CreateTree(const float _trunkH, const float _trunkR)
{
	std::vector<Vertex> trunkVertexCollection;
	float height = 4.0f;
	float radius = 0.25f;
	float doublePI = 2 * 3.14159265f;
	int slices = 20;
	float edgeScalar = doublePI * (1.f / slices);
	for (size_t i = 0; i < slices; ++i) {		
		trunkVertexCollection.push_back(Vertex(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
		trunkVertexCollection.push_back(
			Vertex(glm::vec4(_trunkR * cos(i * edgeScalar), 0.0f, _trunkR * sin(i * edgeScalar), 1.0f)));
		trunkVertexCollection.push_back(
			Vertex(glm::vec4(_trunkR * cos((i + 1) * edgeScalar), 0.0f, _trunkR * sin((i + 1) * edgeScalar), 1.0f)));
		trunkVertexCollection.push_back(
			Vertex(glm::vec4(_trunkR * cos(i * edgeScalar), _trunkH, _trunkR * sin(i * edgeScalar), 1.0f)));
		trunkVertexCollection.push_back(
			Vertex(glm::vec4(_trunkR * cos((i + 1) * edgeScalar), _trunkH, _trunkR * sin((i + 1) * edgeScalar), 1.0f)));
		trunkVertexCollection.push_back(Vertex(glm::vec4(0.0f, _trunkH, 0.0f, 1.0f)));
	}
	return trunkVertexCollection;
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

//void TreeGen::ComputeBranch(float _r, int _depth, float _angle, 
//	float _x0, float _y0, float _z0, 
//	float _x1, float _y1, float _z1, 
//	float& _x2, float& _y2, float& _z2)
//{
//	float xs, ys, zs,
//		xll, yll, zll,
//		xl, yl, zl, m;
//	double val;
//
//	xs = (_x1 - _x0) * _r;
//	ys = (_y1 - _y0) * _r;
//	zs = (_z1 - _z0) * _r;
//
//	m = sqrt(xs * xs + ys * ys + zs * zs);
//	xll = cos(_angle / 2.0f) * xs - sin(_angle / 2.0f) * ys;
//	yll = cos(_angle / 2.0f) * xs + cos(_angle / 2.0f) * ys;
//	zll = 0.0f;
//
//	srand(3);
//	if (_depth % 2 == 0) {
//		val = -2.0f;
//	}
//	else {
//		val = 2.0f;
//	}
//
//	xl = cos(val * _angle / 2.0f) * xll - sin(val * _angle / 2.0f) * zll;
//	yl = yll;
//	zl = sin(val * _angle / 2.0f) * xll + cos(val * _angle / 2.0f) * zll;
//
//	_x2 = _x1 + xl;
//	_y2 = _y1 + yl;
//	_z2 = _z1 + zl;
//}

//void TreeGen::RecurComputeBranch(
//	std::vector<Vertex>& _vertexCollection,
//	float _r, int _maxLevel,  int _depth, 
//	int _index, float _angle, 
//	std::vector<glm::vec3> _basePts, 
//	std::vector<glm::vec3> _brPts)
//{
//	int i, size;
//	float x2, y2, z2;
//	glm::vec3 ttPt;
//	std::vector<glm::vec3> genBasePts, genBrPts;
//	if (_depth > _maxLevel) {
//		return;
//	}
//	size = _basePts.size();
//	if (size == 0) {
//		return;
//	}
//
//	for (int i = 0; i < size; ++i) {
//		//ComputeBranch(_r, _depth, _angle, 
//		//	_basePts[i].x, _basePts[i].y, _basePts[i].z, 
//		//	_brPts[i].x, _brPts[i].y, _brPts[i].z, 
//		//	x2, y2, z2);
//		_vertexCollection[_index].Coords[0] = x2;
//		_vertexCollection[_index].Coords[1] = y2;
//		_vertexCollection[_index].Coords[2] = z2;
//		_vertexCollection[_index].Coords[3] = 1.0f;
//		_index++;
//		genBasePts.push_back(_brPts[i]);
//		ttPt.x = x2; ttPt.y = y2, ttPt.z= z2;
//		genBrPts.push_back(ttPt);
//	}
//
//	_depth++;
//	RecurComputeBranch(_vertexCollection, _r, _maxLevel, _depth, _index, _angle, genBasePts, genBrPts);
//}

void TreeGen::Clear()
{

}
