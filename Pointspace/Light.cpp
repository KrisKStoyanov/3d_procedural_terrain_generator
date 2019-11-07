#include "Light.h"

Light::Light(glm::vec3 _Direction, glm::vec3 _AmbientC, glm::vec3 _DiffuseC, glm::vec3 _SpecularC, LightType _Type)
{
	Direction = _Direction;
	AmbientC = _AmbientC;
	DiffuseC = _DiffuseC;
	SpecularC = _SpecularC;
	Type = _Type;
}

Light::Light(glm::vec3 _Position, glm::vec3 _AmbientC, glm::vec3 _DiffuseC, glm::vec3 _SpecularC, float _ConstantA, float _LinearA, float _QuadraticA, LightType _Type)
{
	Position = _Position;
	AmbientC = _AmbientC;
	DiffuseC = _DiffuseC;
	SpecularC = _SpecularC;
	ConstantA = _ConstantA;
	LinearA = _LinearA;
	QuadraticA = _QuadraticA;
}

Light::Light(glm::vec3 _Position, glm::vec3 _Direction, glm::vec3 _AmbientC, glm::vec3 _DiffuseC, glm::vec3 _SpecularC, float _ConstantA, float _LinearA, float _QuadraticA, float _CutOff, float _OuterCutOff, LightType _Type)
{
	Position = _Position;
	Direction = _Direction;
	AmbientC = _AmbientC;
	DiffuseC = _DiffuseC;
	SpecularC = _SpecularC;
	ConstantA = _ConstantA;
	LinearA = _LinearA;
	QuadraticA = _QuadraticA;
	CutOff = _CutOff;
	OuterCutOff = _OuterCutOff;
}

Light::~Light()
{
}
