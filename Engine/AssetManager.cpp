#include "AssetManager.h"
#include <iostream>

AssetManager::AssetManager():
	mShaderCache(nullptr),
	mTextureCache(nullptr)
{
	mShaderCache = new Cache<Shader>(this);
	mTextureCache = new Cache<Texture>(this);
}

AssetManager::~AssetManager()
{
	std::cout << "Delete asset manager" << std::endl;
}

void AssetManager::Shutdown()
{
	delete mShaderCache;
	delete mTextureCache;

	mShaderCache = nullptr;
	mTextureCache = nullptr;
}

void AssetManager::Clear()
{
	mShaderCache->Clear();
	mTextureCache->Clear();
}

AssetManager* AssetManager::Get()
{
	static AssetManager s_AssetManager;

	return &s_AssetManager;
}