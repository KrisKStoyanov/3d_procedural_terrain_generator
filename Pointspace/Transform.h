#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>

class Transform
{
public:
	Transform(
		glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 _rotation = glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3 _scale = glm::vec3(1.0f, 1.0f, 1.0f));
	//Copy constructor
	inline Transform(const Transform& _other) :
		m_Position(_other.m_Position),
		m_Rotation(_other.m_Rotation),
		m_Scale(_other.m_Scale)
	{}
	~Transform();
	void Translate(glm::vec3 _translation);
	void Rotate(float _angle, glm::vec3 _rotationAxis);
	void Scale(glm::vec3 _scalingRatio);
	void LookAt(Transform _target, glm::vec3 _worldUp = glm::vec3(0.0f, 1.0f, 0.0f));

	inline void SetPosition(glm::vec3 _position) { m_Position = _position; }
	inline void SetOrientation(glm::vec3 _rotation) {
		m_Rotation = _rotation;
		m_LocalRightDir = glm::normalize(glm::cross(m_Rotation, m_WorldUpDir));
		m_LocalUpDir = glm::normalize(glm::cross(m_LocalRightDir, m_Rotation));
	}
	inline glm::vec3 GetPosition() { return m_Position; }
	inline glm::vec3 GetOrientation() { return m_Rotation; };

	inline glm::vec3 GetRightDir() { return m_LocalRightDir; }
	inline glm::vec3 GetUpDir() { return m_LocalUpDir; }

	inline glm::vec3 GetWorldUp() { return m_WorldUpDir; }

	inline glm::mat4 GetModelMatrix() { return m_ModelMatrix; }

private:
	glm::vec3 m_Position;
	glm::vec3 m_Scale;
	glm::vec3 m_Rotation;

	glm::mat4 m_ModelMatrix = glm::mat4(1.0f);

	//Overwrite on every rotation
	glm::vec3 m_LocalRightDir = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 m_LocalUpDir = glm::vec3(0.0, 1.0f, 0.0f);

	glm::vec3 m_WorldUpDir = glm::vec3(0.0f, 1.0f, 0.0f);
};