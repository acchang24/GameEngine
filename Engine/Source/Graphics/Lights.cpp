#include "Lights.h"
#include <iostream>
#include "Renderer.h"
#include "UniformBuffer.h"

Lights::Lights() :
	mLightArrays({}),
	mLightBuffer(nullptr)
{

}

Lights::~Lights()
{
	std::cout << "Delete lights" << std::endl;

	DeAllocateLights();
}

void Lights::CreateBuffer(Renderer* renderer)
{
	if (mLightBuffer == nullptr)
	{
		mLightBuffer = renderer->CreateUniformBuffer(sizeof(LightArrays), BufferBindingPoint::Lights, "LightBuffer");
	}
}

void Lights::SetBuffer()
{
	mLightBuffer->UpdateBufferData(&mLightArrays);
}

SpotLight* Lights::AllocateSpotLight(const glm::vec4& color, const glm::vec3& pos, const glm::vec3& dir, float cutoff, float outerCutoff, float constant, float linear, float quadratic)
{
	SpotLight* spotlight = nullptr;

	for (int i = 0; i < MAX_LIGHTS; ++i)
	{
		if (!mLightArrays.spotLights[i].data.isEnabled)
		{
			spotlight = &mLightArrays.spotLights[i];
			spotlight->data.color = color;
			spotlight->data.isEnabled = true;
			spotlight->position = pos;
			spotlight->cutoff = cutoff;
			spotlight->direction = dir;
			spotlight->outerCutoff = outerCutoff;
			spotlight->constant = constant;
			spotlight->linear = linear;
			spotlight->quadratic = quadratic;
			return spotlight;
		}
	}

	return spotlight;
}

PointLight* Lights::AllocatePointLight(const glm::vec4& color, const glm::vec3& position, float constant, float linear, float quadratic)
{
	PointLight* pointLight = nullptr;

	for (int i = 0; i < MAX_LIGHTS; ++i)
	{
		if (!mLightArrays.pointLights[i].data.isEnabled)
		{
			pointLight = &mLightArrays.pointLights[i];
			pointLight->data.color = color;
			pointLight->data.isEnabled = true;
			pointLight->position = position;
			pointLight->constant = constant;
			pointLight->linear = linear;
			pointLight->quadratic = quadratic;
			return pointLight;
		}
	}

	return pointLight;
}

DirectionalLight* Lights::AllocateDirectionalLight(const glm::vec4& color, const glm::vec3& direction)
{
	DirectionalLight* dirLight = nullptr;
	for (int i = 0; i < MAX_DIR_LIGHT; ++i)
	{
		if (!mLightArrays.directionalLight[i].data.isEnabled)
		{
			dirLight = &mLightArrays.directionalLight[i];
			dirLight->data.color = color;
			dirLight->data.isEnabled = true;
			dirLight->direction = direction;
			return dirLight;
		}
	}

	return dirLight;
}

void Lights::DeAllocateLights()
{
	for (unsigned int i = 0; i < MAX_LIGHTS; ++i)
	{
		mLightArrays.pointLights[i].data.isEnabled = false;

		mLightArrays.spotLights[i].data.isEnabled = false;
	}

	for (unsigned int i = 0; i < MAX_DIR_LIGHT; ++i)
	{
		mLightArrays.directionalLight[i].data.isEnabled = false;
	}
}
