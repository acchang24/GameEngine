#include "MaterialCubeMap.h"
#include <iostream>
#include "Shader.h"
#include "UniformBuffer.h"
#include "Texture.h"
#include "CubeMap.h"

MaterialCubeMap::MaterialCubeMap() :
	Material(),
	mCubeMap(nullptr)
{
}

MaterialCubeMap::MaterialCubeMap(const MaterialColors& mats) :
	Material(mats),
	mCubeMap(nullptr)
{
}

MaterialCubeMap::~MaterialCubeMap()
{
	std::cout << "Delete material cube map" << std::endl;
}

void MaterialCubeMap::SetActive()
{
	mShader->SetActive();

	mMaterialBuffer->UpdateBufferData(&mMats);

	// Set the proper cubemap sampler uniform in the shader
	mShader->SetInt("cubeMap", mCubeMap->GetTextureUnit());

	mCubeMap->BindCubeMap();
}
