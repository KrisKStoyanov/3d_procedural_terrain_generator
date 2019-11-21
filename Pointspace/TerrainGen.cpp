#include "TerrainGen.h"

TerrainGen::TerrainGen(int _mapSize, float _randomRange, glm::vec3 _position, const char* _texturePath)
{
	const int numStrips = _mapSize - 1;
	const int verticesPerStrip = 2 * _mapSize;
	const int indexCount = numStrips * verticesPerStrip + (_mapSize - 2) * 2;
	std::vector<Vertex> terrainVertexCollection(_mapSize * _mapSize);
	std::vector<unsigned int> terrainIndexCollection(indexCount);

	//Build Height Map
	float** heightMap = new float* [_mapSize];
	for (int i(0); i < _mapSize; ++i) {
		heightMap[i] = new float[_mapSize];
	}

	for (int x(0); x < _mapSize; ++x) {
		for (int z(0); z < _mapSize; ++z) {
			heightMap[x][z] = 0;
		}
	}

	//Perform Diamond Square Algorithm
	srand(time(NULL));

	float test = (-_randomRange - static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (_randomRange + _randomRange)));

	int stepSize = _mapSize - 1;
	int iterations = log2(_mapSize);
	int compatibleLength = 1;
	float H = 1;
	heightMap[0][0] = (-_randomRange + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (_randomRange + _randomRange)));
	heightMap[stepSize][0] = (-_randomRange + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (_randomRange + _randomRange)));
	heightMap[0][stepSize] = (-_randomRange + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (_randomRange + _randomRange)));
	heightMap[stepSize][stepSize] = (-_randomRange + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (_randomRange + _randomRange)));

	for (int i = 0; i < iterations; i++) {
		int row = 0;
		for (int x = 0; x < compatibleLength; x++) {
			int col = 0;
			for (int z = 0; z < compatibleLength; z++) {
				DiamondStep(heightMap, row + stepSize / 2, col + stepSize / 2, stepSize, _randomRange, _mapSize);
				col += stepSize;
			}
			row += stepSize;
		}

		row = 0;
		for (int x = 0; x < compatibleLength; x++) {
			int col = 0;
			for (int z = 0; z < compatibleLength; z++) {
				SquareStep(heightMap, row + stepSize / 2, col, stepSize, _randomRange, _mapSize);
				SquareStep(heightMap, row + stepSize / 2, col + stepSize, stepSize, _randomRange, _mapSize);
				SquareStep(heightMap, row, col + stepSize / 2, stepSize, _randomRange, _mapSize);
				SquareStep(heightMap, row + stepSize, col + stepSize / 2, stepSize, _randomRange, _mapSize);
				col += stepSize;
			}
			row += stepSize;
		}
		_randomRange /= 2;
		stepSize /= 2;
		compatibleLength *= 2;
	}

	// Build Vertex Coords;
	float fTextureS = float(_mapSize) * 0.1f;
	float fTextureT = float(_mapSize) * 0.1f;

	int i = 0;
	for (int x = 0; x < _mapSize; x++)
	{
		for (int z = 0; z < _mapSize; z++)
		{
			float fScaleC = (float)(x) / (float)(_mapSize - 1);
			float fScaleR = (float)(z) / (float)(_mapSize - 1);
			// Set the coords (1st 4 elements) and a default colour of black (2nd 4 elements) 
			terrainVertexCollection[i] = Vertex(
				glm::vec4((float)x, heightMap[x][z], (float)z, 1.0),
				glm::vec2(fTextureS * fScaleC, fTextureT * fScaleR));
			i++;
		}
	}

	struct QuadTriangle {
		int FirstVertexIndex;
		int SecondVertexIndex;
		int ThirdVertexIndex;
		glm::vec4 FirstVertexCoords;
		glm::vec4 SecondVertexCoords;
		glm::vec4 ThirdVertexCoords;
		glm::vec3 TriNormal;
	};

	//Build Index Collection:
	std::vector<glm::vec3> TriNormalCollection;
	std::vector<QuadTriangle> TriGroupCollection;
	std::vector<std::vector<glm::vec3>> TriCollection;
	std::vector<int> QuadVertIndexCollection;
	int IndexLow = 0;
	int IndexHigh = _mapSize;
	bool Tweaked = false;
	int QuadCounter = 0;


	for (int i = 0; i < indexCount; i += 2) {
		if (IndexHigh % _mapSize == 0 && IndexHigh > _mapSize && !Tweaked) {
			terrainIndexCollection[i] = terrainIndexCollection[i - 1];
			terrainIndexCollection[i + 1] = IndexLow;
			Tweaked = true;
			QuadVertIndexCollection.clear();
		}
		else {
			terrainIndexCollection[i] = IndexLow;
			terrainIndexCollection[i + 1] = IndexHigh;
			//Push poinds for quad formation on non-degenerate triangles
			QuadVertIndexCollection.push_back(IndexLow);
			QuadVertIndexCollection.push_back(IndexHigh);
			IndexLow++;
			IndexHigh++;
			Tweaked = false;
		}
		if (QuadVertIndexCollection.size() == 4) {

			//Create the quad out of two triangles through the obtained indecies
			QuadTriangle UpperQuadTri;
			UpperQuadTri.FirstVertexIndex = QuadVertIndexCollection[0];
			UpperQuadTri.SecondVertexIndex = QuadVertIndexCollection[1];
			UpperQuadTri.ThirdVertexIndex = QuadVertIndexCollection[2];
			UpperQuadTri.FirstVertexCoords = terrainVertexCollection[UpperQuadTri.FirstVertexIndex].Coords;
			UpperQuadTri.SecondVertexCoords = terrainVertexCollection[UpperQuadTri.SecondVertexIndex].Coords;
			UpperQuadTri.ThirdVertexCoords = terrainVertexCollection[UpperQuadTri.ThirdVertexIndex].Coords;

			QuadTriangle LowerQuadTri;
			LowerQuadTri.FirstVertexIndex = QuadVertIndexCollection[1];
			LowerQuadTri.SecondVertexIndex = QuadVertIndexCollection[2];
			LowerQuadTri.ThirdVertexIndex = QuadVertIndexCollection[3];
			LowerQuadTri.FirstVertexCoords = terrainVertexCollection[LowerQuadTri.FirstVertexIndex].Coords;
			LowerQuadTri.SecondVertexCoords = terrainVertexCollection[LowerQuadTri.SecondVertexIndex].Coords;
			LowerQuadTri.ThirdVertexCoords = terrainVertexCollection[LowerQuadTri.ThirdVertexIndex].Coords;

			UpperQuadTri.TriNormal =
				glm::cross(
					glm::vec3(UpperQuadTri.ThirdVertexCoords - UpperQuadTri.FirstVertexCoords),
					glm::vec3(UpperQuadTri.FirstVertexCoords - UpperQuadTri.SecondVertexCoords));

			LowerQuadTri.TriNormal =
				glm::cross(
					glm::vec3(LowerQuadTri.FirstVertexCoords - LowerQuadTri.ThirdVertexCoords),
					glm::vec3(LowerQuadTri.ThirdVertexCoords - LowerQuadTri.SecondVertexCoords));

			TriGroupCollection.push_back(UpperQuadTri);
			TriGroupCollection.push_back(LowerQuadTri);

			QuadVertIndexCollection[0] = QuadVertIndexCollection[2];
			QuadVertIndexCollection[1] = QuadVertIndexCollection[3];
			QuadVertIndexCollection.pop_back();
			QuadVertIndexCollection.pop_back();
		}
	}

	//Build Vertex Normals:
	//For each triangle check if the index is part of it and
	//add the normal of the triangle to the vertex which the index represents
	for (int i = 0; i < terrainIndexCollection.size(); ++i) {
		for (int j = 0; j < TriGroupCollection.size(); ++j) {
			if (i == TriGroupCollection[j].FirstVertexIndex ||
				i == TriGroupCollection[j].SecondVertexIndex ||
				i == TriGroupCollection[j].ThirdVertexIndex) {
				terrainVertexCollection[i].Normal += TriGroupCollection[j].TriNormal;
			}
		}
	}

	//Normalize the normal value of each vertex
	for (int i = 0; i < terrainVertexCollection.size(); ++i) {
		/*float temp = glm::dot(-terrainVertexCollection[i].Normal, WorldUp);*/
		glm::vec3 tempVec = terrainVertexCollection[i].Normal;
		/*if (tempVec.y < 0) {
			terrainVertexCollection[i].Normal = -tempVec;
		}*/
		terrainVertexCollection[i].Normal = glm::normalize(-terrainVertexCollection[i].Normal);
	}

	m_VertexCollection = terrainVertexCollection;
	m_IndexCollection = terrainIndexCollection;

	m_Transform = new Transform(_position);
	m_Material = new Material(
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		50.0f);
	m_TextureCollection.push_back(new Texture(_texturePath));
	Configure();
}

