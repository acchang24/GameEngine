#pragma once
#include "Cache.h"
#include "../Animation/Animation.h"
#include "../Animation/Skeleton.h"
#include "../Graphics/Shader.h"
#include "../Graphics/ShaderProgram.h"
#include "../Graphics/Texture.h"
#include "../Graphics/Material.h"
#include "../Graphics/UniformBuffer.h"
#include "../Graphics/Mesh.h"
#include "../Graphics/Model.h"

class Renderer3D;

// The AssetManager is a singleton class that helps load assets on demand
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

	// Goes to each of the caches calls the cache's Clear()
	void Clear();

	// Deletes the shader caches
	void Shutdown();

	// Returns the instance of an AssetManager
	// @return - AssetManager* for the instance of an AssetManager
	static AssetManager* Get();

	// Sets a pointer to the renderer
	void SetRenderer(Renderer3D* renderer) { mRenderer = renderer; }

	Renderer3D* GetRenderer() { return mRenderer; }

	// Saves a shader into the shader cache's map
	// @param - const std::string& for the shader's name.
	// @param - Shader* for the shader that is being saved.
	void SaveShader(const std::string& shaderName, Shader* shader) { mShaderCache->StoreCache(shaderName, shader); }
	// Retrieves a shader from the shader cache's map
	// @param - const std::string& for the shader's name.
	// @return - Shader* for the desired shader retrieved from the shader cache map
	Shader* LoadShader(const std::string& shaderName) { return mShaderCache->Get(shaderName); }
	// Clears each element from the shader cache's map
	void ClearShaders() { mShaderCache->Clear(); }

	// Saves a texture into the texture cache's map
	// @param - const std::string& for the texture's name.
	// @param - Texture* for the texture that is being saved. 
	void SaveTexture(const std::string& textureFileName, Texture* texture) { mTextureCache->StoreCache(textureFileName, texture); }
	// Retrieves a texture from the texture cache's map if it exists.
	// If not, it will load/create a new texture and save it into the map.
	// Make sure to call Texture::SetType() right after if loading texture for the first time
	// @param - const std::string& for the texture name
	// @return - Texture* for the desired texture
	static Texture* LoadTexture(const std::string& textureFileName);
	// Clears each element from the texture cache's map
	void ClearTextures() { mShaderCache->Clear(); }

	// Saves a material into the material cache's map
	// @param - const std::string& for the material's name
	// @param - Material* for the material that is being saved
	void SaveMaterial(const std::string& materialName, Material* material) { mMaterialCache->StoreCache(materialName, material); }
	// Retrieves a material from the material cache's map
	// @param - const std::string& for the material's name
	// @return - Material* for the desired material retrieved from the material cache map
	Material* LoadMaterial(const std::string& materialName) { return mMaterialCache->Get(materialName); }
	// Clears each element from the material cache's map
	void ClearMaterials() { mMaterialCache->Clear(); }

	// Saves a mesh into the mesh cache's map
	// @param - const std::string& for the mesh's name
	// @param - Mesh* for the mesh that is being saved
	void SaveMesh(const std::string& meshName, Mesh* mesh) { mMeshCache->StoreCache(meshName, mesh); }
	// Retrieves a mesh from the mesh cache's map
	// @param - const std::string& for the mesh's name
	// @return - Mesh* for the desired mesh retrieved from the mesh cache map
	Mesh* LoadMesh(const std::string& meshName) { return mMeshCache->Get(meshName); }
	// Clears each element from the mesh cache's map
	void ClearMesh() { mMeshCache->Clear(); }

	// Saves a model into the model cache's map
	// @param - const std::string& for the model's name
	// @param - Model* for the model to save
	void SaveModel(const std::string& modelName, Model* model) { mModelCache->StoreCache(modelName, model); }
	// Retrieves a model from the model cache's map
	// @param - const std::string& for the model's name
	// @return - Model* for the desired model retrieved from the model cache map
	Model* LoadModel(const std::string& modelName) { return mModelCache->Get(modelName); }
	// Clears each element from the model cache map
	void ClearModels() { mModelCache->Clear(); }

	// Saves an animation into the animation cache's map
	// @param - const std::string& for the animation's name
	// @param - Animation* for the animation to save
	void SaveAnimation(const std::string& animationName, Animation* animation) { mAnimationCache->StoreCache(animationName, animation); }
	// Retrieves an animation from the animation cache's map
	// @param - const std::string& for the animation's name
	// @return - Animation* for the desired animation retrieved from the animation cache map
	Animation* LoadAnimation(const std::string& animName) { return mAnimationCache->Get(animName); }
	// Clears each element from the animation cache map
	void ClearAnimations() { mAnimationCache->Clear(); }

	// Saves an skeleton into the skeleton cache's map
	// @param - const std::string& for the skeleton's name
	// @param - Skeleton* for the skeleton to save
	void SaveSkeleton(const std::string& skeletonName, Skeleton* skeleton) { mSkeletonCache->StoreCache(skeletonName, skeleton); }
	// Retrieves an skeleton from the skeleton cache's map
	// @param - const std::string& for the skeleton's name
	// @return - Skeleton* for the desired skeleton retrieved from the skeleton cache map
	Skeleton* LoadSkeleton(const std::string& skeletonName) { return mSkeletonCache->Get(skeletonName); }
	// Clears each element from the skeleton cache map
	void ClearSkeletons() { mSkeletonCache->Clear(); }

	// Saves a ShaderProgram into the shader program cache's map
	// @param - const std::string& for the shader's file name
	// @param - ShaderProgram* for the shader program to save
	void SaveShaderProgram(const std::string& shaderFileName, ShaderProgram* program) { mShaderProgramCache->StoreCache(shaderFileName, program); }
	// Retrieves a ShaderProgram from the shader program cache's map if it exists.
	// If not, it will load/create a new ShaderProgram and save it into the map
	// @param - const std::string& for the shader file name
	// @return - ShaderProgram* for the desired shader program
	static ShaderProgram* LoadShaderProgram(const std::string& shaderFileName);
	// Clears each element from the shader program cache map
	void ClearShaderPrograms() { mShaderProgramCache->Clear(); }
	// Deletes a shader program by name
	// @param - const std::string& for the shader file name
	void DeleteShaderProgram(const std::string& shaderFileName) { mShaderProgramCache->Delete(shaderFileName); }

private:
	AssetManager();
	~AssetManager();

	// Pointer to the game's renderer (does not own the renderer, Game will take care of deleting this pointer)
	Renderer3D* mRenderer;

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
};
