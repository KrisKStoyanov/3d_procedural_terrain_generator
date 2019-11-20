#pragma once
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Transform.h"

enum class MovementType {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera
{
public:
	Camera(glm::vec3 _Position, float _FOV, float _ProjWidth, float _ProjHeight);
	~Camera();

	//glm::vec3 Position, Orientation, Up, Right, WorldUp;
	GLfloat Yaw, Pitch, MovementSpeed, MouseSensitivity;
	glm::mat4 ViewMatrix, ProjectionMatrix;

	void UpdateTransformKeyboard(MovementType _Type, float _DeltaTime);
	void UpdateTransformMouse(GLfloat _CursorX, GLfloat _CursorY);
	void UpdateFrontDirection();

	float LastMousePosX;
	float LastMousePosY;
	bool FirstMouse = true;

	inline void SetTransform(Transform* _transform) { m_Transform = _transform; }
	inline Transform* GetTransform() { return m_Transform; }
private:
	Transform* m_Transform;
};

