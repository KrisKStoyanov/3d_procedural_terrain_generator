#include "TreeGen.h"

TreeGen::TreeGen()
{

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

void TreeGen::Draw(Camera*& _camera, Light*& _dirLight, float _deltaTime)
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

	//for (int i = 0; i < m_VertexCollection.size(); ++i) {
	//	m_VertexCollection[i].Coords.x +=
	//		sin(glfwGetTime() * PI / 180) * _deltaTime;
	//	m_VertexCollection[i].Coords.y +=
	//		cos(glfwGetTime() * PI / 180) * _deltaTime;
	//}

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	//glBufferData(GL_ARRAY_BUFFER, m_VertexCollection.size() * sizeof(Vertex), m_VertexCollection.data(), GL_STATIC_DRAW);
	glDrawElements(GL_TRIANGLE_STRIP, m_IndexCollection.size(), GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void TreeGen::Clear()
{
	for (int i = 0; i < m_TextureCollection.size(); ++i) {
		delete m_TextureCollection[i];
	}
}
