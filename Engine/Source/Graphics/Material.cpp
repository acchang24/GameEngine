#include "Material.h"
#include <iostream>
#include "../MemoryManager/AssetManager.h"
#include "Renderer3D.h"
#include "Shader.h"
#include "Texture.h"
#include "UniformBuffer.h"

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
        switch (mTextures[i]->GetType())
        {
        case TextureType::Diffuse:
            name = "diffuse";
            break;
        case TextureType::Specular:
            name = "specular";
            break;
        case TextureType::Emission:
            name = "emission";
            break;
        case TextureType::Normal:
            name = "normal";
            break;
        };

        // Activate proper texture unit before binding
        glActiveTexture(GL_TEXTURE0 + i + 1);
        mShader->SetInt(("textureSamplers." + name), i + 1);
        mTextures[i]->BindTexture();
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