#include "Transform.h"

Transform::Transform(glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale)
{
	m_Position = _position;
	m_Rotation = _rotation;
	m_Scale = _scale;
}

Transform::~Transform()
{
}

void Transform::Translate(glm::vec3 _translation)
{
	//Update position
	m_Position = _translation;
	//Set to identity matrix
	m_ModelMatrix = glm::mat4(1.0f);
	//Move object through matrix set with the argument vector
	m_ModelMatrix = glm::translate(m_ModelMatrix, m_Position);
}
void Transform::Rotate(float _angle, glm::vec3 _rotationAxis)
{
	//Set to identity matrix
	m_ModelMatrix = glm::mat4(1.0f);
	m_ModelMatrix = glm::translate(m_ModelMatrix, m_Position);
	m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(_angle), _rotationAxis);
}
void Transform::Scale(glm::vec3 _scalingRatio)
{
	//Set to identity matrix
	m_ModelMatrix = glm::mat4(1.0f);
	m_ModelMatrix = glm::translate(m_ModelMatrix, m_Position);
	m_ModelMatrix = glm::scale(m_ModelMatrix, _scalingRatio);
}
void Transform::LookAt(Transform _target, glm::vec3 _worldUp)
{
}
