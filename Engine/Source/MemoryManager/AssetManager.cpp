#include "AssetManager.h"
#include <iostream>
#include "../Util/Logger.h"
#include "../Graphics/ModelLoader.h"

AssetManager::AssetManager() :
	mShaderCache(new Cache<Shader>(this)),
	mTextureCache(new Cache<Texture>(this)),
	mMaterialCache(new Cache<Material>(this)),
	mMeshCache(new Cache<Mesh>(this)),
	mModelCache(new Cache<Model>(this)),
	mAnimationCache(new Cache<Animation>(this)),
	mShaderProgramCache(new Cache<ShaderProgram>(this)),
	mSfxCache(new Cache<SFX>(this)),
	mMusicCache(new Cache<Music>(this))
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
	delete mShaderProgramCache;
	delete mSfxCache;
	delete mMusicCache;
}

void AssetManager::Clear()
{
	mShaderCache->Clear();
	mTextureCache->Clear();
	mMaterialCache->Clear();
	mMeshCache->Clear();
	mModelCache->Clear();
	mAnimationCache->Clear();
	mShaderProgramCache->Clear();
	mSfxCache->Clear();
	mMusicCache->Clear();
}

Shader* AssetManager::LoadShader(const std::string& shaderName)
{
	Shader* shader = mShaderCache->Get(shaderName);

	if (!shader)
	{
		LOG_WARNING("Could not find shader name: " + shaderName);
	}

	return shader;
}

Shader* AssetManager::LoadShader(const std::string& name, const char* vertexFile, const char* fragmentFile, const char* geometryFile)
{
	Shader* shader = mShaderCache->Get(name);

	if (!shader)
	{
		shader = new Shader(name, vertexFile, fragmentFile, geometryFile);

		SaveShader(name, shader);
	}

	return shader;
}

Texture* AssetManager::LoadTexture(const std::string& textureFileName, TextureType type)
{
	Texture* texture = mTextureCache->Get(textureFileName);

	if (!texture)
	{
		texture = new Texture(textureFileName, type);
		SaveTexture(textureFileName, texture);
	}

	return texture;
}

Model* AssetManager::LoadModel(const std::string& modelName)
{
	Model* model = mModelCache->Get(modelName);

	if (!model)
	{
		model = ModelLoader::Load(modelName, this);

		SaveModel(modelName, model);
	}

	return model;
}

ShaderProgram* AssetManager::LoadShaderProgram(const std::string& shaderFileName)
{
	ShaderProgram* shaderProgram = mShaderProgramCache->Get(shaderFileName);

	if (!shaderProgram)
	{
		shaderProgram = new ShaderProgram(shaderFileName);
		SaveShaderProgram(shaderFileName, shaderProgram);
	}

	return shaderProgram;
}

SFX* AssetManager::LoadSFX(const std::string& fileName)
{
	SFX* sfx = mSfxCache->Get(fileName);

	if (!sfx)
	{
		sfx = new SFX(fileName);
		SaveSFX(fileName, sfx);
	}

	return sfx;
}

Music* AssetManager::LoadMusic(const std::string& fileName)
{
	Music* music = mMusicCache->Get(fileName);

	if (!music)
	{
		music = new Music(fileName);
		SaveMusic(fileName, music);
	}

	return music;
}
