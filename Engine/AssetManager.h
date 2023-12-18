#pragma once
#include "Cache.h"
#include "Shader.h"
#include "Texture.h"

// The AssetManger is a singleton class that helps load assets on demand
// and cache them so that subsequent loads will return the cached asset
// instead of having to load them again. This manager provides fucntions
// to help save/load assets when needed.
class AssetManager
{
public:
	AssetManager(const AssetManager&) = delete; // Makes sure the class can't be copied
	AssetManager(AssetManager&&) = delete; // No move constructor
	AssetManager& operator=(const AssetManager&) = delete; // Makes sure the class is not assignable
	AssetManager& operator=(AssetManager&&) = delete; // No move assignment
	
	// Goes to each of the caches and deletes all of their elements
	void Clear();

	// Returns the instance of an AssetManager
	// @return - AssetManager* for the instance of an AssetManager
	static AssetManager* Get();

	// Saves a shader into the shader cache's map
	// @param - const std::string& for the shader's name.
	// @param - Shader* for the shader that is being saved.
	void SaveShader(const std::string& shaderName, Shader* shader) { mShaderCache->StoreCache(shaderName, shader); }
	// Retrieves a shader from the shader cache's map
	// @param - const std::string& for the shader's name.
	// @return - The pointer to the desired shader retrieved from the shader cache map
	Shader* LoadShader(const std::string& shaderName) { return mShaderCache->Get(shaderName); }
	// Clears each element from the shader cache's map
	void ClearShaders() { mShaderCache->Clear(); }

	// Saves a texture into the texture cache's map
	// @param - const std::string& for the texture's name.
	// @param - Texture* for the texture that is being saved. 
	void SaveTexture(const std::string& textureName, Texture* texture) { mTextureCache->StoreCache(textureName, texture); }
	// Retrieves a texture from the texture cache's map
	// @param - const std::string& for the texture's name.
	// @return - The pointer to the desired texture retrieved from the texture cache map
	Texture* LoadTexture(const std::string& textureName) { return mTextureCache->Get(textureName); }
	// Clears each element from the texture cache's map
	void ClearTextures() { mShaderCache->Clear(); }

private:
	AssetManager();
	~AssetManager();

	// Shader cache
	Cache<Shader>* mShaderCache;

	// Texture cache
	Cache<Texture>* mTextureCache;
};