TerrainGen::~TerrainGen()
{
	Clear();
}

void TerrainGen::DiamondStep(float** _heightMap, int _x, int _z, int _stepSize, float _randomRange, int _mapSize)
{
	int halfstepSize = _stepSize / 2;
	float avg = 0;
	int counter = 0;

	//Top left
	if (_x - halfstepSize >= 0 && _z - halfstepSize >= 0) {
		avg += _heightMap[_x - halfstepSize][_z - halfstepSize];
		counter++;
	}

	//Top right
	if (_x + halfstepSize < _mapSize && _z - halfstepSize >= 0) {
		avg += _heightMap[_x + halfstepSize][_z - halfstepSize];
		counter++;
	}

	//Bottom left
	if (_x - halfstepSize >= 0 && _z + halfstepSize < _mapSize) {
		avg += _heightMap[_x - halfstepSize][_z + halfstepSize];
		counter++;
	}

	//Bottom right
	if (_x + halfstepSize < _mapSize && _z + halfstepSize < _mapSize) {
		avg += _heightMap[_x + halfstepSize][_z + halfstepSize];
		counter++;
	}
	_heightMap[_x][_z] = avg / counter + (-_randomRange + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (_randomRange + _randomRange)));
}

void TerrainGen::SquareStep(float** _heightMap, int _x, int _z, int _stepSize, float _randomRange, int _mapSize)
{
	int halfstepSize = _stepSize / 2;
	float avg = 0;
	int counter = 0;

	//Right
	if (_x + halfstepSize < _mapSize) {
		avg += _heightMap[_x + halfstepSize][_z];
		counter++;
	}

	//Left
	if (_x - halfstepSize >= 0) {
		avg += _heightMap[_x - halfstepSize][_z];
		counter++;
	}

	//Bottom
	if (_z + halfstepSize < _mapSize) {
		avg += _heightMap[_x][_z + halfstepSize];
		counter++;
	}

	//Top
	if (_z - halfstepSize >= 0) {
		avg += _heightMap[_x][_z - halfstepSize];
		counter++;
	}
	_heightMap[_x][_z] = avg / counter + (-_randomRange + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (_randomRange + _randomRange)));
}

