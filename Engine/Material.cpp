#include "Material.h"
#include "Shader.h"
#include "Texture.h"

Material::Material() : 
	mMats({}),
	mShader(nullptr)
{

}

void Material::SetActive()
{
	mShader->SetActive();
	mShader->SetMaterial(this);

    unsigned int diffuseNum = 0;
    unsigned int specularNum = 0;

    std::string number;
    std::string name;

    // Bind the texture on their texture units
    for (size_t i = 0; i < mTextures.size(); ++i)
    {
        switch (mTextures[i]->GetType())
        {
        case TextureType::Diffuse:
            // Set name to diffuse
            name = "diffuse";
            // Set number to diffuseNum
            number = std::to_string(diffuseNum);
            // Increment the diffuseNum count
            ++diffuseNum;
            break;
        case TextureType::Specular:
            // Set name to specular
            name = "specular";
            // Set number to specularNum
            number = std::to_string(specularNum);
            // Increment the specularNum count
            ++specularNum;
            break;
        };

        // Activate proper texture unit before binding
        glActiveTexture(GL_TEXTURE0 + i);
        // Set the sampler to the correct texture unit
        mShader->SetInt(("textureSamplers." + name + number), i);
        // Bind the texture
        mTextures[i]->SetActive();
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
	}
}