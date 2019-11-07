#version 450 core
precision highp float;
out vec4 FragColor;

struct Material {
	sampler2D TextureMap;
	float Shininess;
};

struct DirLight {
	vec3 Direction;
	vec3 AmbientC;
	vec3 DiffuseC;
	vec3 SpecularC;
};

struct PointLight {
	vec3 Position;
	float ConstantA;
	float LinearA;
	float QuadraticA;
	vec3 AmbientC;
	vec3 DiffuseC;
	vec3 SpecularC;
};

struct SpotLight {
	vec3 Position;
	vec3 Direction;

	float CutOff;
	float OuterCutOff;

	float ConstantA;
	float LinearA;
	float QuadraticA;

	vec3 AmbientC;
	vec3 DiffuseC;
	vec3 SpecularC;
};

in vec3 ExFragPos;
in vec3 ExNormal;
in vec2 ExVertexUV;

uniform vec3 ViewPos;
uniform Material g_Material;
uniform DirLight g_DirLight;
uniform PointLight g_PointLight;
uniform SpotLight g_SpotLight;

vec3 ComputeDirLight(DirLight _Light, vec3 _NormalDir, vec3 _ViewDir);
vec3 ComputePointLight(PointLight _Light, vec3 _NormalDir, vec3 _FragPos, vec3 _ViewDir);
vec3 ComputeSpotLight(SpotLight _Light, vec3 _NormalDir, vec3 _FragPos, vec3 _ViewDir);

void main() {
	vec3 NormalDir = normalize(ExNormal);
	vec3 ViewDir = normalize(ViewPos - ExFragPos);

	vec3 CombinedC = ComputeDirLight(g_DirLight, NormalDir, ViewDir);
	//CombinedC += ComputePointLight(g_PointLight, NormalDir, ExFragPos, ViewDir);
	//CombinedC += ComputeSpotLight(g_SpotLight, NormalDir, ExFragPos, ViewDir);

	FragColor = vec4(CombinedC, 1.0f);
}

vec3 ComputeDirLight(DirLight _Light, vec3 _NormalDir, vec3 _ViewDir)
{
	vec3 lightDir = normalize(-_Light.Direction);
	// diffuse shading
	float diffuseScalar = max(dot(_NormalDir, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, _NormalDir);
	float specularScalar = pow(max(dot(_ViewDir, reflectDir), 0.0), g_Material.Shininess);
	// combine results
	vec3 ambient = _Light.AmbientC * vec3(texture(g_Material.TextureMap, ExVertexUV));
	vec3 diffuse = _Light.DiffuseC * diffuseScalar * vec3(texture(g_Material.TextureMap, ExVertexUV));
	vec3 specular = _Light.SpecularC * specularScalar * vec3(texture(g_Material.TextureMap, ExVertexUV));
	return (ambient + diffuse + specular);
}

vec3 ComputePointLight(PointLight _Light, vec3 _NormalDir, vec3 _FragPos, vec3 _ViewDir)
{
	vec3 lightDir = normalize(-_Light.Position - _FragPos);
	// diffuse shading
	float diffuseScalar = max(dot(_NormalDir, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, _NormalDir);
	float specularScalar = pow(max(dot(_ViewDir, reflectDir), 0.0), g_Material.Shininess);
	// attenuation
	float distance = length(_Light.ConstantA - _FragPos);
	float attenuation = 1.0 / (_Light.ConstantA + _Light.LinearA * distance + _Light.QuadraticA * (distance * distance));
	// combine results
	vec3 ambient = _Light.AmbientC * vec3(texture(g_Material.TextureMap, ExVertexUV));
	vec3 diffuse = _Light.DiffuseC * diffuseScalar * vec3(texture(g_Material.TextureMap, ExVertexUV));
	vec3 specular = _Light.SpecularC * specularScalar * vec3(texture(g_Material.TextureMap, ExVertexUV));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}

vec3 ComputeSpotLight(SpotLight _Light, vec3 _NormalDir, vec3 _FragPos, vec3 _ViewDir)
{
	vec3 lightDir = normalize(_Light.Position - _FragPos);
	// diffuse shading
	float diffuseScalar = max(dot(_NormalDir, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, _NormalDir);
	float specularScalar = pow(max(dot(_ViewDir, reflectDir), 0.0), g_Material.Shininess);
	// attenuation
	float distance = length(_Light.Position - _FragPos);
	float attenuation = 1.0 / (_Light.ConstantA + _Light.LinearA * distance + _Light.QuadraticA * (distance * distance));
	// spotlight intensity
	float theta = dot(lightDir, normalize(-_Light.Direction));
	float epsilon = _Light.CutOff - _Light.OuterCutOff;
	float intensity = clamp((theta - _Light.OuterCutOff) / epsilon, 0.0, 1.0);
	// combine results
	vec3 ambient = _Light.AmbientC * vec3(texture(g_Material.TextureMap, ExVertexUV));
	vec3 diffuse = _Light.DiffuseC * diffuseScalar * vec3(texture(g_Material.TextureMap, ExVertexUV));
	vec3 specular = _Light.SpecularC * specularScalar * vec3(texture(g_Material.TextureMap, ExVertexUV));
	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;
	return (ambient + diffuse + specular);
}