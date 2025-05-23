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
    {TextureType::Diffuse, "diffuse"},
    {TextureType::Specular, "specular"},
    {TextureType::Emission, "emission"},
    {TextureType::Normal, "normal"},
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

    std::string name;

    for (size_t i = 0; i < mTextures.size(); ++i)
    {
        TextureType type = mTextures[i]->GetType();

        // Check if sampler name exists
        auto iter = s_TextureSamplerNames.find(type);
        if (iter != s_TextureSamplerNames.end())
        {
            // Update sampler name
            name = iter->second;

            // Get the type's texture unit
            int textureUnit = static_cast<int>(type);

            // Activate proper texture unit before binding
            glActiveTexture(GL_TEXTURE0 + textureUnit);
            
            // Set the proper texture sampler
            mShader->SetInt(("textureSamplers." + name), textureUnit);
            
            // Bind the texture
            mTextures[i]->BindTexture();
        }
    }
    glActiveTexture(GL_TEXTURE0);
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