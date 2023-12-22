#include "PointLight.h"
#include <iostream>
#include "AssetManager.h"
#include "Sphere.h"

PointLight::PointLight(const glm::vec4& color, const glm::vec3& position, float constant, float linear, float quadratic) :
	Light(color),
	mPointLightData({position, constant, linear, quadratic})
{
	mLightSphere = new Sphere(glm::vec4(1.0f,1.0f,1.0f,1.0f));
	mLightSphere->SetPosition(position);
	mLightSphere->SetScale(0.1f);
}

PointLight::~PointLight()
{
	std::cout << "Delete point light" << std::endl;
}

void PointLight::SetLight()
{
	mPointLightData.position = mLightSphere->GetPosition();

	Shader* s = AssetManager::Get()->LoadShader("phong");
	s->SetActive();
	s->SetPointLight(this);
}
