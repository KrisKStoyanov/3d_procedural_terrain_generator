#include "Camera.h"

Camera::Camera(glm::vec3 _Position, float _FOV, float _ProjWidth, float _ProjHeight)
{
	m_Transform = new Transform(_Position);

	Yaw = -90.0f;
	Pitch = 0.0f;
	MovementSpeed = 3.f;
	MouseSensitivity = 0.1f;

	ProjectionMatrix = glm::perspective(glm::radians(_FOV), _ProjWidth * (1.f / _ProjHeight), 0.1f, 9000.0f);
	UpdateFrontDirection();
}

Camera::~Camera()
{
}

void Camera::UpdateTransformKeyboard(MovementType _Type, float _DeltaTime)
{
	float velocity = MovementSpeed * 20.0f * _DeltaTime;
	switch (_Type) {
	case MovementType::FORWARD:
		m_Transform->SetPosition(m_Transform->GetPosition() + m_Transform->GetRotation() * velocity);
		break;
	case MovementType::BACKWARD:
		m_Transform->SetPosition(m_Transform->GetPosition() - m_Transform->GetRotation() * velocity);
		break;
	case MovementType::LEFT:
		m_Transform->SetPosition(m_Transform->GetPosition() - m_Transform->GetRightDir() * velocity);
		break;
	case MovementType::RIGHT:
		m_Transform->SetPosition(m_Transform->GetPosition() + m_Transform->GetRightDir() * velocity);
		break;
	default:
		break;
	}
}

void Camera::UpdateTransformMouse(GLfloat _CursorX, GLfloat _CursorY)
{
	if (FirstMouse) {
		LastMousePosX = _CursorX;
		LastMousePosY = _CursorY;
		FirstMouse = false;
	}

	float OffsetX = _CursorX - LastMousePosX;
	float OffsetY = _CursorY - LastMousePosY;

	LastMousePosX = _CursorX;
	LastMousePosY = _CursorY;

	OffsetX *= MouseSensitivity;
	OffsetY *= MouseSensitivity;

	Yaw += OffsetX;
	Pitch += OffsetY;

	Pitch = Pitch > 89.f ? 89.f : Pitch;
	Pitch = Pitch < -89.f ? -89.f : Pitch;

	UpdateFrontDirection();
}

void Camera::UpdateFrontDirection()
{
	glm::vec3 UpdatedFrontDir;
	UpdatedFrontDir.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	UpdatedFrontDir.y = sin(glm::radians(Pitch));
	UpdatedFrontDir.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	m_Transform->SetRotation(glm::normalize(glm::normalize(UpdatedFrontDir)));

	ViewMatrix = glm::lookAt(m_Transform->GetPosition(), m_Transform->GetPosition() + m_Transform->GetRotation(), m_Transform->GetUpDir());
}
