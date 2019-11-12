#include "Material.h"

Material::Material(glm::vec4 _AmbientC, glm::vec4 _DiffuseC, glm::vec4 _SpecularC, float _Shininess)
{
	AmbientC = _AmbientC;
	DiffuseC = _DiffuseC;
	SpecularC = _SpecularC;
	Shininess = _Shininess;
}

Material::~Material()
{
}
