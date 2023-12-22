#include "Light.h"
#include <iostream>
#include "AssetManager.h"
#include "Shader.h"

Light::Light() :
	mData({ glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 0.1f, 1.0f, 1.0f, true})
{
}

Light::~Light()
{
	std::cout << "Delete light" << std::endl;
}

void Light::SetLight() const
{
	Shader* s = AssetManager::Get()->LoadShader("phong");
	s->SetActive();
	s->SetLight(mData);
}
