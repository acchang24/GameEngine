#pragma once
#include <vector>
#include <glm/glm.hpp>

class Shader;
class Texture;

// Struct to define the material's ambient, diffuse, specular colors, shininess, and specular intensity.
struct MaterialColors
{
	glm::vec4 diffuseColor;		// The material's diffuse color
	glm::vec4 specularColor;	// The material's specular color
	float specularIntensity;	// How strong the specular light is
	float shininess;			// How concentrated the specular light is
	bool hasDiffuseTexture;		// If the material has a diffuse texture (this is just normal textures)
	bool hasSpecularTexture;	// If the material has a specular texture (used for specular maps)
};

// The Material class helps create the object's material's colors and properties it surface. It defines
// the diffuse and specular components of an object, as well as containing information
// about the object's textures.
class Material
{
public:
	Material();
	~Material();
	Material(const Material& rhs);
	Material& operator=(const Material& rhs);

	// Sets the material's shader as active. It also loops
	// through the texture vector and binds any textures
	// onto its texture units and sends the MaterialColors
	// struct to the shader.
	void SetActive();

	// Adds a texture to the material's vector of textures.
	// Sets the material's diffuse and specular texture status
	// based on the type of the texture.
	// @param - Texture* for the new texture
	void AddTexture(Texture* t);

	// Gets the material's colors
	// @returns - const MaterialColors& for the material's colors
	const MaterialColors& GetMats() const { return mMats; }
	// Gets the material's shader
	// @returns - Shader* for the material's shader
	Shader* GetShader() const { return mShader; }

	// Sets the material's colors
	// @param - const MaterialColors& for the new material colors
	void SetMaterialColors(const MaterialColors& color) { mMats = color; }
	// Sets the material's diffuse color
	// @param - const glm::vec4& for the new diffuse color
	void SetDiffuseColor(const glm::vec4& diffuse) { mMats.diffuseColor = diffuse; }
	// Sets the material's specular color
	// @param - const glm::vec4& for the new specular color
	void SetSpecularColor(const glm::vec4& specular) { mMats.specularColor = specular; }
	// Sets the material's specular intensity
	// @param - float for the new specular intensity
	void SetSpecularIntensity(float intensity) { mMats.specularIntensity = intensity; }
	// Sets the material's shininess
	// @param - float for the new shininess
	void SetShininess(float shininess) { mMats.shininess = shininess; }
	// Sets whether or not a material has diffuse texture
	// @param - bool for if the material has a diffuse texture
	void SetHasDiffuseTexture(bool diffuse) { mMats.hasDiffuseTexture = diffuse; }
	// Sets whether or not a material has specular texture
	// @param - bool for if the material has a specular texture
	void SetHasSpecularTexture(bool specular) { mMats.hasSpecularTexture = specular; }
	// Set the material's shader
	// @param - Shader* for the new shader
	void SetShader(Shader* shader) { mShader = shader; }

	// Changes texture based on its index
	void ChangeTexture(size_t index, Texture* texture) { mTextures[index] = texture; }

private:
	// Material's color
	MaterialColors mMats;

	// Shader used for this material
	Shader* mShader;

	// Vector of textures used by this material
	std::vector<Texture*> mTextures;
};
