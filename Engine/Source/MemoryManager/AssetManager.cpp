#include "AssetManager.h"
#include <iostream>

AssetManager::AssetManager() :
	mShaderCache(new Cache<Shader>(this)),
	mTextureCache(new Cache<Texture>(this)),
	mMaterialCache(new Cache<Material>(this)),
	mMeshCache(new Cache<Mesh>(this)),
	mModelCache(new Cache<Model>(this)),
	mAnimationCache(new Cache<Animation>(this)),
	mSkeletonCache(new Cache<Skeleton>(this)),
	mShaderProgramCache(new Cache<ShaderProgram>(this))
{
}

AssetManager::~AssetManager()
{
	std::cout << "Deleted asset manager\n";
}

void AssetManager::Shutdown()
{
	std::cout << "Shutdown asset manager\n";

	delete mShaderCache;
	delete mTextureCache;
	delete mMaterialCache;
	delete mMeshCache;
	delete mModelCache;
	delete mAnimationCache;
	delete mSkeletonCache;
	delete mShaderProgramCache;

	mShaderCache = nullptr;
	mTextureCache = nullptr;
	mMaterialCache = nullptr;
	mMeshCache = nullptr;
	mModelCache = nullptr;
	mAnimationCache = nullptr;
	mSkeletonCache = nullptr;
	mShaderProgramCache = nullptr;
}

void AssetManager::Clear()
{
	mShaderCache->Clear();
	mTextureCache->Clear();
	mMaterialCache->Clear();
	mMeshCache->Clear();
	mModelCache->Clear();
	mAnimationCache->Clear();
	mSkeletonCache->Clear();
	mShaderProgramCache->Clear();
}

AssetManager* AssetManager::Get()
{
	static AssetManager s_AssetManager;

	return &s_AssetManager;
}
