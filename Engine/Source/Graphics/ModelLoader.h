#pragma once
#include <string>
#include <assimp/scene.h>
#include "VertexLayouts.h"

class AssetManager;
class Material;
class Mesh;
class Model;
class Skeleton;

namespace ModelLoader
{
	// Loads a 3D model by file name using Assimp, and loads all meshes, textures/materials, and animations
	// @param - const std::string& for the file name of the model
	// @param - AssetManager* for the engine's asset manager to cache meshes, textures, and animations
	Model* Load(const std::string& fileName, AssetManager* am);

	// Recursively goes through the scene's nodes and loads any meshes
	// @param - aiNode* for the current scene node that's being processed
	// @param - const aiScene* for the model's assimp scene
	// @param - Model* for the target model to save all vertex and skelteon data
	// @param - Skeleton* if there is a skeleton for this model
	// @param - AssetManager* for the engine's asset manager
	// @param - bool for if the model has animations
	void ProcessNodes(aiNode* node, const aiScene* scene, Model* targetModel, Skeleton* skeleton, AssetManager* am, bool hasAnims);

	// Takes an assimp mesh and store it in our own Mesh object.
	// It then appends that mesh to the entity's vector of meshes.
	// @param - aiMesh* for the current mesh being processed
	// @param - const aiScene* for the model's assimp scene
	// @param - Model* for the target model to save all vertex and skelteon data
	// @param - Skeleton* if there is a skeleton for this model
	// @param - AssetManager* for the engine's asset manager
	// @param - bool for if the model has animations
	Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene, Model* targetModel, Skeleton* skeleton, AssetManager* am, bool hasAnims);

	// Extracts vertex data and returns a vertex containing pos, normal, and texture coordinates
	// @param - const aiMesh* for the mesh being processed
	// @param - bool for if there are textures
	// @param - index of the mesh
	const Vertex GetVertexData(const aiMesh* mesh, bool hasTextures, unsigned int index);

	// Loads a mesh's materials
	// @param - const aiScene* for the model's assimp scene
	// @param - const aiMesh* for the mesh being processed
	// @param - Model* for the target model to save all vertex and skelteon data
	// @param - AssetManager* for the engine's asset manager
	// @param - bool for if the model has animations
	Material* LoadMaterial(const aiScene* scene, aiMesh* mesh, Model* targetModel, AssetManager* am, bool hasAnims);

	// Loads the textures of the material
	// @param - aiMaterial*
	// @param - aiTextureType
	// @param - Material* for the material
	// @param - Model* for the target model to save all vertex and skelteon data
	void LoadMaterialTextures(aiMaterial* mat, aiTextureType aiTextureType, Material* material, Model* targetModel);
}
