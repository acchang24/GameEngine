#include "DirectionalLight.h"
#include <iostream>
#include "AssetManager.h"

DirectionalLight::DirectionalLight(const glm::vec3& dir) :
	Light(),
	mDirection(dir)
{
}

DirectionalLight::DirectionalLight(const glm::vec4& color, const glm::vec3& dir) :
	Light(color),
	mDirection(dir)
{
}

DirectionalLight::~DirectionalLight()
{
	std::cout << "Delete directional light" << std::endl;
}

void DirectionalLight::SetLight()
{
	Shader* s = AssetManager::Get()->LoadShader("phong");
	s->SetActive();
	s->SetDirectionalLight(this);
}
