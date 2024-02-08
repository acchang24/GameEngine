#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include <assimp/scene.h>

class Mesh;
class Skeleton;
class Material;
class AssetManager;
class Entity3D;

// Model class represents any 3D object
class Model
{
public:
	// Model constructor loads the model using assimp
	// @param - const std::string& for the model's file name
	// @param - Entity3D* for the model's entity
	Model(const std::string& fileName, Entity3D* entity);
	~Model();

	// Makes this an instanced model so that multiple instances of the same
	// vertices can be rendered with one draw function call. This generates 
	// a new buffer and loops through all the meshes to enable its array attributes
	// and points them to the data needed per instance.
	// @param - unsigned int for the number of instances to draw
	void MakeInstance(unsigned int numInstances);

	// Loads the model's file using Assimp
	// @param - const std::string& for the file name
	// @param - Entity3D* for the model's entity
	bool LoadModel(const std::string& fileName, Entity3D* entity);
	// Recursively goes through the scene's nodes and loads any meshes
	// @param - aiNode*
	// @param - const aiScene*
	// @param - Skeleton* if there is a skeleton for this model
	void ProcessNodes(aiNode* node, const aiScene* scene, Skeleton* skeleton);
	// Takes an assimp mesh and store it in our own Mesh object.
	// It then appends that mesh to the entity's vector of meshes.
	// @param - aiMesh*
	// @param - const aiScene*
	// @param - Skeleton* if there is a skeleton for this model
	Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene, Skeleton* skeleton);
	// Loads the textures of the material
	// @param - aiMaterial*
	// @param - aiTextureType
	// @param - Material* for the material
	void LoadMaterialTextures(aiMaterial* mat, aiTextureType aiTextureType, Material* material, AssetManager* am);

	// Adds a mesh to the model's vector of meshes
	// @param - Mesh* for the new mesh
	void AddMesh(Mesh* m) { mMeshes.emplace_back(m); }
	// Gets the model's vector of meshes (can change data)
	// @return - std::vector<Mesh*>& for the vector of meshes
	std::vector<Mesh*>& GetMeshes() { return mMeshes; }

	// Gets a material by name through the entity's material map
	// @param - const std::string& for the name of the material
	// @return - Material* for the desired material
	Material* GetMaterial(const std::string& name) { return mMaterialMap[name]; }

	// Loops through each mesh and calls Mesh::Draw()
	// @param - const glm::mat4& for the model matrix
	void Draw(const glm::mat4& model);

	Skeleton* GetSkeleton() { return mSkeleton; }

private:
	// Model's vector of meshes
	std::vector<Mesh*> mMeshes;

	// Map of the model's materials. Each mesh of this model
	// is indexed to one of these materials.
	std::unordered_map<std::string, Material*> mMaterialMap;

	// The model's file directory/name
	std::string mDirectory;

	// Model's skeleton, if it has one
	Skeleton* mSkeleton;

	// Number of meshes
	size_t mNumMeshes;
	// Number of materials
	size_t mNumMaterials;
	// Number of textures
	size_t mNumTextures;
};
