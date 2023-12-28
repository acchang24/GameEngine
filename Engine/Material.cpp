#include "Material.h"
#include <iostream>
#include "Shader.h"
#include "Texture.h"

Material::Material() : 
	mMats({}),
	mShader(nullptr)
{

}

Material::~Material()
{
    std::cout << "Delete material" << std::endl;
}

Material::Material(const Material& rhs) : mShader(rhs.mShader), mMats(rhs.mMats)
{
    mTextures = rhs.mTextures;
}

Material& Material::operator=(const Material& rhs)
{
    if (this != &rhs)
    {
        mShader = rhs.mShader;
        mMats = rhs.mMats;
        mTextures = rhs.mTextures;
    }
    return *this;
}

void Material::SetActive()
{
	mShader->SetActive();
	mShader->SetMaterial(this);

    unsigned int diffuseNum = 1;
    unsigned int specularNum = 1;
    unsigned int emissionNum = 1;

    std::string number;
    std::string name;

    for (size_t i = 0; i < mTextures.size(); ++i)
    {
        switch (mTextures[i]->GetType())
        {
        case TextureType::Diffuse:
            name = "diffuse";
            number = std::to_string(diffuseNum);
            ++diffuseNum;
            break;
        case TextureType::Specular:
            name = "specular";
            number = std::to_string(specularNum);
            ++specularNum;
            break;
        case TextureType::Emission:
            name = "emission";
            number = std::to_string(emissionNum);
            ++emissionNum;
            break;
        };

        // Activate proper texture unit before binding
        glActiveTexture(GL_TEXTURE0 + i);
        mShader->SetInt(("textureSamplers." + name + number), i);
        mTextures[i]->SetActive();
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
	}
}