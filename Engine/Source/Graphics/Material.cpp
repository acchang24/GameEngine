#include "Material.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include "../MemoryManager/AssetManager.h"
#include "Renderer3D.h"
#include "Shader.h"
#include "Texture.h"
#include "UniformBuffer.h"

static std::unordered_map<TextureType, std::string> s_TextureSamplerNames = 
{
    {TextureType::Diffuse, "textureSamplers.diffuse"},
    {TextureType::Specular, "textureSamplers.specular"},
    {TextureType::Emission, "textureSamplers.emission"},
    {TextureType::Normal, "textureSamplers.normal"},
};

Material::Material() : 
	mMats({ glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 0.5f, 16.0f, false, false, false, false }),
	mShader(nullptr),
    mMaterialBuffer(AssetManager::Get()->GetRenderer()->GetUniformBuffer("MaterialBuffer"))
{
}

Material::Material(const MaterialColors& mats) :
    mMats(mats),
    mShader(nullptr),
    mMaterialBuffer(AssetManager::Get()->GetRenderer()->GetUniformBuffer("MaterialBuffer"))
{
}

Material::~Material()
{
    std::cout << "Delete material" << std::endl;
}

void Material::SetActive()
{
	mShader->SetActive();
 
    mMaterialBuffer->UpdateBufferData(&mMats);

    std::string samplerName;

    for (size_t i = 0; i < mTextures.size(); ++i)
    {
        // Check if texture type with sampler name exists
        auto iter = s_TextureSamplerNames.find(mTextures[i]->GetType());
        if (iter != s_TextureSamplerNames.end())
        {
            samplerName = iter->second;

            // Set the proper texture sampler uniform in the shader
            mShader->SetInt((samplerName), mTextures[i]->GetTextureUnit());
            
            mTextures[i]->BindTexture();
        }
    }
}

void Material::AddTexture(Texture* t)
{
	mTextures.emplace_back(t);

	switch (t->GetType())
	{
	case TextureType::Diffuse:
		SetHasDiffuseTexture(true);
		break;
	case TextureType::Specular:
		SetHasSpecularTexture(true);
		break;
    case TextureType::Emission:
        SetHasEmissionTexture(true);
        break;
    case TextureType::Normal:
        SetHasNormalTexture(true);
        break;
	}
}