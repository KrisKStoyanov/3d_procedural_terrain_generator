#include "Camera.h"

Camera::Camera(glm::vec3 _Position, float _FOV, float _ProjWidth, float _ProjHeight)
{
	Position = _Position;
	Front = glm::vec3(0.0f, 0.0f, -1.0f);
	Direction = glm::normalize(Position);
	WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	Right = glm::normalize(glm::cross(WorldUp, Direction));
	Up = glm::cross(Direction, Right);

	MovementSpeed = 0.01f;

	ViewMatrix = glm::lookAt(Position, Position + Front, Up);
	ProjectionMatrix = glm::perspective(glm::radians(_FOV), _ProjWidth * (1.f/_ProjHeight), 0.1f, 1000.0f);
}

Camera::~Camera()
{
}

void Camera::SetupShader(const GLchar* _VertexShaderPath, const GLchar* _FragmentShaderPath, ShaderType _Type)
{
	switch (_Type) {
	case ShaderType::BASIC:
		BasicShader = new Shader(_VertexShaderPath, _FragmentShaderPath);
		if (BasicShader) {
			ProjectionMatrixLoc = glGetUniformLocation(BasicShader->ProgramID, "ProjectionMatrix");
			ViewMatrixLoc = glGetUniformLocation(BasicShader->ProgramID, "ViewMatrix");
			ModelMatrixLoc = glGetUniformLocation(BasicShader->ProgramID, "ModelMatrix");
		}
		break;
	case ShaderType::SKYBOX:
		SkyboxShader = new Shader(_VertexShaderPath, _FragmentShaderPath);
		if (SkyboxShader) {
			//glGenTextures(1, &TextureID);
			//glBindTexture(GL_TEXTURE_CUBE_MAP, TextureID);

			//int width, height, nrChannels;
			//unsigned char* data;
			//for(GLuint i = 0; i < )

			ProjectionMatrixLoc = glGetUniformLocation(SkyboxShader->ProgramID, "ProjectionMatrix");
			ViewMatrixLoc = glGetUniformLocation(SkyboxShader->ProgramID, "ViewMatrix");
		}
	default:
		break;
	}
}

void Camera::UpdateScene(glm::mat4 _ModelMatrix)
{
	glUniformMatrix4fv(ProjectionMatrixLoc, 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
	glUniformMatrix4fv(ViewMatrixLoc, 1, GL_FALSE, glm::value_ptr(ViewMatrix));
	glUniformMatrix4fv(ModelMatrixLoc, 1, GL_FALSE, glm::value_ptr(_ModelMatrix));
}

void Camera::UpdateTransform(MovementType _Type)
{
	switch (_Type) {
	case MovementType::MOVE_FORWARD:
		Position += Front * MovementSpeed;
		ViewMatrix = glm::lookAt(Position, Position + Front, Up);
		break;
	case MovementType::MOVE_BACKWARD:
		Position += -Front * MovementSpeed;
		ViewMatrix = glm::lookAt(Position, Position + Front, Up);
		break;
	case MovementType::MOVE_LEFT:
		Position += -glm::normalize(glm::cross(Front, Up)) * MovementSpeed;
		ViewMatrix = glm::lookAt(Position, Position + Front, Up);
		break;
	case MovementType::MOVE_RIGHT:
		Position += glm::normalize(glm::cross(Front, Up)) * MovementSpeed;
		ViewMatrix = glm::lookAt(Position, Position + Front, Up);
		break;
	default:
		break;
			
	}
}
