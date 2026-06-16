#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

class Material;
class Mesh;
class Shader;
class Skeleton;

// Model class represents any 3D object
class Model
{
public:
	Model();
	
	// Model destructor:
	// Meshs and Materials are owned/deleted by AssetManager.
	// Don't call delete on any meshes and materials here.
	~Model();

	// Makes this an instanced model so that multiple instances of the same
	// vertices can be rendered with one draw function call. This generates 
	// a new buffer and loops through all the meshes to enable its array attributes
	// and points them to the data needed per instance.
	// @param - unsigned int for the number of instances to draw
	void MakeInstance(unsigned int numInstances);

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

	// Return the number of meshes this model has
	// @return - size_t for num of meshes
	size_t GetNumMeshes() const { return mMeshes.size(); }

	// Returns the model name by its file name
	// @return - const std::string& for the name
	const std::string& GetName() { return mDirectory; }

	// Loops through each mesh and calls Mesh::Draw()
	// @param - const glm::mat4& for the model matrix
	void Draw(const glm::mat4& model);

	// Loops through each mesh and calls Mesh::Draw() using specified shader
	// @param - const glm::mat4& for the model matrix
	// @param - Shader* to use
	void Draw(Shader* s, const glm::mat4& model);

	// Returns true if there are animations for this model
	// @return - bool for animations or no animations
	bool HasAnimations() const { return mHasAnimations; }

	// Sets the name/directory
	// @param - const std::string& for the name
	void SetName(const std::string& name) { mDirectory = name; }

	// Sets the model's skeleton
	// @param - Skeleton* for the skeleton
	void SetSkeleton(Skeleton* skeleton) { mSkeleton = skeleton; }

	void SaveMaterial(const std::string& name, Material* material) { mMaterialMap[name] = material; }

	void SetHasAnimations(bool anim) { mHasAnimations = anim; }

	Skeleton* GetSkeleton() { return mSkeleton; }

private:
	// Model's vector of meshes
	std::vector<Mesh*> mMeshes;

	// Map of the model's materials. Each mesh of this model
	// is indexed to one of these materials.
	std::unordered_map<std::string, Material*> mMaterialMap;

	// The model's file directory/name
	std::string mDirectory;

	// Model's skeleton for animation
	Skeleton* mSkeleton;

	// Bool for if this model has animations
	bool mHasAnimations;
};
