#include "CloudGen.h"

CloudGen::CloudGen(int _mapSize, float _randRange, float _frequency, glm::vec3 _position, const char* _texturePath)
{
	const int numStrips = _mapSize - 1;
	const int verticesPerStrip = 2 * _mapSize;
	const int vertexCount = _mapSize * _mapSize;
	const int indexCount = numStrips * verticesPerStrip + (_mapSize - 2) * 2;
	std::vector<Vertex> vertexCollection(vertexCount);
	std::vector<unsigned int> indexCollection(indexCount);

	srand(time(NULL));

	//Value noise
	//float* vNoiseMap = new float[_mapSize];
	//for (int i = 0; i < _mapSize * _mapSize; ++i) {
	//	vNoiseMap[i] = -_randRange + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (_randRange - -_randRange)));
	//}
	float** vNoiseMap = new float*[_mapSize];
	for (int i = 0; i < _mapSize; ++i) {
		vNoiseMap[i] = new float[_mapSize];
	}
	for (unsigned int z = 0; z < _mapSize; ++z) {
		for (unsigned int x = 0; x < _mapSize; ++x) {
			vNoiseMap[z][x] = -_randRange + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (_randRange - -_randRange)));
		}
	}
	//float** noiseMap = new float* [_mapSize];
	//for (int i = 0; i < _mapSize; ++i) {
	//	noiseMap[i] = new float[_mapSize];
	//}
	/*noiseMap[0][0] = -_randRange + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (_randRange + _randRange)));
	noiseMap[0][_mapSize - 1] = -_randRange + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (_randRange + _randRange)));
	noiseMap[_mapSize - 1][0] = -_randRange + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (_randRange + _randRange)));
	noiseMap[_mapSize - 1][_mapSize - 1] = -_randRange + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (_randRange + _randRange)));*/
	//Seed map with random values
	//for (unsigned int z = 0; z < _mapSize; ++z) {
	//	for (unsigned int x = 0; x < _mapSize; ++x) {
	//		noiseMap[z][x] = -_randRange + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (_randRange - -_randRange)));
	//	}
	//}

	float** noiseSamples = new float* [_mapSize];
	for (int i = 0; i < _mapSize; ++i) {
		noiseSamples[i] = new float[_mapSize];
	}

	for (unsigned int z = 0; z < _mapSize; ++z) {
		for (unsigned int x = 0; x < _mapSize; ++x) {
			//vNoiseMap[z * _mapSize + x] = ValueNoise(vNoiseMap, x * _frequency,z * _frequency, _mapSize);
			noiseSamples[z][x] = ValueNoise(vNoiseMap, x * _frequency, z * _frequency, _mapSize);
		}
	}

	
	float fTextureS = float(_mapSize) * 0.1f;
	float fTextureT = float(_mapSize) * 0.1f;

	int i = 0;
	for (int z = 0; z < _mapSize; z++)
	{
		for (int x = 0; x < _mapSize; x++)
		{
			float fScaleC = (float)(x) / (float)(_mapSize - 1);
			float fScaleR = (float)(z) / (float)(_mapSize - 1);
			// Set the coords (1st 4 elements) and a default colour of black (2nd 4 elements) 
			vertexCollection[i] = Vertex(
				glm::vec4((float)x, noiseSamples[z][x], (float)z, 1.0),
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
			indexCollection[i] = indexCollection[i - 1];
			indexCollection[i + 1] = IndexLow;
			Tweaked = true;
			QuadVertIndexCollection.clear();
		}
		else {
			indexCollection[i] = IndexLow;
			indexCollection[i + 1] = IndexHigh;
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
			UpperQuadTri.FirstVertexCoords = vertexCollection[UpperQuadTri.FirstVertexIndex].Coords;
			UpperQuadTri.SecondVertexCoords = vertexCollection[UpperQuadTri.SecondVertexIndex].Coords;
			UpperQuadTri.ThirdVertexCoords = vertexCollection[UpperQuadTri.ThirdVertexIndex].Coords;

			QuadTriangle LowerQuadTri;
			LowerQuadTri.FirstVertexIndex = QuadVertIndexCollection[1];
			LowerQuadTri.SecondVertexIndex = QuadVertIndexCollection[2];
			LowerQuadTri.ThirdVertexIndex = QuadVertIndexCollection[3];
			LowerQuadTri.FirstVertexCoords = vertexCollection[LowerQuadTri.FirstVertexIndex].Coords;
			LowerQuadTri.SecondVertexCoords = vertexCollection[LowerQuadTri.SecondVertexIndex].Coords;
			LowerQuadTri.ThirdVertexCoords = vertexCollection[LowerQuadTri.ThirdVertexIndex].Coords;

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
	for (int i = 0; i < indexCollection.size(); ++i) {
		for (int j = 0; j < TriGroupCollection.size(); ++j) {
			if (i == TriGroupCollection[j].FirstVertexIndex ||
				i == TriGroupCollection[j].SecondVertexIndex ||
				i == TriGroupCollection[j].ThirdVertexIndex) {
				vertexCollection[i].Normal += TriGroupCollection[j].TriNormal;
			}
		}
	}

	//Normalize the normal value of each vertex
	for (int i = 0; i < vertexCollection.size(); ++i) {
		/*float temp = glm::dot(-terrainVertexCollection[i].Normal, WorldUp);*/
		//glm::vec3 tempVec = vertexCollection[i].Normal;
		/*if (tempVec.y < 0) {
			terrainVertexCollection[i].Normal = -tempVec;
		}*/
		vertexCollection[i].Normal = glm::normalize(-vertexCollection[i].Normal);
	}

	m_VertexCollection = vertexCollection;
	m_IndexCollection = indexCollection;

	m_Transform = new Transform(_position);
	m_Material = new Material(
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		50.0f);
	m_TextureCollection.push_back(new Texture(_texturePath));
	Configure();
}

CloudGen::~CloudGen()
{
	Clear();
}

float CloudGen::ValueNoise(float**& _noiseMap, const float _xPos, const float _zPos, const int _mapSize)
{
	int xOffset = std::floor(_xPos);
	int zOffset = std::floor(_zPos);

	float xSample = _xPos - xOffset;
	float zSample = _zPos - zOffset;

	int sampleIdLowX = xOffset & _mapSize;
	int sampleIdHighX = sampleIdLowX == _mapSize - 1 ?
		0 : sampleIdLowX + 1;

	int sampleIdLowZ = zOffset & _mapSize;
	int sampleIdHighZ = sampleIdLowZ == _mapSize - 1 ?
		0 : sampleIdLowZ + 1;

	const float sampleValLowX = _noiseMap[sampleIdLowZ][sampleIdLowX];
	const float sampleValHighX = _noiseMap[sampleIdLowZ][sampleIdHighX];
	const float sampleValLowZ = _noiseMap[sampleIdHighZ][sampleIdLowX];
	const float sampleValHighZ = _noiseMap[sampleIdHighZ][sampleIdHighX];

	float smoothXSample = Smoothstep(xSample);
	float smoothZSample = Smoothstep(zSample);

	float lerpSampleLowX = Lerp(sampleValLowX, sampleValHighX, smoothXSample);
	float lerpSampleHighX = Lerp(sampleValLowZ, sampleValHighZ, smoothXSample);

	float lerpSample = Lerp(lerpSampleLowX, lerpSampleHighX, smoothZSample);
	return lerpSample;
	//Calculate value of sample at set location:
}

void CloudGen::Configure()
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

void CloudGen::Draw(Camera*& _camera, Light*& _dirLight, float _deltaTime)
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
	m_Shader->SetFloat("u_time", glfwGetTime());
	for (int i = 0; i < m_TextureCollection.size(); ++i) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_TextureCollection[i]->m_ID);
	}
	glDisable(GL_CULL_FACE);
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLE_STRIP, m_IndexCollection.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glEnable(GL_CULL_FACE);
}

void CloudGen::Clear()
{
	for (int i = 0; i < m_TextureCollection.size(); ++i) {
		delete m_TextureCollection[i];
	}
}
