#include "Camera.h"

Camera::Camera(glm::vec3 _Position, float _FOV, float _ProjWidth, float _ProjHeight)
{
	Position = _Position;
	Up = WorldUp = glm::vec3(0.f, 1.f, 0.f);
	Front = glm::vec3(0.f, 0.f, -1.f);
	Right = glm::vec3(1.f, 0.f, 0.f);

	Yaw = -90.0f;
	Pitch = 0.0f;
	MovementSpeed = 5.f;
	MouseSensitivity = 0.1f;

	ProjectionMatrix = glm::perspective(glm::radians(_FOV), _ProjWidth * (1.f / _ProjHeight), 0.1f, 5000.0f);
	UpdateFrontDirection();
}

Camera::~Camera()
{
}

void Camera::UpdateTransformKeyboard(MovementType _Type, float _DeltaTime)
{
	float velocity = MovementSpeed * _DeltaTime;
	switch (_Type) {
	case MovementType::FORWARD:
		Position += Front * velocity;
		break;
	case MovementType::BACKWARD:
		Position += -Front * velocity;
		break;
	case MovementType::LEFT:
		Position += -Right * velocity;
		break;
	case MovementType::RIGHT:
		Position += Right * velocity;
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
	Front = glm::normalize(UpdatedFrontDir);
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));

	ViewMatrix = glm::lookAt(Position, Position + Front, Up);
}

