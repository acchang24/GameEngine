#include "Light.h"
#include <iostream>
#include "AssetManager.h"
#include "Shader.h"

Light::Light(const glm::vec4& color) :
	mData({ color, 0.1f, 1.0f, 1.0f, true})
{

}

Light::Light(const glm::vec4& color, float ambient, float diffuse, float specular) :
	mData({color, ambient, diffuse, specular, true})
{

}

Light::~Light()
{
	std::cout << "Delete light" << std::endl;
}

void Light::SetLight()
{

}