void TerrainGen::Configure()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_VertexCollection.size() * sizeof(Vertex), m_VertexCollection.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_IndexCollection.size() * sizeof(unsigned int), m_IndexCollection.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(m_VertexCollection[0]), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(m_VertexCollection[0]), (GLvoid*)sizeof(m_VertexCollection[0].Coords));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(m_VertexCollection[0]), (GLvoid*)(sizeof(m_VertexCollection[0].Coords) + sizeof(m_VertexCollection[0].Normal)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	m_Shader = new Shader(m_VertexShaderSource, m_FragmentShaderSource);
}

void TerrainGen::Draw(Camera*& _camera, Light*& _dirLight)
{
	m_Shader->Activate();
	m_Transform->Translate(m_Transform->GetPosition());
	m_Transform->SetNormalMatrix(glm::transpose(glm::inverse(glm::mat3(_camera->ViewMatrix * m_Transform->GetModelMatrix()))));

	m_Shader->SetMat4("u_projectionMatrix", _camera->ProjectionMatrix);
	m_Shader->SetMat4("u_viewMatrix", _camera->ViewMatrix);
	m_Shader->SetMat4("u_modelMatrix", m_Transform->GetModelMatrix());
	m_Shader->SetMat3("u_normalMatrix", m_Transform->GetNormalMatrix());

	m_Shader->SetVec4("u_dirLight.ambCols", _dirLight->AmbientC);
	m_Shader->SetVec4("u_dirLight.difCols", _dirLight->DiffuseC);
	m_Shader->SetVec4("u_dirLight.specCols", _dirLight->SpecularC);
	m_Shader->SetVec4("u_dirLight.coords", _dirLight->Coords);

	m_Shader->SetVec4("u_material.ambRefl", m_Material->AmbientC);
	m_Shader->SetVec4("u_material.difRefl", m_Material->DiffuseC);
	m_Shader->SetVec4("u_material.specRefl", m_Material->SpecularC);
	m_Shader->SetFloat("u_material.shininess", m_Material->Shininess);

	m_Shader->SetInt("u_texSampler", 0);

	for (int i = 0; i < m_TextureCollection.size(); ++i) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_TextureCollection[i]->m_ID);
	}
	
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLE_STRIP, m_IndexCollection.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void TerrainGen::Clear()
{
	for (int i = 0; i < m_TextureCollection.size(); ++i) {
		delete m_TextureCollection[i];
	}
}


