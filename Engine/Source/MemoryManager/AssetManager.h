#pragma once
#include "Cache.h"
#include "../Animation/Animation.h"
#include "../Animation/Skeleton.h"
#include "../Audio/Sound.h"
#include "../Graphics/Shader.h"
#include "../Graphics/ShaderProgram.h"
#include "../Graphics/Texture.h"
#include "../Graphics/Material.h"
#include "../Graphics/UniformBuffer.h"
#include "../Graphics/Mesh.h"
#include "../Graphics/Model.h"

class Renderer;

// The AssetManager is a singleton class that helps load assets on demand
// and cache them so that subsequent loads will return the cached asset
// instead of having to load them again. This manager provides fucntions
// to help save/load assets when needed.
class AssetManager
{
public:
	// Goes to each of the asset caches and calls the cache's Clear()
	void Clear();

	// Deletes the shader caches
	void Shutdown();

	// Returns the instance of an AssetManager
	// @return - AssetManager* for the instance of an AssetManager
	static AssetManager* Get();

	// Sets a pointer to the renderer
	void SetRenderer(Renderer* renderer) { mRenderer = renderer; }

	// Gets a pointer to the game's renderer
	// @return - Renderer3D* for the renderer
	Renderer* GetRenderer() { return mRenderer; }

	
	// Saves a shader into the shader cache's map
	// @param - const std::string& for the shader's name.
	// @param - Shader* for the shader that is being saved.
	void SaveShader(const std::string& shaderName, Shader* shader) { mShaderCache->StoreCache(shaderName, shader); }

	// Loads a shader from the shader cache's map if it exists, nullptr if not.
	// Ownership of any Shader* returned from this method is handled by the AssetManager. Don't need to free memory manually.
	// Call AssetManager::DeleteShader() if you need to delete/remove a shader by name
	// @param - const std::string& for the shader's name.
	// @return - Shader* for the desired shader retrieved from the shader cache map
	static Shader* LoadShader(const std::string& shaderName);

	// Creates and returns a shader, saving it in the shader cache's map if it doesn't exist.
	// Ownership of any Shader* returned from this method is handled by the AssetManager. Don't need to free memory manually.
	// Call AssetManager::DeleteShader() if you need to delete/remove a shader by name
	// @param - const std::string& for the name of the shader (used to acces through AssetManager)
	// @param - const char* for the vertex shader name/file path
	// @param - const char* for the fragment shader name/file path
	// @param - const char* for the geometry shader name/file path if it exists (defaults to nullptr)
	// @return - Shader* for the newly created shader
	static Shader* LoadShader(const std::string& name, const char* vertexFile, const char* fragmentFile, const char* geometryFile = nullptr);

	// Deletes/clears each element from the shader cache's map
	void ClearShaders() { mShaderCache->Clear(); }

	// Deletes a shader in the shader cache map by name
	// @param - const std::string& for the shader name
	void DeleteShader(const std::string& shaderName) { mShaderCache->Delete(shaderName); }


	// Saves a texture into the texture cache's map
	// @param - const std::string& for the texture's name.
	// @param - Texture* for the texture that is being saved. 
	void SaveTexture(const std::string& textureFileName, Texture* texture) { mTextureCache->StoreCache(textureFileName, texture); }

	// Loads a texture from the texture cache's map if it exists, nullptr if not.
	// Ownership of any Texture* returned from this method is handled by the AssetManager. Don't need to free memory manually.
	// Call AssetManager::DeleteTexture() if you need to delete/remove a texture by name
	// @param - const std::string& for the texture name
	// @return - Texture* for the desired texture
	static Texture* LoadTexture(const std::string& textureFileName) { return AssetManager::Get()->mTextureCache->Get(textureFileName); }

	// Creates and returns a texture, saving it in the texture cache's map if it doesn't exist.
	// Ownership of any Texture* returned from this method is handled by the AssetManager. Don't need to free memory manually.
	// Call AssetManager::DeleteTexture() if you need to delete/remove a texture by name
	// @param - const std::string& for the texture name
	// @param - TextureType for the type
	// @return - Texture* for the desired texture
	static Texture* LoadTexture(const std::string& textureFileName, TextureType type);

	// Deletes/clears each element from the texture cache's map
	void ClearTextures() { mShaderCache->Clear(); }

	// Deletes a texture in the texture cache map by name
	// @param - const std::string& for the texture name
	void DeleteTexture(const std::string& textureName) { mTextureCache->Delete(textureName); }


	// Saves a material into the material cache's map
	// @param - const std::string& for the material's name
	// @param - Material* for the material that is being saved
	void SaveMaterial(const std::string& materialName, Material* material) { mMaterialCache->StoreCache(materialName, material); }

