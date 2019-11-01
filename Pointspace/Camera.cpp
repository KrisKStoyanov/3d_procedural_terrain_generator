#include "Camera.h"

Camera::Camera(glm::vec3 _Position, float _FOV, float _ProjWidth, float _ProjHeight)
{
	Position = _Position;
	Front = glm::vec3(0.0f, 0.0f, -1.0f);
	Direction = glm::normalize(Position - Target);
	WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	Right = glm::normalize(glm::cross(WorldUp, Direction));
	Up = glm::cross(Direction, Right);

	ViewMatrix = glm::lookAt(Position, Position + Front, Up);
	ProjectionMatrix = glm::perspective(glm::radians(_FOV), _ProjWidth * (1.f/_ProjHeight), 0.1f, 1000.0f);
}

Camera::~Camera()
{
}

void Camera::SetupShader(const GLchar* _VertexShaderPath, const GLchar* _FragmentShaderPath)
{
	MainShader = new Shader(_VertexShaderPath, _FragmentShaderPath);
	MainShader->Activate();
	if (MainShader) {
		ProjectionMatrixLoc = glGetUniformLocation(MainShader->ProgramID, "ProjectionMatrix");
		ViewMatrixLoc = glGetUniformLocation(MainShader->ProgramID, "ViewMatrix");
		ModelMatrixLoc = glGetUniformLocation(MainShader->ProgramID, "ModelMatrix");
	}
}

void Camera::Update(glm::mat4 _ModelMatrix)
{
	glUniformMatrix4fv(ProjectionMatrixLoc, 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
	glUniformMatrix4fv(ViewMatrixLoc, 1, GL_FALSE, glm::value_ptr(ViewMatrix));
	glUniformMatrix4fv(ModelMatrixLoc, 1, GL_FALSE, glm::value_ptr(_ModelMatrix));
}
