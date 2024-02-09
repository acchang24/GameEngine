#include "AssetManager.h"
#include <iostream>

AssetManager::AssetManager() :
	mShaderCache(new Cache<Shader>(this)),
	mTextureCache(new Cache<Texture>(this)),
	mMaterialCache(new Cache<Material>(this)),
	mMeshCache(new Cache<Mesh>(this)),
	mBufferCache(new Cache<UniformBuffer>(this)),
	mModelCache(new Cache<Model>(this)),
	mAnimationCache(new Cache<Animation>(this))
{
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
	delete mMeshCache;
	delete mBufferCache;
	delete mModelCache;
	delete mAnimationCache;

	mShaderCache = nullptr;
	mTextureCache = nullptr;
	mMaterialCache = nullptr;
	mMeshCache = nullptr;
	mBufferCache = nullptr;
	mModelCache = nullptr;
	mAnimationCache = nullptr;
}

void AssetManager::Clear()
{
	mShaderCache->Clear();
	mTextureCache->Clear();
	mMaterialCache->Clear();
	mMeshCache->Clear();
	mBufferCache->Clear();
	mModelCache->Clear();
	mAnimationCache->Clear();
}

AssetManager* AssetManager::Get()
{
	static AssetManager s_AssetManager;

	return &s_AssetManager;
}
