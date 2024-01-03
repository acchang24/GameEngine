#include "AssetManager.h"
#include <iostream>

AssetManager::AssetManager() :
	mShaderCache(nullptr),
	mTextureCache(nullptr),
	mMaterialCache(nullptr)
{
	mShaderCache = new Cache<Shader>(this);
	mTextureCache = new Cache<Texture>(this);
	mMaterialCache = new Cache<Material>(this);
	mBufferCache = new Cache<UniformBuffer>(this);
}

AssetManager::~AssetManager()
{
	std::cout << "Delete asset manager" << std::endl;
}

void AssetManager::Shutdown()
{
	delete mShaderCache;
	delete mTextureCache;
	delete mMaterialCache;
	delete mBufferCache;

	mShaderCache = nullptr;
	mTextureCache = nullptr;
	mMaterialCache = nullptr;
	mBufferCache = nullptr;
}

void AssetManager::Clear()
{
	mShaderCache->Clear();
	mTextureCache->Clear();
	mMaterialCache->Clear();
	mBufferCache->Clear();
}

AssetManager* AssetManager::Get()
{
	static AssetManager s_AssetManager;

	return &s_AssetManager;
}