	// Loads a material from the material cache's map if it exists, nullptr if not.
	// Ownership of any Material* returned from this method is handled by the AssetManager. Don't need to free memory manually.
	// Call AssetManager::DeleteMaterial() if you need to delete/remove a material by name
	// @param - const std::string& for the material's name
	// @return - Material* for the desired material retrieved from the material cache map
	Material* LoadMaterial(const std::string& materialName) { return mMaterialCache->Get(materialName); }

	// Deletes/clears each element from the material cache's map
	void ClearMaterials() { mMaterialCache->Clear(); }

	// Deletes a material in the material cache map by name
	// @param - const std::string& for the material name
	void DeleteMaterial(const std::string& materialName) { mMaterialCache->Delete(materialName); }


	// Saves a mesh into the mesh cache's map
	// @param - const std::string& for the mesh's name
	// @param - Mesh* for the mesh that is being saved
	void SaveMesh(const std::string& meshName, Mesh* mesh) { mMeshCache->StoreCache(meshName, mesh); }

	// Loads a mesh from the mesh cache's map if it exists, nullptr if not.
	// Ownership of any Mesh* returned from this method is handled by the AssetManager. Don't need to free memory manually.
	// Call AssetManager::DeleteMesh() if you need to delete/remove a mesh by name
	// @param - const std::string& for the mesh's name
	// @return - Mesh* for the desired mesh retrieved from the mesh cache map
	Mesh* LoadMesh(const std::string& meshName) { return mMeshCache->Get(meshName); }

	// Deletes/clears each element from the mesh cache's map
	void ClearMesh() { mMeshCache->Clear(); }

	// Deletes a mesh in the mesh cache map by name
	// @param - const std::string& for the mesh name
	void DeleteMesh(const std::string& meshName) { mMeshCache->Delete(meshName); }


	// Saves a model into the model cache's map
	// @param - const std::string& for the model's name
	// @param - Model* for the model to save
	void SaveModel(const std::string& modelName, Model* model) { mModelCache->StoreCache(modelName, model); }

	// Loads a model from the model cache's map if it exists, nullptr if not.
	// Ownership of any Model* returned from this method is handled by the AssetManager. Don't need to free memory manually.
	// Call AssetManager::DeleteModel() if you need to delete/remove a model by name
	// @param - const std::string& for the model's name
	// @return - Model* for the desired model retrieved from the model cache map
	static Model* LoadModel(const std::string& modelName) { return AssetManager::Get()->mModelCache->Get(modelName); }

	// Deletes/clears each element from the model cache map
	void ClearModels() { mModelCache->Clear(); }

	// Deletes a model in the model cache map by name
	// @param - const std::string& for the model name
	void DeleteModel(const std::string& modelName) { mModelCache->Delete(modelName); }


	// Saves an animation into the animation cache's map
	// @param - const std::string& for the animation's name
	// @param - Animation* for the animation to save
	static void SaveAnimation(const std::string& animationName, Animation* animation) { AssetManager::Get()->mAnimationCache->StoreCache(animationName, animation); }

	// Loads an animation from the animation cache's map if it exists, nullptr if not.
	// Ownership of any Animation* returned from this method is handled by the AssetManager. Don't need to free memory manually.
	// Call AssetManager::DeleteAnimation() if you need to delete/remove a animation by name
	// @param - const std::string& for the animation's name
	// @return - Animation* for the desired animation retrieved from the animation cache map
	static Animation* LoadAnimation(const std::string& animName) { return AssetManager::Get()->mAnimationCache->Get(animName); }

	// Deletes/clears each element from the animation cache map
	void ClearAnimations() { mAnimationCache->Clear(); }

	// Deletes an animation in the animation cache map by name
	// @param - const std::string& for the animation name
	void DeleteAnimation(const std::string& animationName) { mAnimationCache->Delete(animationName); }


	// Saves an skeleton into the skeleton cache's map
	// @param - const std::string& for the skeleton's name
	// @param - Skeleton* for the skeleton to save
	static void SaveSkeleton(const std::string& skeletonName, Skeleton* skeleton) { AssetManager::Get()->mSkeletonCache->StoreCache(skeletonName, skeleton); }
	
	// Loads an skeleton from the skeleton cache's map if it exists, nullptr if not.
	// Ownership of any Skeleton* returned from this method is handled by the AssetManager. Don't need to free memory manually.
	// Call AssetManager::DeleteSkeleton() if you need to delete/remove a skeleton by name
	// @param - const std::string& for the skeleton's name
	// @return - Skeleton* for the desired skeleton retrieved from the skeleton cache map
	static Skeleton* LoadSkeleton(const std::string& skeletonName) { return AssetManager::Get()->mSkeletonCache->Get(skeletonName); }
	
