#include "MaterialCubeMap.h"
#include <iostream>
#include "Graphics/Shader.h"
#include "Graphics/UniformBuffer.h"
#include "Graphics/Texture.h"
#include "Graphics/CubeMap.h"

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
	mCubeMap->SetActive(mShader);
	mMaterialBuffer->UpdateBufferData(&mMats);
}
