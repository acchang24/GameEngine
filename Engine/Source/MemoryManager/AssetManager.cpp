#include "AssetManager.h"
#include <iostream>
#include "../Graphics/Renderer.h"

AssetManager::AssetManager() :
	mRenderer(nullptr),
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

Shader* AssetManager::LoadShader(const std::string& name, const char* vertexFile, const char* fragmentFile, const char* geometryFile)
{
	AssetManager* am = AssetManager::Get();

	Shader* shader = am->mShaderCache->Get(name);

	if (!shader)
	{
		shader = new Shader(name, vertexFile, fragmentFile, geometryFile);

		am->SaveShader(name, shader);
	}

	return shader;
}

Texture* AssetManager::LoadTexture(const std::string& textureFileName, TextureType type)
{
	AssetManager* am = AssetManager::Get();

	Texture* texture = am->mTextureCache->Get(textureFileName);

	if (!texture)
	{
		texture = new Texture(textureFileName, type);
		am->SaveTexture(textureFileName, texture);
	}

	return texture;
}

ShaderProgram* AssetManager::LoadShaderProgram(const std::string& shaderFileName)
{
	AssetManager* am = AssetManager::Get();

	ShaderProgram* shaderProgram = am->mShaderProgramCache->Get(shaderFileName);

	if (!shaderProgram)
	{
		shaderProgram = new ShaderProgram(shaderFileName);
		am->SaveShaderProgram(shaderFileName, shaderProgram);
	}

	return shaderProgram;
}