	// Deletes/clears each element from the skeleton cache map
	void ClearSkeletons() { mSkeletonCache->Clear(); }

	// Deletes a skeleton in the skeleton cache map by name
	// @param - const std::string& for the skeleton name
	void DeleteSkeleton(const std::string& skeletonName) { mSkeletonCache->Delete(skeletonName); }


	// Saves a ShaderProgram into the shader program cache's map
	// @param - const std::string& for the shader's file name
	// @param - ShaderProgram* for the shader program to save
	void SaveShaderProgram(const std::string& shaderFileName, ShaderProgram* program) { mShaderProgramCache->StoreCache(shaderFileName, program); }
	
	// Loads an ShaderProgram from the ShaderProgram cache's map if it exists, nullptr if not.
	// Ownership of any ShaderProgram* returned from this method is handled by the AssetManager. Don't need to free memory manually.
	// Call AssetManager::DeleteShaderProgram() if you need to delete/remove a ShaderProgram by name
	// @param - const std::string& for the ShaderProgram's name
	// @return - ShaderProgram* for the desired ShaderProgram retrieved from the ShaderProgram cache map
	static ShaderProgram* LoadShaderProgram(const std::string& shaderFileName);

	// Deletes/clears each element from the shader program cache map
	void ClearShaderPrograms() { mShaderProgramCache->Clear(); }

	// Deletes a shader program by name
	// @param - const std::string& for the shader file name
	void DeleteShaderProgram(const std::string& shaderFileName) { mShaderProgramCache->Delete(shaderFileName); }


	// Saves an SFX into the sfx program cache's map
	// @param - const std::string& for the SFX's file name
	// @param - SFX* for the sfx to save
	void SaveSFX(const std::string& fileName, SFX* sfx) { mSfxCache->StoreCache(fileName, sfx); }

	// Loads an SFX file from SFX cache's map if it exists, nullptr if not.
	// Ownership of any SFX* returned from this method is handled by the AssetManager. Don't need to free memory manually.
	// Call AssetManager::DeleteSFX() if you need to delete/remove an SFX by name
	// @param - const std::string& for the SFX file name
	// @return - SFX* for the desired SFX retrieved from the SFX cache map
	static SFX* LoadSFX(const std::string& fileName);

	// Deletes/clears each element from the sfx cache map
	void ClearSFX() { mSfxCache->Clear(); }

	// Deletes an sfx by name
	// @param - const std::string& for the sfx file name
	void DeleteSfx(const std::string& fileName) { mSfxCache->Delete(fileName); }


	// Saves a Music into the music program cache's map
	// @param - const std::string& for the music file name
	// @param - Music* for the Music to save
	void SaveMusic(const std::string& fileName, Music* music) { mMusicCache->StoreCache(fileName, music); }

	// Loads a music file from music cache's map if it exists, nullptr if not.
	// Ownership of any Music* returned from this method is handled by the AssetManager. Don't need to free memory manually.
	// Call AssetManager::DeleteMusic() if you need to delete/remove an Music by name
	// @param - const std::string& for the Music file name
	// @return - Music* for the desired Music retrieved from the SFX cache map
	static Music* LoadMusic(const std::string& fileName);

	// Deletes/clears each element from the music cache map
	void ClearMusic() { mMusicCache->Clear(); }

	// Deletes a music by name
	// @param - const std::string& for the music file name
	void DeleteMusic(const std::string& fileName) { mMusicCache->Delete(fileName); }

private:
	AssetManager();
	~AssetManager();
	AssetManager(const AssetManager&) = delete; // Makes sure the class can't be copied
	AssetManager(AssetManager&&) = delete; // No move constructor
	AssetManager& operator=(const AssetManager&) = delete; // Makes sure the class is not assignable
	AssetManager& operator=(AssetManager&&) = delete; // No move assignment

	// Pointer to the game's renderer (does not own the renderer, Game will take care of deleting this pointer)
	Renderer* mRenderer;

	// Shader cache
	Cache<Shader>* mShaderCache;

	// Texture cache
	Cache<Texture>* mTextureCache;

	// Material cache
	Cache<Material>* mMaterialCache;

	// Mesh cache
	Cache<Mesh>* mMeshCache;

	// Model cache
	Cache<Model>* mModelCache;

	// Animation cache
	Cache<Animation>* mAnimationCache;

	// Skeleton cache
	Cache<Skeleton>* mSkeletonCache;

	// ShaderProgram cache
	Cache<ShaderProgram>* mShaderProgramCache;

	// SFX cache
	Cache<SFX>* mSfxCache;

	// Music sound track cache
	Cache<Music>* mMusicCache;
};
