#include "SpotLight.h"
#include <iostream>
#include "Sphere.h"
#include "AssetManager.h"

SpotLight::SpotLight(const glm::vec4& color, const glm::vec3& pos, const glm::vec3& dir, float cutoff, float outerCutoff, float constant, float linear, float quadratic) :
	Light(color),
	mSpotLightData({pos, dir, cutoff, outerCutoff, constant, linear, quadratic})
{
	mLightSphere = new Sphere(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	mLightSphere->SetPosition(pos);
	mLightSphere->SetScale(0.1f);
}

SpotLight::~SpotLight()
{
	std::cout << "Delete spot light" << std::endl;
}

void SpotLight::SetLight()
{
	mSpotLightData.position = mLightSphere->GetPosition();

	Shader* s = AssetManager::Get()->LoadShader("phong");
	s->SetActive();
	s->SetSpotLight(this);
}
